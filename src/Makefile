#
# Makefile for mucom88
#

CXX = g++
CC = gcc
CFLAGS = 
CXXFLAGS = -Wno-write-strings
LDFLAGS = 
DEFINES = 
OBJS_DIR = objs
TARGET_NAME = mucom88
TARGET = $(TARGET_NAME).exe
TARGET_MODTEST = mod_test.exe

# Win32用
OS_WIN32 = 1

ifeq ($(DEBUG),1)
TARGET = $(TARGET_NAME)_d.exe
LDFLAGS = -g
CFLAGS := $(CFLAGS) -O0 -g
CXXFLAGS := $(CXXFLAGS) -O0 -g
OBJS_DIR = objs_d
endif

LIBS = -ldsound 
LIBS += -lole32 -lwinmm -luuid 
LIBS += -ldsound -ldxguid
LIBS += -static -lstdc++
LIBS += -mconsole

.PHONY: all test $(OBJS_DIR)

all: makedir $(TARGET)
test: makedir $(TARGET_TEST) $(TARGET_MODTEST)

DIRS = z80 fmgen plugin module tests

SRCS = membuf.cpp adpcm.cpp


SRCS += md5.c
SRCS += soundbuf.cpp

SRCS += cmucom.cpp
SRCS += mucomerror.cpp
SRCS += mucomvm.cpp

SRCS += plugin/plugin.cpp

SRCS += Z80/Z80.cpp

SRCS += fmgen/file.cpp fmgen/fmgen.cpp fmgen/fmtimer.cpp 
SRCS += fmgen/opm.cpp fmgen/opna.cpp fmgen/psg.cpp

SRCS += callback.cpp
SRCS += wavout.cpp
SRCS += osdep.cpp

MAIN_SRCS = main.cpp

ifeq ($(OS_WIN32),1)
DEFINES += -DMUCOM88WIN
DIRS += win32
DEP_SRCS += win32/realchip.cpp
DEP_SRCS += win32/soundds.cpp
DEP_SRCS += win32/osdep_win.cpp
else 
DIRS += dummy
DEP_SRCS += dummy/osdep_dummy.cpp
endif

MOD_SRCS = module/mucom_module.cpp
MODTEST_SRCS = tests/module_test.cpp

# OS依存
DEP_SRCS_BASE = $(basename $(DEP_SRCS))
DEP_OBJS = $(addprefix $(OBJS_DIR)/,$(addsuffix .o,$(DEP_SRCS_BASE)))

# OS依存から参照される依存しないコード
REF_SRCS = soundbuf.cpp
REF_SRCS_BASE = $(basename $(REF_SRCS))
REF_OBJS = $(addprefix $(OBJS_DIR)/,$(addsuffix .o,$(REF_SRCS_BASE)))

# コマンドライン基本
MAIN_SRCS_BASE = $(basename $(MAIN_SRCS))
MAIN_OBJS = $(addprefix $(OBJS_DIR)/,$(addsuffix .o,$(MAIN_SRCS_BASE)))

# モジュール化
MOD_SRCS_BASE = $(basename $(MOD_SRCS))
MOD_OBJS = $(addprefix $(OBJS_DIR)/,$(addsuffix .o,$(MOD_SRCS_BASE)))

# モジュールテスト
MODTEST_SRCS_BASE = $(basename $(MODTEST_SRCS))
MODTEST_OBJS = $(addprefix $(OBJS_DIR)/,$(addsuffix .o,$(MODTEST_SRCS_BASE)))

# 本体ソース
SRCS_BASE = $(basename $(SRCS))
OBJS = $(addprefix $(OBJS_DIR)/,$(addsuffix .o,$(SRCS_BASE)))
OBJS += $(DEP_OBJS) $(REF_OBJS)


INCLUDES += -I. $(addprefix -I,$(DIRS) )
CFLAGS += $(DEFINES) $(INCLUDES)
CXXFLAGS += $(DEFINES) $(INCLUDES)

$(TARGET_MODTEST) : $(OBJS) $(MOD_OBJS) $(MODTEST_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

$(TARGET) : $(MAIN_OBJS) $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

makedir : $(OBJS_DIR)
	mkdir -p $(OBJS_DIR) $(addprefix $(OBJS_DIR)/,$(DIRS))

$(OBJS_DIR)/%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJS_DIR)/%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	rm -f $(TARGET_MODTEST)
	rm -f $(TARGET)
	rm -rf $(OBJS_DIR)

