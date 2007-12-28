# $Id$
# VxWorks 6.6

ifeq ($(VXWORKS_VERSION_FLAG),)
  VXWORKS_VERSION_FLAG = -DACE_VXWORKS=0x660
endif # VXWORKS_VERSION_FLAG

# Disable strict aliasing warnings, these are given on some VxWorks system
# header files and has been reported to WindRiver
ifeq ("$(TOOL_FAMILY)","gnu")
  CPPFLAGS += -fno-strict-aliasing
endif

include $(ACE_ROOT)/include/makeinclude/platform_vxworks6.5.GNU

