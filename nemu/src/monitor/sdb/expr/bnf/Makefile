include $(NEMU_HOME)/include/config/auto.conf
include $(NEMU_HOME)/include/config/auto.conf.cmd
# 定义 remove_quote
remove_quote = $(patsubst "%",%,$(1))
GUEST_ISA = $(call remove_quote,$(CONFIG_ISA))

isa: 
	@echo "ISA"
	@echo $(GUEST_ISA)

FILELIST_MK = $(shell find -L $(NEMU_HOME)/src/isa -name "filelist.mk")
include $(FILELIST_MK)
INC_PATH := $(NEMU_HOME)/include $(INC_PATH) 
INCLUDES = $(addprefix -I, $(INC_PATH))

inc: 
	@echo "INC"
	@echo $(INCLUDES)

DIRS-y += $(NEMU_HOME)/src/cpu
SRCS-y += $(shell find -L $(DIRS-y) -name "*.c")
SRCS=$(wildcard *.c)
SRCS += $(SRCS-y)

src: 
	@echo "srcs"
	@echo $(SRCS)


CC = $(call remove_quote,$(CONFIG_CC))
CFLAGS_BUILD += $(call remove_quote,$(CONFIG_CC_OPT))
CFLAGS_BUILD += $(if $(CONFIG_CC_LTO),-flto,)
CFLAGS_BUILD += $(if $(CONFIG_CC_DEBUG),-Og -ggdb3,)
CFLAGS_BUILD += $(if $(CONFIG_CC_ASAN),-fsanitize=address,)
CFLAGS_TRACE += -DITRACE_COND=$(if $(CONFIG_ITRACE_COND),$(call remove_quote,$(CONFIG_ITRACE_COND)),true)
CFLAGS  += $(CFLAGS_BUILD) $(CFLAGS_TRACE) -D__GUEST_ISA__=$(GUEST_ISA) $(INCLUDES)
LDFLAGS += $(CFLAGS_BUILD)

# C编译器参数：使用C11标准，生成debug信息，禁止将未初始化的全局变量放入到common段
# CFLAGS=-std=c11 -g -fno-common $(INCLUDES) -D__GUEST_ISA__=$(GUEST_ISA)
# 指定C编译器，来构建项目
# C源代码文件，表示所有的.c结尾的文件

# C文件编译生成的未链接的可重定位文件，将所有.c文件替换为同名的.o结尾的文件名
OBJS=$(SRCS:%.c=$(BUILD_DIR)/%.o)
DEPS=$(wildcard *.h)
BUILD_DIR=build

# objs: 
# 	@echo $(OBJS)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c -o $@ $<

# app 标签，表示如何构建最终的二进制文件，依赖于所有的.o文件
# $@表示目标文件，此处为 app，$^表示依赖文件，此处为$(OBJS)
$(BUILD_DIR)/app: $(OBJS)
# 将多个*.o文件编译为 app
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# 所有的可重定位文件依赖于rvcc.h的头文件
$(OBJS): $(DEPS)

# expr="0*9+5*2==4+4*(6/3)-2"
expr="$a0"

# 测试标签，运行测试脚本
test: $(BUILD_DIR)/app
	./$(BUILD_DIR)/app $(expr)


gdb: $(BUILD_DIR)/app
	gdb --args $(BUILD_DIR)/app $(expr)

# 清理标签，清理所有非源代码文件
clean:
	rm -rf $(BUILD_DIR)

# 伪目标，没有实际的依赖文件
.PHONY: test clean
