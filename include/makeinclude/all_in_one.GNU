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
  ifneq ($(CLEANUP_BIN),)
    ifeq ($(BIN),)
      BIN = $(CLEANUP_BIN)
    endif # !BIN
  endif # !CLEANUP_BIN
  ifdef static_libs
    ifneq ($(CLEANUP_LIB),)
      ifeq ($(LIB),)
        LIB = $(CLEANUP_LIB)
      endif # !LIB
    endif # !CLEANUP_LIB
  endif # static_libs
  ifdef shared_libs
    ifneq ($(CLEANUP_SHLIB),)
      ifeq ($(SHLIB),)
        SHLIB = $(CLEANUP_SHLIB)
      endif # !SHLIB
    endif # !CLEANUP_SHLIB
  endif # !shared_libs
endif # COMPILE_COMPONENT

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU

# The CLEAN_* targets are defined in wrapper_macros.GNU, so use them
# to determine if we should include rules.bin.GNE or rules.lib.GNU.

ifneq ($(CLEANUP_BIN),)
  include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
endif # CLEANUP_BIN

ifneq ($(CLEANUP_LIB),)
  include_lib = 1
endif # LIB

ifneq ($(CLEANUP_SHLIB),)
  include_lib = 1
endif # SHLIB

ifeq ($(include_lib),1)
  include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
endif

# Always build the V* targets in this order.  We control assignment to them
# elsewhere.
BUILD = $(IDL_SRC) $(VLIBS) $(VBIN)

# Add VSHLIB if we're building shared_libs, could be undefined.
ifdef shared_libs
  VLIBS = $(VSHLIB) $(SHLIBA)
endif

# Finally, add VLIB if building static_libs, which could also be undefined.
ifdef static_libs 
  VLIBS += $(VLIB)
endif

include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

# Remove the -g option if requested.
ifeq ($(SUPPRESS_DASH_G),1)
  DCFLAGS := $(filter-out -g, $DCFLAGS)
endif

endif # ALL_IN_ONE_GNU
