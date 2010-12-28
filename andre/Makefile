DEVICE = atmega328p
AVRDUDE_DEVICE = m328p
DEVICE ?= atmega168
AVRDUDE_DEVICE ?= m168

CFLAGS=-g -Wall -mcall-prologues -mmcu=$(DEVICE) -Os -lpololu_atmega328p -L/usr/lib/avr/lib -I/usr/lib/avr/include
CPPFLAGS=-g -Wall -mcall-prologues -mmcu=$(DEVICE) -Os -lpololu_atmega328p -L/usr/lib/avr/lib -I/usr/lib/avr/include
CXXFLAGS=-g -Wall -mcall-prologues -mmcu=$(DEVICE) -Os -lpololu_atmega328p -L/usr/lib/avr/lib -I/usr/lib/avr/include
CC=avr-gcc
CPP=avr-g++
CXX=avr-g++

OBJ2HEX=avr-objcopy 
LDFLAGS=-Wl,-gc-sections -lpololu_$(DEVICE) -Wl,-relax

PORT = COM4
PORT ?= /dev/ttyUSB0
AVRDUDE=avrdude
OBJECT_FILES=main.o robot.o speed.o line.o crossroad.o common.o code.o
TARGET=main


all: $(TARGET).hex

clean:
	rm -f *.o *.hex *.obj *.hex

%.hex: %.obj
	$(OBJ2HEX) -R .eeprom -O ihex $< $@

%.obj: $(OBJECT_FILES)
	$(CPP) $(CFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o $@

program: $(TARGET).hex
	$(AVRDUDE) -p $(AVRDUDE_DEVICE) -c avrisp2 -P $(PORT) -U flash:w:$(TARGET).hex

