# $Id$

# This platform macros file is intended to work with
# Digital UNIX using the KAI compiler.

ifndef ACE_DU_VERSION
  ACE_DU_VERSION := $(shell uname -r)
endif # ACE_DU_VERSION

ifeq (3.,$(findstring 3.,$(ACE_DU_VERSION)))
  include $(ACE_ROOT)/include/makeinclude/platform_osf1_3.2.GNU

else # ! 3.x
ifeq (4.,$(findstring 4.,$(ACE_DU_VERSION)))
  include $(ACE_ROOT)/include/makeinclude/platform_osf1_4.0_kcc.GNU

  ifeq (4.0F,$(findstring 4.0F,$(shell /usr/sbin/sizer -v)))
    CFLAGS += -DDIGITAL_UNIX=0x40F
    # variable "" was declared but never referenced.
    # Get these from PTHREAD_THIS_CATCH_NP.
    WARNING_FLAGS += --diag_suppress 177
  else  # ! 4.0F
    CFLAGS += -DDIGITAL_UNIX=0x400
  endif # ! 4.0F

else # ! 4.x
ifeq (5.,$(findstring 5.,$(ACE_DU_VERSION)))
  include $(ACE_ROOT)/include/makeinclude/platform_osf1_4.0_kcc.GNU

  # variable "" was declared but never referenced.
  # Get these from PTHREAD_THIS_CATCH_NP.
  WARNING_FLAGS += --diag_suppress 177

  # KCC makes a lot of these warnings from the C compiler.
  WARNING_FLAGS += --backend -msg_disable --backend nonstandcast,missingreturn,uninit1,ptrmismatch

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
