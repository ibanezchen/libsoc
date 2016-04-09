SOC_INC=-I$(PREFIX)/include/linkit

SOC_ALL=flash_rw.elf gpio_led.elf net.elf net.elf.bin
SOC_LIB=-lhal -llwip -lbsp -lservice -lbsp_sec -lnvram -lservice -ldhcpd -lminisupp -lsoc-linkit bin/linkit/system_mt7687.o

.PHONY:hello.rom.elf
hello.rom.elf:
	$(MAKE) clean
	LSCRIPT=rom.ld $(MAKE) hello.elf
	cp hello.elf $@

.PHONY:hello.rom
hello.rom:hello.rom.elf
	LSCRIPT=rom.ld $(MAKE) hello.elf.bin
	cp hello.elf.bin $@

.PHONY:net.rom.elf
net.rom.elf:
	$(MAKE) clean
	LSCRIPT=rom.ld $(MAKE) net.elf
	cp net.elf $@

.PHONY:net.rom
net.rom:net.rom.elf
	LSCRIPT=rom.ld $(MAKE) net.elf.bin
	cp net.elf.bin $@
