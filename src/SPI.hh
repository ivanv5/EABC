#ifndef SPI_HH
#define SPI_HH

#include "RPi.hh"
#include <array>

namespace SPI {

	/*! \brief SPI bus addressing.

	This class encapsulates the SPI bus address as a CRTP in 
	order to delegate pin control to another class.

	Note that pin_ctrl class must supply a default constructor.
	*/
	template <class pin_ctrl>
	class addressing: public pin_ctrl {
	public:
		addressing() : _addr(~0) {}

		template <class T>
		addressing(const T& cfg): pin_ctrl(cfg) {}

		void set(uint8_t addr) {
			if(update(addr)) pin_ctrl::slave_select(addr);
		}

	protected:
		bool update(uint8_t addr) {
			if (_addr == addr) return false;
			_addr = addr;
			return true;
		}

		uint8_t _addr;
	};

	/*! \brief CS pin control for SPI address.

	This class encapsulates SPI CS handling.  SPI in BCM2835 
	uses upto 3 CS wires but only 2 are actually available in 
	the 40 pin header.

	Polarity is assumed to be active low. This class is agnostic
	with respect to whether a decoder is used to generate actual
	device CS signal from CS[0:2] or not.
	*/
	class cs_ctrl {
	public:
		cs_ctrl() {
			for (uint8_t cs : _cspins)
				bcm2835_spi_setChipSelectPolarity(cs, 0);
		}

		static constexpr size_t size() { return sizeof(_cspins); }

	protected:
		/*! \brief Set current address value.
		*/
		void slave_select(uint8_t addr) {
			if (addr >= sizeof _cspins) return;
			bcm2835_spi_chipSelect(_cspins[addr]); 
		}

	private:
		static const uint8_t _cspins[3];
	};

	/*! \brief GPIO pin control for explicit slave addressing.
	
	In order to avoid performance penalties and dyanmic memory
	usage we use a static size for the pin array.
	*/
	template <unsigned _size>
	class gpio_ctrl {
	public:
		gpio_ctrl() = delete;

		/*! \brief Construct a GPIO slave addresser with a list
		of pins to be used as chip select wires
		(active low).

		Chip select wires for each slave are specified in order.
		Slave 0 uses the first pin, slave 1 the second one, and so
		on.

		\param pins an ordered list of pins to be used as chip
		select wires.
		*/
		template <class T>
		gpio_ctrl(const std::initializer_list<T>& pins) {
			bcm2835_spi_chipSelect(BCM2835_SPI_CS_NONE);
			std::copy(pins.begin(), pins.end(), _pins.begin());
			for (auto pin : _pins)
				bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
		}

		static constexpr size_t size() { return _size; }

	protected:
		/*! \brief Set current address value.

		Valid address is any positive in range [0:size-1]. It
		is assumed that.

		\param addr a valid address in [0:7] range.
		*/
		void slave_select(uint8_t addr) {
			if (addr >= _size) return;
			int target = _pins[addr];
			for (int pin : _pins)
				bcm2835_gpio_write(pin, pin == target ? LOW : HIGH);
		}

		std::array<uint8_t, _size> _pins;
	};

	template <class pin_ctrl>
	class bus : public RPi::base {
	public:
		bus() { init(); }

		template <class T>
		bus(const std::initializer_list<T>& cfg) : _addressing(cfg) { init(); }
	
		~bus() { bcm2835_spi_end(); }

		void set_speed(unsigned s) {
			if (_speed == s) return;
			_speed = s;
			uint16_t div = static_cast<uint16_t>(RPi::SYSTEM_CLOCK_FREQ/s);
			// Divider must be even
			bcm2835_spi_setClockDivider(div & 1);
		}

		void set_address(uint8_t addr) { 
			_addressing.set(addr); 
		}

	private:
		void init() {
			if (!bcm2835_spi_begin())
				throw std::runtime_error("Failed to init SPI. Are you root?");
			bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);
			set_speed(4000000);
		}

	private:
		unsigned _speed;
		addressing<pin_ctrl> _addressing;
	};
    
	template <class pin_ctrl>
	class device {
	public:
		explicit device(uint8_t address) : _address(address) {
		}

		template <class T>
		void read(T& data) const { xfer(data); }

		template <class T>
		void write(const T& data) const { T out; xfer(data, out); }
 
		template <class T>
		void xfer(T& data) const { xfer(data, data); }

		template <class T, class U>
		void xfer(const T& data_out, U& data_in) const {
			_bus.set_address(_address);
			char& in  = reinterpret_cast<char&>(data_in);
			char& out  = const_cast<char&>(reinterpret_cast<const char&>(data_out));
			bcm2835_spi_transfernb(&out, &in,
						std::max(sizeof(T), sizeof(U)));
		}

	private:
		uint8_t _address;
		static bus<pin_ctrl> _bus;
	};


}

namespace RPi {
	using spi_ctrl = SPI::gpio_ctrl<4>;
}

#endif
