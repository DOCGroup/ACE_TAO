# $Id$

ifeq ($(buildbits),32)
  FLAGS_C_CC += -m32
  LDFLAGS    += -m32
endif
ifeq ($(buildbits),64)
  FLAGS_C_CC += -m64
  LDFLAGS    += -m64
endif
ifeq ($(buildbits),universal)
  FLAGS_C_CC += -arch i386 -arch x86_64
  LDFLAGS    += -arch i386 -arch x86_64
endif

PLATFORM_NDDS_FLAGS="-ppPath cpp-4.2 -I."

include $(ACE_ROOT)/include/makeinclude/platform_macosx_common.GNU

