# -*- Makefile -*-

#----------------------------------------------------------------------------
#  $Id$
#----------------------------------------------------------------------------

# Only include once.
ifndef ALL_IN_ONE_GNU
ALL_IN_ONE_GNU = 1

# Include wrapper macros to all user defines, etc.
include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

ifneq ($(ADDITIONAL_INCLUDE),)
  include $(ADDITIONAL_INCLUDE)
endif

# Now test for installed components, and set the BIN. LIB, and SHLIB
# targets from the *_UNCHECKED ones if the component checks 
# succeed.  Client Makefile must set the variable REQUIRED_COMPONENTS
# with a list of required components.  If no additional components
# are required, REQUIRED_COMPONENTS need not be set.
include $(ACE_ROOT)/include/makeinclude/component_check.GNU

# If no required components are missing, set the real targets
ifeq ($(MISSING_COMPONENTS),)
  ifdef BIN_UNCHECKED
    ifndef BIN
      BIN = $(BIN_UNCHECKED)
    endif # !BIN
  endif # !BIN_UNCHECKED
  ifdef static_libs
    ifdef LIB_UNCHECKED
      ifndef LIB
        LIB = $(LIB_UNCHECKED)
      endif # !LIB
    endif # !LIB_UNCHECKED
  endif # static_libs
  ifdef shared_libs
    ifdef SHLIB_UNCHECKED
      ifndef SHLIB
        SHLIB = $(SHLIB_UNCHECKED)
      endif # !SHLIB
    endif # !SHLIB_UNCHECKED
  endif # !shared_libs
endif # COMPILE_COMPONENT

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU

ifdef DIRS
  include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
else # !DIRS
  include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
endif # DIRS
ifdef LIB
  include_lib = 1
endif # LIB

ifdef SHLIB
  include_lib = 1
endif # SHLIB

ifeq ($(include_lib),1)
  include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
endif

ifdef BIN
  include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
endif # BIN

include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

# Remove the -g option if requested.
ifndef SUPPRESS_DASH_G
  SUPPRESS_DASH_G ?=
endif
ifeq ($(SUPPRESS_DASH_G),1)
  DCFLAGS := $(filter-out -g, $DCFLAGS)
endif

endif # ALL_IN_ONE_GNU
