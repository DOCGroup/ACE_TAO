# $Id$

# This platform macros file is intended to work with
# Digital UNIX using the cxx compiler.

ifndef ACE_DU_VERSION
  ACE_DU_VERSION := $(shell uname -r)
endif # ! ACE_DU_VERSION

ifeq (3.,$(findstring 3.,$(ACE_DU_VERSION)))
  include $(ACE_ROOT)/include/makeinclude/platform_osf1_3.2.GNU

else # ! 3.x
ifeq (4.,$(findstring 4.,$(ACE_DU_VERSION)))
  include $(ACE_ROOT)/include/makeinclude/platform_osf1_4.0.GNU

  ifndef ACE_DU_SUBVERSION
    ACE_DU_SUBVERSION := $(shell /usr/sbin/sizer -v)
  endif # ! ACE_DU_SUBVERSION

  ifeq (4.0F,$(findstring 4.0F,$(ACE_DU_SUBVERSION)))
    CFLAGS += -DDIGITAL_UNIX=0x40F
  else
  ifeq (4.0E,$(findstring 4.0E,$(ACE_DU_SUBVERSION)))
    CFLAGS += -DDIGITAL_UNIX=0x40E
  else
  ifeq (4.0D,$(findstring 4.0D,$(ACE_DU_SUBVERSION)))
    CFLAGS += -DDIGITAL_UNIX=0x40D
  else
    CFLAGS += -DDIGITAL_UNIX=0x400
  endif
  endif
  endif

  ifneq ($(CXX_VER),CXX_5)
    # 9: nested comment not allowed.  But there's one in /usr/include/pdsc.h!
    WARNING_FLAGS += -msg_disable 9
  endif # 6.0 or later

else # ! 4.x
ifeq (5.,$(findstring 5.,$(ACE_DU_VERSION)))
  include $(ACE_ROOT)/include/makeinclude/platform_osf1_4.0.GNU

  SOFLAGS = -shared -use_ld_input $(ACELIB)

  ifndef ACE_DU_SUBVERSION
    ACE_DU_SUBVERSION := $(shell /usr/sbin/sizer -v)
  endif # ! ACE_DU_SUBVERSION

  ifeq (V5.1,$(findstring V5.1,$(ACE_DU_SUBVERSION)))
    CFLAGS += -DDIGITAL_UNIX=0x510
  else
    CFLAGS += -DDIGITAL_UNIX=0x500
  endif

endif # 5.x
endif # ! 4.x
endif # ! 3.x
