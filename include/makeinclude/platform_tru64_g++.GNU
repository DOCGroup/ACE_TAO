# $Id$

# This platform macros file is intended to work with
# Digital UNIX using the GNU compiler.

# Of late, this platform and compiler combination hasn't been tested. In
# fact neither compilation nor testing has been done for almost a year.
# So it's better to avoid this combination, if possible.

ifndef ACE_DU_VERSION
  ACE_DU_VERSION := $(shell uname -r)
endif # ACE_DU_VERSION

ifeq (3.,$(findstring 3.,$(ACE_DU_VERSION)))
  include $(ACE_ROOT)/include/makeinclude/platform_osf1_3.2.GNU

else # ! 3.x
ifeq (4.,$(findstring 4.,$(ACE_DU_VERSION)))
  include $(ACE_ROOT)/include/makeinclude/platform_osf1_4.0_g++.GNU

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

else # ! 4.x
ifeq (5.,$(findstring 5.,$(ACE_DU_VERSION)))
  include $(ACE_ROOT)/include/makeinclude/platform_osf1_4.0_g++.GNU
  CFLAGS += -DDIGITAL_UNIX=0x500
endif # 5.x

endif # ! 4.x
endif # ! 3.x
