# $Id$

# This platform macros file is intended to work with
# Digital UNIX using the cxx compiler.

ifndef ACE_DU_VERSION
  ACE_DU_VERSION := $(shell uname -r)
endif # ACE_DU_VERSION

ifeq (3.,$(findstring 3.,$(ACE_DU_VERSION)))
  include $(ACE_ROOT)/include/makeinclude/platform_osf1_3.2.GNU
else # ! 3.x
ifeq (4.,$(findstring 4.,$(ACE_DU_VERSION)))
  include $(ACE_ROOT)/include/makeinclude/platform_osf1_4.0.GNU

  CFLAGS += -DDIGITAL_UNIX=4
  ifneq ($(CXX_VER),CXX_5)
    # 9: nested comment not allowed.  But there's one in /usr/include/pdsc.h!
    WARNING_FLAGS += -msg_disable 9
  endif # 6.0 or later
else # ! 4.x
ifeq (5.,$(findstring 5.,$(ACE_DU_VERSION)))
  include $(ACE_ROOT)/include/makeinclude/platform_osf1_4.0.GNU

  CFLAGS += -DDIGITAL_UNIX=5

  ifeq (6.,$(findstring 6.,$(CXX_VERSION)))
    #### variable "PTHREAD_THIS_CATCH_NP" was declared but never referenced
    WARNING_FLAGS += -msg_disable 177
  endif # 6.0 or later

  SOFLAGS = -shared -use_ld_input $(ACELIB)
endif # 5.x
endif # ! 4.x
endif # ! 3.x
