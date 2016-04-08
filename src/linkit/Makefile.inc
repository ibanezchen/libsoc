PLT_INC=-I/home/ibanez/ws-os/gva/driver/board/mt76x7_hdk/inc	\
	-I/home/ibanez/ws-os/gva/middleware/third_party/lwip/src/include/	\
	-I/home/ibanez/ws-os/gva/middleware/third_party/lwip/ports/include/	\
	-I/home/ibanez/ws-os/gva/middleware/MTK/nvdm/inc/	\
	-I/home/ibanez/ws-os/gva/middleware/MTK/nvram/inc/ \
	-I/home/ibanez/ws-os/gva/driver/chip/inc/ \
	-I/home/ibanez/ws-os/gva/driver/chip/mt7687/inc/	\
	-I/home/ibanez/ws-os/gva/driver/CMSIS/Device/MTK/mt7687/Include/ \
	-I/home/ibanez/ws-os/gva/driver/CMSIS/Include/	\
	-I/home/ibanez/ws-os/gva/driver/chip/mt7687/inc \
	-I/home/ibanez/ws-os/gva/kernel/service/inc
ifeq ($(HAL),1)
	PLT_ALL=flash_rw.elf gpio_led.elf net.elf net.elf.bin
	PLT_LIB=-lhal -llwip -lbsp -lservice -lbsp_sec -lnvram -lservice -ldhcpd -lminisupp
endif

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
