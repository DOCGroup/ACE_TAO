# $Id$

# This platform macros file is intended to work with
# Digital UNIX using the cxx compiler.

# Include the common macros for all compilers
#
include $(ACE_ROOT)/include/makeinclude/platform_tru64_pre.GNU


# Include the macro file for osf1_4.x
#
include $(ACE_ROOT)/include/makeinclude/platform_osf1_4.x_cxx.GNU


### There is a bug with pthreads header file shipped with v1885 of
### V5.1. We need to turn this flag to get asm () inline for
### compilation.
ifeq (1885,$(findstring 1885,$(ACE_DU_SUBVERSION)))
  ifeq (6.,$(findstring 6.,$(CXX_VERSION)))
    CPPFLAGS += -D_PTHREAD_ENV_DECCXX
  endif # 6.x
endif #v1885

# Include the common macros for all compilers
#
include $(ACE_ROOT)/include/makeinclude/platform_tru64_post.GNU
