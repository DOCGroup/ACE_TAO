# $Id$

include $(ACE_ROOT)/include/makeinclude/platform_macosx_leopard.GNU

CFLAGS := $(subst -Wno-long-double,,$(CFLAGS))
