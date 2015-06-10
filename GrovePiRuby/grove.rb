require "i2c/i2c"

module Grove
  module Constants
    DIGITAL_READ = 1
    DIGITAL_WRITE = 2
    ANALOG_READ = 3
    ANALOG_WRITE = 4
    PIN_MODE = 5
    # Pin modes, see Arduino.h:
    INPUT = 0
    OUTPUT = 1
    INPUT_PULLUP = 2
  end

  class I2CDev
    I2C_BUS_DEV = "/dev/i2c-1"
    GROVE_ADDRESS = 0x04
    SYNC_TIME = 0.001  # seconds

    include Constants
    
    attr_accessor :address, :bus, :sync_time, :used_pins
    
    def initialize(overwrite_pins=true, sync_time=SYNC_TIME)
      @address = GROVE_ADDRESS
      @bus = I2C::create(I2C_BUS_DEV)
      @sync_time = sync_time
      @used_pins = {}
      @overwrite_pins = overwrite_pins
    end

    def send(cmd, pin, val1=0, val2=0)
      @bus.write @address, cmd, pin, val1, val2
      # must be padded with zeros to indicated unused bytes
    end

    def wait_for_digital
      sleep @sync_time
      @bus.read(@address, 1).ord
    end

    def wait_for_analog
      sleep @sync_time
      bytes = @bus.read @address, 3
      (bytes[1].ord << 8) + bytes[2].ord
    end

    def register_pin(name, pin_obj)
      if !@overwrite_pins && @used_pins.member?(name)
        raise "Pin #{name} already used."
      end
      @used_pins[name] = pin_obj
    end
  end

  class DefaultPin
    include Constants
    
    def get_mode_and_prefix
      [INPUT, "D"]
    end

    def initialize(raw_dev, pin_num)
      @raw = raw_dev
      @pin_num = pin_num
      mode, prefix = get_mode_and_prefix
      @raw.send PIN_MODE, @pin_num, mode
      @raw.register_pin "#{prefix}#{pin_num}", self
    end
  end    

  module Digital
    class Output < DefaultPin
      def get_mode_and_prefix
        [OUTPUT, "D"]
      end
      
      def write(value)
        @raw.send DIGITAL_WRITE, @pin_num, value
      end
    end
    
    class Input < DefaultPin
      def initialize(raw_dev, pin_num, pullup=false)
        @pullup = pullup
        super(raw_dev, pin_num)
      end
      
      def get_mode_and_prefix
        [@pullup ? INPUT_PULLUP : INPUT,
         "D"]
      end
      
      def read
        @raw.send DIGITAL_READ, @pin_num
        @raw.wait_for_digital
      end
    end
  end
  
  class PWMOutput < DefaultPin
    def get_mode_and_prefix
      [OUTPUT, "D"]
    end
    
    def write(value)
      @raw.send ANALOG_WRITE, @pin_num, value
    end
  end
  
  class AnalogInput < DefaultPin
    def get_mode_and_prefix
      [INPUT, "A"]
    end

    def read
      @raw.send ANALOG_READ, @pin_num
      @raw.wait_for_analog
    end      
  end
end
