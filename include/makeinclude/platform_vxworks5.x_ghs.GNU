# $Id$
#
# VxWorks 5.2-5.3.1 with Green Hills 1.8.8

#### Notes:
#### 1) This file requires that your PATH be set to include the location
####    of the Green Hills executables.  And, it usually requires that
####    WIND_BASE and WIND_HOST_TYPE be set.
####
#### 2) If your CPU is not a ppc, you'll need to set your CPU environment
####    variable.
####
#### 3) You'll probably need to set your GHS environment variable to the
####    location of your Green Hills executables.
####
#### 4) For x86 target, the ghsblin.o, ghsmath.o, and objghsvx/*.o
####    object files were added to libedgnoe.a using this command:
####
####    $(WIND_BASE)/host/$(WIND_HOST_TYPE)/bin/ar386 rsv libedgnoe.a \
####    ghs*.o objghsvx/*.o
####
####    If you don't do that, you'll have to add the individual .o files
####    to LIBS below.
####
#### Here's an example platform_macros.GNU file:
####
#### WIND_BASE = /project/doc/pkg/wind
#### WIND_HOST_TYPE = sun4-solaris2
####
#### ifeq (,$(findstring /project/doc/pkg/wind/green,$PATH))
####   PATH +=:/project/doc/pkg/wind/green
#### endif # PATH
####
#### CPU = 86
#### GHS = /project/doc/pkg/wind/green/vx$(CPU)
#### include $(ACE_ROOT)/include/makeinclude/platform_vxworks5.x_ghs.GNU

VXWORKS = 1
CROSS-COMPILE = 1

debug = 1
optimize = 1
shared_libs =
static_libs = 1


#### CPU type
ifndef CPU
  CPU = ppc
endif # CPU

#### Green Hills location, and target-specific definitions.
ifeq ($(CPU),86)
  # With GHS' ax, ld386 complains that it can't read the
  # archive symbol table.
  ifndef GHS
    GHS = /project/doc/pkg/wind/green/vx$(CPU)
  endif # GHS
  AR = $(WIND_BASE)/host/$(WIND_HOST_TYPE)/bin/ar386
  ARFLAGS  = rsu
  LIBS += $(GHS)/libedgnoe.a
else
ifeq ($(CPU),ppc)
  ifndef GHS
    GHS = /appl/newgreen/vx$(CPU)603
  endif # GHS
  AR = ax
  ARFLAGS  = cr
  LIBS += $(GHS)/ghsbltin.o $(GHS)/ghsmath.o
endif # ppc
endif # CPU


CC       = cxvx$(CPU)
CXX      = $(CC)
CCFLAGS  += $(CFLAGS) --no_rtti -tnone
DCFLAGS  += -g
DLD      = $(CXX)
LD       = $(CXX)
LDFLAGS  = --no_auto_instantiation
OCFLAGS  += -OL -OM
PIC      =
