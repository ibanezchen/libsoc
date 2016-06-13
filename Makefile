PREFIX ?= $(shell pwd)/../prefix/$(CROSS:%-=%)
SOC:=linkit
NAME   :=soc-$(SOC)
TARGET :=arm-none-eabi
CROSS  :=$(TARGET)-
CPU    :=arm
include src/$(SOC)/Makefile.inc

INCLUDE:=-Iinclude -I$(PREFIX)/include -Isrc -Isrc/$(SOC) $(SOC_INC)
COPTS  ?=-march=armv7-m -mthumb -fsingle-precision-constant -Wdouble-promotion -mfpu=fpv4-sp-d16 -mfloat-abi=hard
AARCH  :=7
MOPTS  :=$(COPTS) \
	-DCFG_AARCH=$(AARCH) \
	-fno-builtin -fno-common \
	-ffunction-sections -fdata-sections -fshort-enums
BAUD?=230400
CONFIG :=-DHZ=128 -DHC=1 -DSOC_SBRK_MAX=0x20040000 \
	-DWIFI_SSID=$(WIFI_SSID) -DWIFI_PASSWD=$(WIFI_PASSWD)
ASFLAGS:=$(MOPTS) $(CONFIG) -O2 -g -Wall -Werror -D __ASSEMBLY__
CFLAGS :=$(MOPTS) $(CONFIG) -O2 -g -Wall -Werror
LSCRIPT?=rom.ld
LDFLAGS:=$(MOPTS) -g -nostartfiles -nodefaultlibs -L . -L$(PREFIX)/lib -Tbin/$(SOC)/$(LSCRIPT)
LDFLAGS+= -Wl,--start-group -lhcos -lc -lgcc $(SOC_LIB) -Wl,--end-group -Wl,--gc-sections

MSCRIPT:=$(PREFIX)/share/mod.ld
LIB    :=lib$(NAME).a

ALL    :=include/soc-mcfg.h \
	$(PREFIX)/bin/$(SOC) $(SOC_ALL)
CLEAN  :=
CPU    :=arm

VPATH  :=src src/$(SOC)
VOBJ   :=$(patsubst %.S,%.o, \
		$(patsubst %.c,%.o, \
		$(patsubst %.cpp, %.o, \
			$(notdir $(foreach DIR,$(VPATH),\
				$(wildcard $(DIR)/*.S)	\
				$(wildcard $(DIR)/*.c) 	\
				$(wildcard $(DIR)/*.cpp))))))
default:all

include $(PREFIX)/share/Makefile.rule

.PHONY:$(PREFIX)/bin/$(SOC)
$(PREFIX)/bin/$(SOC):
	rm -rf $@
	cp -r bin/$(SOC) $@

.PHONY:include/soc-cfg.h
include/soc-mcfg.h:
	$(PREFIX)/bin/m-cfg.sh $(CONFIG) > $@

F?=hello.elf

openocd.gdb:
	echo target remote 127.0.0.1:3333 > $@
	echo monitor reset init>> $@

ddd:openocd.gdb $(F)
	ddd --debugger $(CROSS)gdb -x openocd.gdb $(F)
	
ddd-attach:
	echo "target remote 127.0.0.1:3333" > attach.gdb
	ddd --debugger $(CROSS)gdb -x attach.gdb $(F)

brd-dbg:
	openocd -f bin/$(SOC)/cmsis.cfg -s bin
	
brd-console:
	echo "pu port             /dev/ttyACM0" >~/.minirc.cdc
	echo "pu lock             /var/lock" >>~/.minirc.cdc
	echo "pu baudrate         $(BAUD)" >>~/.minirc.cdc
	echo "pu bits             8" >>~/.minirc.cdc
	echo "pu parity           N" >>~/.minirc.cdc
	echo "pu stopbits         1" >>~/.minirc.cdc
	minicom cdc

