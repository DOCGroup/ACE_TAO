# $Id$
# VxWorks 6.6

ifeq ($(VXWORKS_VERSION_FLAG),)
  VXWORKS_VERSION_FLAG = -DACE_VXWORKS=0x660
endif # VXWORKS_VERSION_FLAG

include $(ACE_ROOT)/include/makeinclude/platform_vxworks6.5.GNU
