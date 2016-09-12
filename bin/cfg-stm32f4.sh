#!/bin/bash

D=$BASH_SOURCE
D=${D%/*}
export PREFIX=${D%/*}
if ! [[ $PREFIX == /* ]]; then
	PREFIX=`pwd`/$PREFIX
fi
function cfg_stm32f4
{
	echo PREFIX=$PREFIX
	source ${PREFIX}/bin/cfg-lib.sh || return
	cfg_v7mTF_core|| return
	export SOC_INC="-I${PREFIX}/include/stm32f4 -I${PREFIX}/include/soc-stm32f4"
	echo SOC_INC=$SOC_INC
	export SOC_LIB="-lsoc-stm32f4"
	echo SOC_LIB=$SOC_LIB
	export SOC="stm32f4"
	echo SOC=$SOC
}
