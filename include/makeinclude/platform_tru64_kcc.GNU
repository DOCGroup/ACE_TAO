# $Id$

# This platform macros file is intended to work with
# Digital UNIX using the KAI compiler.

# Include the common macros for all compilers
#
include $(ACE_ROOT)/include/makeinclude/platform_tru64_pre.GNU

# Include the macro file for osf1_4.x
#
include $(ACE_ROOT)/include/makeinclude/platform_osf1_4.x_kcc.GNU

ifeq (4.,$(findstring 4.,$(ACE_DU_VERSION)))
  ifeq (4.0F,$(findstring 4.0F,$(ACE_DU_SUBVERSION)))
    # variable "" was declared but never referenced.
    # Get these from PTHREAD_THIS_CATCH_NP.
    WARNING_FLAGS += --diag_suppress 177
  endif # ! 4.0F

else # ! 4.x
ifeq (5.,$(findstring 5.,$(ACE_DU_VERSION)))
  # variable "" was declared but never referenced.
  # Get these from PTHREAD_THIS_CATCH_NP.
  WARNING_FLAGS += --diag_suppress 177

  # KCC makes a lot of these warnings from the C compiler.
  WARNING_FLAGS += --backend -msg_disable --backend nonstandcast,missingreturn,uninit1,ptrmismatch

endif # 5.x
endif # ! 4.x

# Include the common macros for all compilers
#
include $(ACE_ROOT)/include/makeinclude/platform_tru64_post.GNU
