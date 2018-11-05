include $(ROOTDIR)/setting.mk

dirs:=$(shell find . -maxdepth 1 -type d)
dirs:=$(basename $(patsubst ./%,%,$(dirs)))
dirs:=$(filter-out $(code_dirs),$(dirs))
SUBDIRS := $(dirs)

CUR_SOURCE=${wildcard *.c}
CUR_OBJS=${patsubst %.c, %.o, $(CUR_SOURCE)}

ALLOBJS=$(ROOTDIR)/output/objs/src/*.o
CODEOBJS = $(OUTPUTOBJSDIR)/*.o \

all:$(SUBDIRS) 
	$(CC) $(CODEOBJS) -o $(OUTPUTBINSDIR)/$(TARGET) $(LIFLAGS) $(LSFLAGS) $(LDFLAGS)

#先执行其它目录下的makefile
$(SUBDIRS):ECHO
	make -C $@

#最后执行main下的主函数
#MAIN:ECHO
#	make -C main
#依赖ECHO进行目录打印
ECHO:
	@echo "make $(SUBDIRS)"

#当前目录的依赖，所有的子目录下的Makefile都是这么处理的,输出到OUtput目录
#$(CUR_OBJS):%.o:%.c
#	$(CC) $(CFLAGS) $^ -o $(OUTPUTOBJSDIR)/$@ $(LDFLAGS) $(LSFLAGS) 

.PHONY:clean
clean:
	rm -rf $(OUTPUTOBJSDIR)/*.o
