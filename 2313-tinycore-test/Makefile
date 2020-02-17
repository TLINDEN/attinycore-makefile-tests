# Arduino Make file. Refer to https://github.com/sudar/Arduino-Makefile

# Arduino stuff
PROJECT_DIR        = $(shell pwd)
BOARD_TAG          = attinyx313
BOARD_SUB          = 2313
ARDUINO_DIR        = /usr/local/arduino
ARDMK_DIR          = /usr/local/arduino/Arduino-Makefile
MONITOR_PORT       = /dev/ttyACM0
ISP_PORT           = /dev/ttyACM0
AVRDUDE            = /usr/local/bin/avrdude
ARDUINO_LIBS       =
ARDUINO_SKETCHBOOK = .

# mk stuff
ALTERNATE_CORE			= ATTinyCore
F_CPU					= 8000000L
MONITOR_BAUDRATE		= 115200
AVRDUDE_ARD_PROGRAMMER	= stk500v2
AVRDUDE_ARD_BAUDRATE	= 9600
AVR_TOOLS_DIR           = /usr/local/avr
AVRDUDE_CONF            = /usr/local/etc/avrdude.conf

# compiler stuff
CFLAGS_STD        = -std=gnu11
CXXFLAGS_STD      = -std=gnu++11
CXXFLAGS         += -pedantic -Wall -Wextra
CURRENT_DIR       = $(shell pwd)

# keep this!
include $(ARDMK_DIR)/Arduino.mk


