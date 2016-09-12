SOC_INC:=-Iinclude/stm32f4 -I$(PREFIX)/include/stm32f4
SOC_LIB:=-lsoc-stm32f4
SOC_CFG:=-DUSE_STDPERIPH_DRIVER -DSTM32F40_41xxx -DBAUD=230400
SOC_ALL:=bin/$(SOC)/startup.o hello.elf

bin/$(SOC)/startup.o:startup.o
	mkdir -p bin/$(SOC)
	cp $^ $@

brd-dbg:
	openocd -f /usr/share/openocd/scripts/board/stm32f4discovery.cfg

brd-console:
	echo "pu port             /dev/ttyUSB0" >~/.minirc.usb0
	echo "pu lock             /var/lock" >>~/.minirc.usb0
	echo "pu baudrate         $(BAUD)" >>~/.minirc.usb0
	echo "pu bits             8" >>~/.minirc.usb0
	echo "pu parity           N" >>~/.minirc.usb0
	echo "pu stopbits         1" >>~/.minirc.usb0
	minicom usb0

st-flash:
	st-flash write $(F) 0x8008000
