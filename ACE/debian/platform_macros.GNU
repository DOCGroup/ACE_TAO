# $Id$

debug          = 0
optimize       = 1
inline         = 1

ssl            = 1

xt             = 1
tk             = 1
fl             = 1
fox            = 1
qt4            = 1
ace_qt4reactor = 1

bzip2          = 1
lzo1           = 1
zlib           = 1

# Work-around #593225
ARMEL_TARGET := $(shell echo '__ARMEL__' | $(CC) -E - | tail -n 1)
ifeq ($(ARMEL_TARGET),1)
  no_hidden_visibility = 1
endif

include $(ACE_ROOT)/include/makeinclude/platform_linux.GNU

PLATFORM_FOX_CPPFLAGS=-I/usr/include/fox-1.6
PLATFORM_FOX_LIBS=-lFOX-1.6
