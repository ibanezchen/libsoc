SOC_INC=-I$(PREFIX)/include/linkit

SOC_ALL=flash_rw.elf gpio_led.elf net.elf net.elf.bin
SOC_LIB=-lhal -llwip -lbsp -lservice -lbsp_sec -lnvram -lservice -ldhcpd -lminisupp -lsoc-linkit bin/linkit/system_mt7687.o

utest:
	$(MAKE) clean
	$(MAKE) test.elf.pack LSCRIPT=ram.ld
	$(MAKE) ddd F=test.elf.pack

hello:
	$(MAKE) clean
	$(MAKE) hello.elf LSCRIPT=ram.ld
	$(MAKE) ddd F=hello.elf

.PHONY:net.rom
net.rom:
	$(MAKE) clean
	$(MAKE) net.elf.bin LSCRIPT=rom.ld
	mv net.elf.bin $@
