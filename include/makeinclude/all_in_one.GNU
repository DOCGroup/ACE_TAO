# -*- Makefile -*-

#----------------------------------------------------------------------------
#  $Id$
#----------------------------------------------------------------------------

# Only include once.
ifndef ALL_IN_ONE_GNU
ALL_IN_ONE_GNU = 1

# Include wrapper macros to all user defines, etc.
include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

# Now test for installed components, and set the BIN. LIB, and SHLIB
# targets from the *_UNCHECKED ones if the component checks 
# succeed.  Client Makefile must set the variable REQUIRED_COMPONENTS
# with a list of required components.  If no additional components
# are required, REQUIRED_COMPONENTS need not be set.
include $(ACE_ROOT)/include/makeinclude/component_check.GNU

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU

# The CLEAN_* targets are defined in wrapper_macros.GNU, so use them
# to determine if we should include rules.bin.GNE or rules.lib.GNU.

ifdef CLEANUP_BIN
  include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
endif # CLEANUP_BIN

ifdef CLEANUP_LIB
  include_lib = 1
endif # LIB

ifdef CLEANUP_SHLIB
  include_lib = 1
  # If the Makefile has both BIN and SHLIB targets, add .shobj to OBJDIRS
  # so that it will get cleanup too.
  ifneq ($(findstring $(VSHDIR),$(OBJDIRS)),$(VSHDIR))
    OBJDIRS += $(VSHDIR)
  endif
endif # SHLIB

ifeq ($(include_lib),1)
  include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
endif

# Always add VBIN, it could be undefined, but that's not problem.
BUILD := $(VBIN)

# Add VSHLIB if we're building shared_libs, could be undefined.
ifdef shared_libs
  BUILD := $(BUILD) $(VSHLIB)
endif

# Finally, add VLIB if building static_libs, which could also be undefined.
ifdef static_libs
  BUILD := $(BUILD) $(VLIB)
endif

include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

# Remove the -g option if requested.
ifeq ($(SUPPRESS_DASH_G),1)
  DCFLAGS := $(filter-out -g, $DCFLAGS)
endif

endif # ALL_IN_ONE_GNU
