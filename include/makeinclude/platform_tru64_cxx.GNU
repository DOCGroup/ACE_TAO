# $Id$

# This platform macros file is intended to work with
# Digital UNIX using the cxx compiler.

# Include the common macros for all compilers
#
include $(ACE_ROOT)/include/makeinclude/platform_tru64_pre.GNU


# Include the macro file for osf1_4.x
#
include $(ACE_ROOT)/include/makeinclude/platform_osf1_4.x_cxx.GNU


# Flags specific to 5.x
#
ifeq (5.,$(findstring 5.,$(ACE_DU_VERSION)))
    SOFLAGS = -shared -use_ld_input $(ACELIB)
endif # ! 5.x


# Include the common macros for all compilers
#
include $(ACE_ROOT)/include/makeinclude/platform_tru64_post.GNU
