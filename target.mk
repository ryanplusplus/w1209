TARGET := w1209
BUILD_DIR := ./build/target
TOOLS_DIR := tools
STM8_TINY := lib/stm8-tiny
TINY := $(STM8_TINY)/lib/tiny

DEVICE := stm8s003f3
DEVICE_TYPE := STM8S003
STLINK := stlinkv2
OPENOCD_CFG := $(TOOLS_DIR)/openocd/stm8s103.cfg
TOOLCHAIN_VERSION := 4.0.0

MAIN := src/main.c

SRC_FILES := \

SRC_DIRS := \

LIB_FILES := \

LIB_DIRS := \
  $(TINY)/src \
  $(STM8_TINY)/src \

INC_DIRS := \
	$(TINY)/include \

include $(TOOLS_DIR)/makefile-worker.mk
