PREFIX ?= $(shell pwd)/../prefix/$(CROSS:%-=%)
PLT:=linkit
NAME   :=soc-$(PLT)
TARGET :=arm-none-eabi
CROSS  :=$(TARGET)-
CPU    :=arm
HAL?=1
include src/$(PLT)/Makefile.inc

INCLUDE:=-Iinclude -I$(PREFIX)/include -Isrc -Isrc/$(PLT) $(PLT_INC)

COPTS  ?=-march=armv7-m -mthumb 
AARCH  :=7
MOPTS  :=$(COPTS) \
	-DCFG_AARCH=$(AARCH) \
	-fsingle-precision-constant -Wdouble-promotion -mfpu=fpv4-sp-d16 -mfloat-abi=hard \
	-fno-builtin -fno-common \
	-ffunction-sections -fdata-sections
BAUD?=230400
CONFIG :=-DHAL=$(HAL) -DHZ=100 -DHC=1
ASFLAGS:=$(MOPTS) $(CONFIG) -O1 -g -Wall -Werror -D __ASSEMBLY__
CFLAGS :=$(MOPTS) $(CONFIG) -O1 -g -Wall -Werror -fno-builtin
LSCRIPT?=rom.ld
LDFLAGS:=$(MOPTS) -g -nostartfiles -nodefaultlibs -L$(PREFIX)/lib -L . -Tsrc/$(PLT)/$(LSCRIPT) dbg.o \
	-L/home/ibanez/ws-os/gva/project/mt7687_hdk/apps/lwip_socket/GCC/Build #LINKIT

MSCRIPT:=$(PREFIX)/share/mod.ld
LIB    :=lib$(NAME).a

ALL    :=startup.o hello.elf $(PLT_ALL)
LDFLAGS+= -Wl,--start-group -lhcos -lc -lgcc $(PLT_LIB) -Wl,--end-group
CLEAN  :=
CPU    :=arm

VPATH  :=src src/$(PLT)
VOBJ   :=$(patsubst %.S,%.o, \
		$(patsubst %.c,%.o, \
		$(patsubst %.cpp, %.o, \
			$(notdir $(foreach DIR,$(VPATH),\
				$(wildcard $(DIR)/*.S)	\
				$(wildcard $(DIR)/*.c) 	\
				$(wildcard $(DIR)/*.cpp))))))
default:all

include $(PREFIX)/share/Makefile.rule

F?=hello.elf

ddd:
	ddd --debugger $(CROSS)gdb -x openocd.gdb $(F)
	
gdb:
	$(CROSS)gdb -x openocd.gdb $(F)

brd-dbg:
	openocd -f bin/$(PLT)/cmsis.cfg -s bin
	
brd-console:
	echo "pu port             /dev/ttyACM0" >~/.minirc.cdc
	echo "pu lock             /var/lock" >>~/.minirc.cdc
	echo "pu baudrate         $(BAUD)" >>~/.minirc.cdc
	echo "pu bits             8" >>~/.minirc.cdc
	echo "pu parity           N" >>~/.minirc.cdc
	echo "pu stopbits         1" >>~/.minirc.cdc
	minicom cdc
