debug          = 0
optimize       = 1
inline         = 1

ssl            = 1

xt             = 1
tk             = 1
fl             = 1
fox            = 1

xerces3        = 1
versioned_so   = 2

include $(ACE_ROOT)/include/makeinclude/platform_linux.GNU

PLATFORM_FOX_CPPFLAGS=-I/usr/include/fox-1.6
PLATFORM_FOX_LIBS=-lFOX-1.6
