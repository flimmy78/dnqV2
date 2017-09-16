#
# Copyright (C) ychen
# Copyright (C) Jiuzhou, Inc.
# 

INCLUDES = -I. -I$(TOP_DIR)/include
CFLAGS =
LDFLAGS = -lm -lrabbitmq -lpthread
LDPATH = -L$(TOP_DIR)/lib
LIBS =•

PUB_DIR=$(TOP_DIR)/pub

APP_DIR=$(PUB_DIR)/app
ROOTFS_DIR=$(PUB_DIR)/rootfs
IMAGE_DIR=$(PUB_DIR)/image

HWVER=0x100
SWVER=0x100

VERSION = 1.5.1


