# $Id$

# This platform macros file is intended to work with
# Digital UNIX using the GNU compiler.

ifndef ACE_DU_VERSION
  ACE_DU_VERSION := $(shell uname -r)
endif # ACE_DU_VERSION

ifeq (3.,$(findstring 3.,$(ACE_DU_VERSION)))
  include $(ACE_ROOT)/include/makeinclude/platform_osf1_3.2.GNU

else # ! 3.x
ifeq (4.,$(findstring 4.,$(ACE_DU_VERSION)))
  include $(ACE_ROOT)/include/makeinclude/platform_osf1_4.0_g++.GNU

  ifeq (4.0F,$(findstring 4.0F,$(shell /usr/sbin/sizer -v)))
    CFLAGS += -DDIGITAL_UNIX=0x40F
  else  # ! 4.0F
    CFLAGS += -DDIGITAL_UNIX=0x400
  endif # ! 4.0F

else # ! 4.x
ifeq (5.,$(findstring 5.,$(ACE_DU_VERSION)))
  include $(ACE_ROOT)/include/makeinclude/platform_osf1_4.0_g++.GNU
  CFLAGS += -DDIGITAL_UNIX=0x500

endif # 5.x
endif # ! 4.x
endif # ! 3.x
