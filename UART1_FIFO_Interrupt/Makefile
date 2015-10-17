#######################
# SDCC Makefile for making a hexfile from all .C files in this directory.
# Hexfile location is '.', other output files are generated in directory 'output'.
#######################

#DEVICE=STM8S105
DEVICE=STM8S208

# serial port for flasher (RasPi via GPIO and USB, MacOSX via USB and Win via USB)
#PORT=/dev/ttyAMA0
#PORT=/dev/ttyUSB0
PORT=/dev/tty.usbserial-A4009I0O
#PORT=COM20


# define compiler path (if not in PATH), and flags
CC       = sdcc
CFLAGS   = -mstm8 --std-sdcc99 -D$(DEVICE) -DUART1_FIFO
LFLAGS   = -mstm8 -lstm8 $(OPTIMIZE) --out-fmt-ihx

# set target
OBJDIR   = $(DEVICE)
TARGET   = $(OBJDIR)/main.ihx

# set project options (all .c files in folder)
PRJ_ROOT = .
PRJ_INC_DIR = $(PRJ_ROOT)
PRJ_SRC_DIR = $(PRJ_ROOT)
PRJ_SOURCE  = $(wildcard *.c)
PRJ_HEADER  = $(wildcard *.h)
PRJ_OBJECT  = $(addprefix $(OBJDIR)/, $(PRJ_SOURCE:.c=.rel))

# set STM8_Lib options
LIB_ROOT    = ../STM8_Lib
LIB_INC_DIR = $(LIB_ROOT)
LIB_SRC_DIR = $(LIB_ROOT)
LIB_SOURCE  = gpio.c timer4.c uart1.c putchar.c 
LIB_HEADER  = $(patsubst %.c,%.h,$(LIB_SOURCE))
LIB_OBJECT := $(addprefix $(OBJDIR)/, $(LIB_SOURCE:.c=.rel))

# concat all include folders
INCLUDE = -I$(PRJ_INC_DIR) -I$(LIB_INC_DIR)

# concat all source directories
VPATH=$(PRJ_SRC_DIR):$(LIB_SRC_DIR)


.PHONY: clean all default objects

.PRECIOUS: $(TARGET) $(PRJ_OBJECT) $(LIB_OBJECT) 

default: $(OBJDIR) $(TARGET)

all: default

# create output folder
$(OBJDIR):
	mkdir -p $(OBJDIR)

# compile all *c files
$(OBJDIR)/%.rel: %.c $(PRJ_HEADER) $(LIB_HEADER) $(LIB_SOURCE) $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# link all object files and libaries
$(TARGET): $(PRJ_OBJECT) $(LIB_OBJECT)
	$(CC) $(LFLAGS) $(PRJ_OBJECT) $(LIB_OBJECT) -o $@
	
# clean up
clean:
	rm -fr .DS_Store
	rm -fr -- -p
	rm -fr $(OBJDIR)
	rm -fr *.TMP
	rm -fr STVD_Cosmic/Release
	rm -fr STVD_Cosmic/Debug
	rm -fr STVD_Cosmic/*.TMP
	rm -fr STVD_Cosmic/*.spy
	rm -fr STVD_Cosmic/*.dep
	rm -fr STVD_Cosmic/*.pdb
	rm -fr STVD_Cosmic/*.wdb
	rm -fr STVD_Cosmic/*.wed



# upload via stm8flash / SWIM (https://github.com/vdudouyt/stm8flash)
# stm8flash requires device in lower-case -> http://gnu-make.2324884.n4.nabble.com/how-to-achieve-conversion-to-uppercase-td14496.html 
swim:
	stm8flash -c stlink -w $(TARGET) -p $(shell echo $(DEVICE) | tr A-Z a-z) 

# upload via STM8 bootloader (https://github.com/gicking/STM8_serial_flasher)
# select path to loader and port name
serial:
ifneq (,$(findstring /dev/ttyAMA,$(PORT)))          # RasPi via GPIO
	/home/pi/STM8_serial_flasher/STM8_serial_flasher -p $(PORT) -f $(TARGET) -q -Q -r -u 2
endif
ifneq (,$(findstring /dev/ttyUSB,$(PORT)))          # RasPi via USB
	/home/pi/STM8_serial_flasher/STM8_serial_flasher -p $(PORT) -f $(TARGET) -q
endif
ifneq (,$(findstring /dev/tty.usbserial,$(PORT)))   # Macintosh via USB
	~/Public/GitHub/STM8_serial_flasher/STM8_serial_flasher -p $(PORT) -f $(TARGET) -q
endif
ifneq (,$(findstring COM,$(PORT)))                  # Windows via USB / RS232
	C:\Users\ick2bue\Documents\STM8_serial_flasher\STM8_serial_flasher.exe -p $(PORT) -f $(TARGET) -q -Q
endif	
