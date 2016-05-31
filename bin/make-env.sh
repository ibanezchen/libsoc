#!/bin/bash

if [ -z "$SOC" ]; then
	echo "set SOC variable, e.g. linkit, ameba"
	return
fi
PREFIX=${PREFIX:-../prefix/arm-none-eabi}
source $PREFIX/bin/cfg-${SOC}.sh
eval cfg_${SOC}

MOPTS="${COPTS} \
	-fno-builtin -fno-common \
	-ffunction-sections -fdata-sections -fshort-enums"
ASFLAGS="${MOPTS} -O2 -g -Wall -Werror -D __ASSEMBLY__"
CFLAGS="${MOPTS} -O2 -g -Wall -Werror"
LSCRIPT="ram.ld"
LDFLAGS="${MOPTS} -g -nostartfiles -nodefaultlibs -L${PREFIX}/lib -L${PREFIX}/bin/${SOC} -T${PREFIX}/bin/${SOC}/${LSCRIPT}"
LDFLAGS="${LDFLAGS} -Wl,--start-group -lhcos -lc -lgcc ${SOC_LIB} -Wl,--end-group"

TARGET=arm-none-eabi
CC=${TARGET}gcc
CPP=${TARGET}cpp
CXX=${TARGET}g++
LD=${TARGET}ld
AR=${TARGET}ar
