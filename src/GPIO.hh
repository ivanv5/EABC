#ifndef GPIO_HH
#define GPIO_HH
#include <iosfwd>
#include <functional>

#include <experimental/optional>
  
namespace GPIO {

    // RAII class for export/unexport
    class gpio_pin {
		int _pin;

		explicit gpio_pin(int pin);
		~gpio_pin();

		friend class gpio;
		friend std::ostream& operator<< (std::ostream& os, const gpio_pin& pin);
    };

    // message formatting
    std::ostream& operator<< (std::ostream& os, const GPIO::gpio_pin& pin);

    // User-level interface
    class gpio {
    public:
		enum class direction {
			IN,
			OUT
		};
	
		enum class edge {
			NONE,
			RAISING,
			FALLING,
			BOTH
		};

		using callback = std::function<void(bool)>; 

		explicit gpio(int pin,
					direction d = direction::IN,
					bool active_low = false);
		~gpio();
		void set(bool v);
		bool get() const;
		void configure(direction);
		void configure(edge, std::experimental::optional<callback> = {});
		void set_active_low(bool v = true);

	private:
		void set_callback(std::experimental::optional<callback> cb);

	private:
		gpio_pin _pin;
    };
    
}

#endif
