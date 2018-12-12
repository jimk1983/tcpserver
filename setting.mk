#公共部分
#CC=gcc
CC=g++
CFLAGS=-c 
MAKE=make
AR=ar cr
RM=-rm -rf
CFLAGS +=-Wall -g #-Werror -O3  	#将-Werror将告警当作错误处理


###########################
#公共属性定义
TARGET=MainServer
OUTPUTOBJSDIR=$(ROOTDIR)/output/objs
OUTPUTBINSDIR=$(ROOTDIR)/output/bin

##################################
#添加系统配置的头文件目录
CFLAGS+=-I$(ROOTDIR)
CFLAGS+=-I$(ROOTDIR)/src
CFLAGS+=-I$(ROOTDIR)/src/dataplane
CFLAGS+=-I$(ROOTDIR)/src/shareplane
CFLAGS+=-I$(ROOTDIR)/src/mgmtplane
CFLAGS+=-I$(ROOTDIR)/libs/include
CFLAGS+=-I$(ROOTDIR)/libs/include/vos
CFLAGS+=-I$(ROOTDIR)/libs/include/rct

LIFLAGS=-L$(ROOTDIR)/libs/bin -lrct -lvos  -lredisclient -lhiredis

#LSFLAGS +=

LDFLAGS = -ldl -lpthread -lrt -lm #-ldb


















