#Makefile的设置
include $(ROOTDIR)/setting.mk

dirs:=$(shell find . -maxdepth 1 -type d)
dirs:=$(basename $(patsubst ./%,%,$(dirs)))
SUBDIRS := $(dirs)


CUR_SOURCE=${wildcard *.c}
CUR_OBJS=${patsubst %.c, %.o, $(CUR_SOURCE)}
all:$(SUBDIRS) $(CUR_OBJS)

$(SUBDIRS):ECHO
	make -C $@
$(CUR_OBJS):%.o:%.c
	$(CC) $(CFLAGS) $^ -o $(OUTPUTOBJSDIR)/$@ $(LDFLAGS) $(LSFLAGS)
ECHO:
	@echo $(SUBDIRS)
