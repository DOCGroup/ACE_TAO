ssl?=0

include $(ACE_ROOT)/include/makeinclude/platform_macosx_yosemite.GNU

## The following is to circumvent the restriction of System Integrity Protection (SIP) on Mac OS X El Capitan 
## by embedding the path information of dynamic libraries into the executables.

SOFLAGS	+= -install_name @rpath/$@
space := $(subst ,, )

LDFLAGS += $(foreach libpath,$(LIBPATHS),-rpath $(if $(filter "/%,$(subst $(space),;,$(libpath))),$(libpath),@executable_path/$(libpath)))


