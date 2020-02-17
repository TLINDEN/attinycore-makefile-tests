#-------------------------------------------------------------------------------------------------------
# makefile
#
# written by Steffen from mikrocontrollerspielwiese.de 
#
# inspired from Guido Socher's makefile 
# http://www.linuxfocus.org/Deutsch/November2004/article352.shtml
#
# license: GPL (http://www.gnu.org/licenses/gpl.txt)
#-------------------------------------------------------------------------------------------------------


AVRDUDE=/usr/local/bin/avrdude

# avr-gcc part name
MCU=attiny85

# avrdude part name
PART=t85

CC=avr-gcc
CPP=avr-c++
#CPP=env -P/usr/local/bin:/usr/bin - avr-c++
#CC=env -P/usr/local/bin:/usr/bin - avr-gcc
OBJCOPY=avr-objcopy

PORT=$(shell ls /dev/ttyACM*)

#-------------------
# Programmieradapter

# hier kannst Du Deinen Programmieradapter angeben, wenn Du einen 
# anderen nimmst, als in der Mikrocontrollerspielwiese vorgeschlagen

# Diamex
PROGRAMMER = -c stk500v2 -P $(PORT) -b 9600 -B 1 -v

#der USB-Programmieradapter der Mikrocontrollerspielwiese:
#PROGRAMMER = -c usbasp

#mein Mac-Programmieradapter:
#PROGRAMMER = -c stk500v2 -P /dev/tty.usbmodem431

#
# attiny compatibility layer
#COMPAT=-I/home/scip/devel/at/tiny/cores/tiny

# gloabal defines
DEFS=-DF_CPU=1000000UL

#-------------------
# auf Kleinheit optimieren:
CFLAGS=-g -mmcu=$(MCU) -Wall -Wstrict-prototypes -Os -mcall-prologues -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Wundef -I. $(COMPAT) $(DEFS)
CPPFLAGS=-g -mmcu=$(MCU) -Wall -Os -mcall-prologues -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Wundef -I. $(COMPAT) $(DEFS)

PWD=$(shell pwd)
SRC=$(shell basename $(PWD))
OBJ=$(shell ls *.c *.cpp | sed -e 's/\.cpp/.o/' -e 's/\.c/.o/')


#-------------------
all: $(SRC).hex

#-------------------
help: 
	@echo
	@echo "Moegliche Befehle:"
	@echo "		make all - compiliert Dein Programm und erzeugt die .hex-Datei"
	@echo "		make load - compiliert Dein Programm und schiebt es in den AVR"
	@echo "		make clean - loescht die beim Compilieren erzeugten Dateien"
	@echo "		make rdfuses - gibt Dir Informationen ueber die gesetzten Fusebits und mehr"
	@echo "		make wrfuse4.8mhz - setzt Fusebit fuer 4.8 MHz intern"
	@echo "		make wrfuse9.6mhz - setzt Fusebit fuer 9.6 MHz intern"
	@echo "		make wrfuse128khz - setzt Fusebit fuer 128 kHz intern"
	@echo "		make wrfusecrystal - setzt Fusebit fuer externen Quarz / Crystal (Achtung!)"
	@echo "		make wrfusenoreset - setzt Fusebit fuer PB5 (Achtung!)"
	@echo "		make help - zeigt diesen Hilfetext"
	@echo
	@echo "Achtung: ohne Quarz hast Du keine Chance wrfusecrystal rueckgaengig zu machen!"
	@echo
	@echo "Achtung: wrfusenoreset schaltet PB5 frei und deaktiviert RESET !!!"
	@echo

#-------------------
$(SRC).hex : $(SRC).out 
	$(OBJCOPY) -R .eeprom -O ihex $(SRC).out $(SRC).hex 

$(SRC).out : $(OBJ)
	$(CC) $(CFLAGS) -o $(SRC).out -Wl,-Map,$(SRC).map $(OBJ)

%.o : %.c 
	$(CC) $(CFLAGS) -Os -c $< -o $@

%.o : %.cpp
	$(CPP) $(CPPFLAGS) -Os -c $< -o $@
#------------------
load: $(SRC).hex
	$(AVRDUDE) -p $(PART) $(PROGRAMMER) -e -U flash:w:"$(SRC).hex"







#-------------------
# fuse byte settings attiny13:
#   
#  Fuse Low Byte       = 0x69 (4,8MHz internal), 0x6a (9.6MHz internal)
#  Fuse High Byte      = 0xff (RESET enabled),  0xfe (PB5 enabled, RESET disabled)
#  Factory default is 0x6a / 0xff
#  Check this with make rdfuses

rdfuses:
	$(AVRDUDE) -p $(PART) $(PROGRAMMER) -v -q	 


# use internal RC oscillator 4.8 MHz
wrfuse4.8mhz:

		$(AVRDUDE) -p $(PART) $(PROGRAMMER) -u -v -U lfuse:w:0x69:m
		$(AVRDUDE) -p $(PART) $(PROGRAMMER) -u -v -U hfuse:w:0xff:m

# use internal RC oscillator 9.6 MHz
wrfuse9.6mhz:
		$(AVRDUDE) -p $(PART) $(PROGRAMMER) -u -v -U lfuse:w:0x6a:m
		$(AVRDUDE) -p $(PART) $(PROGRAMMER) -u -v -U hfuse:w:0xff:m

# use external crystal
wrfusecrystal:
	clear
	@echo "Warnung: Das Setzen des Quarz-Fusebits kann nur mit Quarz rueckgaengig gemacht werden!"
	@echo "         Du hast 15 Sekunden, um mit crtl-c abzubrechen."
	@echo
	@echo "Warning: The external crystal setting can not be changed back without a working crystal"
	@echo "         You have 15 seconds to abort this with crtl-c"
	@sleep 15
	$(AVRDUDE) -p $(PART) $(PROGRAMMER) -u -v -U lfuse:w:0x68:m
	$(AVRDUDE) -p $(PART) $(PROGRAMMER) -u -v -U hfuse:w:0xff:m

# fuse byte setting for using PB5 (disables RESET) 
wrfusenoreset:
	clear
	@echo "Warnung:"
	@echo	
	@echo "Das Setzen des Reset-Fusebits kann nicht rueckgaengig gemacht werden!"
	@echo "Du hast 15 Sekunden, um mit crtl-c abzubrechen."
	@echo
	@sleep 15
	$(AVRDUDE) -p $(PART) $(PROGRAMMER) -u -v -U hfuse:w:0xfe:m

#-------------------
clean:
	rm -f *.o *.map *.out *.hex

#-------------------
