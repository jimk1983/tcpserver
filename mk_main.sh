#!/bin/bash

shopt -s expand_aliases
#获取当前的目录,注意：如果别的脚本引用该变量，那么该变量为其Makefile所在的目录
export CURDIR="$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)"

#环境变量设置好后，值不会改变，设置根目录
export ROOTDIR="$CURDIR"
export SRCCODE="$ROOTDIR/src"
export INVOSDIR="$ROOTDIR../Linuxvos"
export INRCTDIR="$ROOTDIR../Linuxtcprct"

#设置别名命令，进行编译的命令
alias mainmake="make --include-dir=$INVOSDIR --include-dir=$INRCTDIR -C $SRCCODE"

function main_compile
{
	echo $1
	
	echo "make clean"	
    	mainmake clean
		
	rm -rf $ROOTDIR/output/bin/*
		
	echo "make all"
       mainmake all
	 
}

main_compile mainmake



