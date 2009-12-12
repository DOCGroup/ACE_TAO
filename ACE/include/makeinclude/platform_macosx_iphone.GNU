# $Id$

include $(ACE_ROOT)/include/makeinclude/platform_macosx_common.GNU

XCODE:=/Developer
IPHONE_PLATFORM:=$(XCODE)/Platforms/iPhoneSimulator.platform/Developer
IPHONE_SDK:=$(IPHONE_PLATFORM)/SDKs/iPhoneSimulator3.1.2.sdk
CC:=$(IPHONE_PLATFORM)/usr/bin/gcc
CXX:=$(IPHONE_PLATFORM)/usr/bin/g++
AR:=$(IPHONE_PLATFORM)/usr/bin/ar
CFLAGS   +=-W -Wall -arch i386 -O3 -isysroot $(IPHONE_SDK)
CPPFLAGS +=
CXXFLAGS += -W -Wall -arch i386 -O3 -isysroot $(IPHONE_SDK)
DLD = $(CC)
LD  = $(CC)

