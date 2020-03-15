# Arduino Make file. Refer to https://github.com/sudar/Arduino-Makefile

# attiny841:
#   BOARD_TAG          = attinyx41
#   BOARD_SUB          = 841
# attiny861:
#   BOARD_TAG          = attinyx61
#   BOARD_SUB          = 861
# attiny85:
#   BOARD_TAG          = attinyx5
#   BOARD_SUB          = 85
# attiny84:
#   BOARD_TAG          = attinyx4
#   BOARD_SUB          = 84

ARDUINO_VERSION = 10810

PROJECT_DIR              = $(shell pwd)
BOARD_TAG                = attinyx41
BOARD_SUB                = 841
ARDUINO_DIR              = /usr/local/arduino
ARDMK_DIR                = /usr/local/arduino/Arduino-Makefile
MONITOR_PORT             = /dev/ttyACM0
ISP_PORT                 = /dev/ttyACM0
AVRDUDE                  = /usr/local/bin/avrdude
ARDUINO_LIBS             = SPI TinyBME280 RFTransmitter
ARDUINO_SKETCHBOOK       = .

# mk stuff
ALTERNATE_CORE           = ATTinyCore
F_CPU                    = 8000000L
MONITOR_BAUDRATE         = 115200
AVRDUDE_ARD_PROGRAMMER   = stk500v2
AVRDUDE_ARD_BAUDRATE     = 9600
AVR_TOOLS_DIR            = /usr/local/avr
AVRDUDE_CONF             = /usr/local/etc/avrdude.conf

# Micronucleus
MNINST                   = sudo micronucleus

# compiler stuff
CFLAGS_STD               = -std=gnu11
CXXFLAGS_STD             = -std=gnu++11
CXXFLAGS               + = -pedantic -Wall -Wextra
CURRENT_DIR              = $(shell pwd)

# keep this!
include $(ARDMK_DIR)/Arduino.mk

# install using micronucleus
install: $(TARGET_HEX) verify_size
	@echo "########### Press RESET on the Nanite! ##############"
	$(MNINST) $(TARGET_HEX)
