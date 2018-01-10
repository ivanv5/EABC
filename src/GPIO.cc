#include "GPIO.hh"

#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <thread>
#include <chrono>
#include <map>
#include <fmt/format.h>
#include <fmt/ostream.h>

using namespace GPIO;
using namespace fmt;

#ifdef NDEBUG
const std::string sysfs_path{"/sys/class/gpio"};
#else
# include <iostream>
  const std::string sysfs_path{"."};
#endif

#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>


/* It is easier to provide copy semantics rather than taking care of
 * proper movement around lambdas */
class unix_desc {
public:
    explicit unix_desc(int fd) : _fd{fd} {}
    unix_desc() : _fd{-1} {}
    unix_desc(const unix_desc& f) : _fd{::dup(f._fd)} {}
    unix_desc(unix_desc&& f) { _fd = f._fd; f._fd = -1; }
    ~unix_desc() { if (_fd >= 0) ::close(_fd); }

    unix_desc& operator=(const unix_desc& f) {
	if (_fd >= 0) ::close(_fd);
	_fd = ::dup(f._fd);
    }
    
    unix_desc& operator=(unix_desc&& f) {
	if (_fd >= 0) ::close(_fd);
	_fd = f._fd; f._fd = -1;
    }
    
    explicit operator bool() const { return _fd != -1; }
    int fd() const { return _fd; }
    
protected:
    int _fd;
};


class gpio_file: public unix_desc {
public:
    explicit gpio_file(std::string const& path)
	: unix_desc{::open(path.c_str(), O_RDONLY|O_NONBLOCK)}, _path(path) {}
    gpio_file() {}

    std::string path() const { return _path; }
    bool value() const {
	::lseek(_fd, 0, SEEK_SET);
	char v; int n = ::read(_fd, &v, 1);
	if (n < 0) throw std::runtime_error(format("Unable to read {} ({})", _fd, _path));
	if (v == '1') return true;
	if (v == '0') return false;
	throw std::runtime_error(format("Invalid value read in file {} ({})", _path, v));
    }
    
private:
    std::string _path;
};


class gpio_reactor: public unix_desc {
public:
    gpio_reactor() : unix_desc{::epoll_create1(0)} {
	std::thread t([this] {
		for(;;)
		    this->handle_events();
	    });
	t.detach();
    }

    void handle_events() {
	const int MAX_EVENTS = 10;
	struct epoll_event ev[MAX_EVENTS];
	int n = ::epoll_wait(_fd, ev, MAX_EVENTS, 100);
	if (n < 0)
	    throw std::runtime_error(format("epoll_wait"));
	for (int i=0; i<n; ++i) {
	    int fd = ev[i].data.fd;
	    try { _fds[fd](); }
	    catch(...) { _fds.erase(fd); }
	}
    }

    using callback = std::function<void()>;
    void add(int fd, callback cb) {
	_fds[fd] = cb;
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = fd;
	::epoll_ctl(_fd, EPOLL_CTL_ADD, fd, &ev);
    }

    void remove(int fd) { _fds.erase(fd); }

private:
    std::map<int, callback> _fds;
};



static void retry(int retries, std::function<void()> f);
static gpio_reactor reactor;
static std::map<std::string, int> gpio_fds;

gpio::gpio(int pin, direction d, bool active_low)
    : _pin(pin)
{
    retry(200,
	  [=] {
	      configure(d);
	      configure(edge::NONE);
	      set_active_low(active_low);
	  });
}

gpio::~gpio()
{
    configure(direction::IN);
}

void gpio::configure(direction d)
{
    std::ofstream sysfs(format("{}/gpio{}/direction", sysfs_path, _pin));
    if(!sysfs)
	throw std::runtime_error(format("Unable to set direction of gpio{}", _pin));

    const char* dir_names[] = {
	"in",
	"out"
    };
    sysfs << dir_names[static_cast<int>(d)];
}

void gpio::configure(edge e, std::experimental::optional<gpio::callback> cb)
{
    set_callback(cb);
    std::ofstream sysfs(format("{}/gpio{}/edge", sysfs_path, _pin));
    if(!sysfs)
	throw std::runtime_error(format("Unable to set active edge of gpio{}", _pin));

    const char* edge_names[] = {
	"none",
	"raising",
	"falling",
	"both"
    };
    sysfs << edge_names[static_cast<int>(e)];
}

void gpio::set_callback(std::experimental::optional<gpio::callback> cb)
{
    auto value_path = format("{}/gpio{}/value", sysfs_path, _pin);
    auto prev = gpio_fds.find(value_path);

    if (prev != gpio_fds.end()) {
	reactor.remove(prev->second);
	gpio_fds.erase(prev);
    }

    if (!cb) return;
    
    gpio_file f(value_path);
    reactor.add(f.fd(), [f = std::move(f), cb = *cb] { cb(f.value()); });
    gpio_fds[f.path()] = f.fd();
}
	

void gpio::set_active_low(bool v)
{
    std::ofstream sysfs(format("{}/gpio{}/active_low", sysfs_path, _pin));
    if(!sysfs)
	throw std::runtime_error(format("Unable to set active_low gpio{}", _pin));
    sysfs << (int)v;
}

void gpio::set(bool v)
{
    std::ofstream sysfs(format("{}/gpio{}/value", sysfs_path, _pin));
    if(!sysfs)
	throw std::runtime_error(format("Unable to set gpio{}", _pin));
    sysfs << (int)v;
}

bool gpio::get() const
{
    std::ifstream sysfs(format("{}/gpio{}/value", sysfs_path, _pin));
    if(!sysfs)
	throw std::runtime_error(format("Unable to get gpio{}", _pin));
    bool v;
    sysfs >> v;
    return v;
}

gpio_pin::gpio_pin(int pin) : _pin(pin)
{
    std::ofstream sysfs(format("{}/export", sysfs_path));
    if(!sysfs)
	throw std::runtime_error(format("Unable to export gpio{}", _pin));
    sysfs << pin;
}

gpio_pin::~gpio_pin()
{
    std::ofstream sysfs(format("{}/unexport", sysfs_path));
    if(sysfs) sysfs << _pin;
}

std::ostream&
GPIO::operator<< (std::ostream& os, const gpio_pin& pin)
{
    return os << pin._pin;
}


static void
retry(int retries, std::function<void()> f)
{
    for(int i=0; i<retries; ++i) {
	try {
	    f();
	    return;
	}
	catch(...) {
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    f();
}
