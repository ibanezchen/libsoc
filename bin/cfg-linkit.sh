#!/bin/bash

D=$BASH_SOURCE
D=${D%/*}
export PREFIX=${D%/*}
if ! [[ $PREFIX == /* ]]; then
	PREFIX=`pwd`/$PREFIX
fi
function cfg_linkit
{
	echo PREFIX=$PREFIX
	source ${PREFIX}/bin/cfg-lib.sh || return
	cfg_v7mTF_core|| return
	export SOC_INC="-I${PREFIX}/include/linkit -I${PREFIX}/include/soc-linkit"
	echo SOC_INC=$SOC_INC
	export SOC_LIB="${PREFIX}/bin/linkit/system_mt7687.o -lhal -llwip -lbsp -lservice -lbsp_sec -lnvram -lservice -ldhcpd -lminisupp -lsoc-linkit"
	echo SOC_LIB=$SOC_LIB
	export SOC="linkit"
	echo SOC=$SOC
}
