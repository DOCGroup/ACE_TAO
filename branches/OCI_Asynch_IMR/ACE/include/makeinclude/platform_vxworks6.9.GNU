# $Id$
# VxWorks 6.9

ifeq ($(VXWORKS_VERSION_FLAG),)
  VXWORKS_VERSION_FLAG = -DACE_VXWORKS=0x690
endif # VXWORKS_VERSION_FLAG

WIND_USR_LIB = ../lib$(MAJOR_CLI_OPTION_SUFFIX)/usr/lib

VXBUILD += $(if $(findstring 64,$(buildbits)), LP64)

include $(ACE_ROOT)/include/makeinclude/platform_vxworks6.8.GNU

# override. we do not want -Wsystem-header (new in vx6.9)
CC_WARNINGS_ALL = -Wall
