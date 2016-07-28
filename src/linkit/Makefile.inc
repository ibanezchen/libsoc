SOC_INC=-Iinclude/linkit -I$(PREFIX)/include/linkit
SOC_LIB=bin/linkit/system_mt7687.o \
	-lhal -llwip -lbsp -lservice -lbsp_sec -lnvram -lservice -ldhcpd \
	-lminisupp -lsoc-linkit

SOC_ALL=bin/$(SOC)/startup.o startup.o hello.elf flash-io.elf gpio_led.elf net.elf net.elf.bin

#utest:
#	$(MAKE) clean
#	$(MAKE) test.elf.pack LSCRIPT=ram.ld
#	$(MAKE) ddd F=test.elf.pack

#hello:
#	$(MAKE) clean
#	$(MAKE) hello.elf LSCRIPT=ram.ld
#	$(MAKE) ddd F=hello.elf

.PHONY:net.rom
net.rom:
	$(MAKE) clean
	$(MAKE) net.elf.bin LSCRIPT=rom.ld
	mv net.elf.bin $@

bin/$(SOC)/startup.o:startup.o
	cp $^ $@
