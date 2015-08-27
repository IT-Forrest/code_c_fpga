#
TARGET = my_app

#
OS_NAME = $(shell uname -o)
LC_OS_NAME = $(shell echo $(OS_NAME) | tr '[A-Z]' '[a-z]')
ifeq ($(LC_OS_NAME), cygwin)
CROSS_COMPILE = arm-linux-gnueabihf-
CFLAGS = -static -g -Wall  -I${SOCEDS_DEST_ROOT}/ip/altera/hps/altera_hps/hwlib/include
else
CROSS_COMPILE = arm-angstrom-linux-gnueabi-
CFLAGS = -static -g -Wall  -I/home/root/mydesign/hwlib/include
endif
#
LDFLAGS =  -g -Wall  
CC = $(CROSS_COMPILE)gcc
ARCH= arm


build: $(TARGET)
$(TARGET): main.o psoc_port.o serial_port_io.o device.o set_config.o test_adc.o test_sa.o 
	$(CC) $(LDFLAGS)   $^ -o $@  
%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(TARGET) *.a *.o *~ 
