# $Id$
#
# VxWorks 5.3.1 and later with Green Hills 1.8.8 and later.

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
#### GHSROOT = /project/doc/pkg/wind/green
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
  ifndef GHSROOT
    GHSROOT = /project/doc/pkg/wind/green
  endif
  ifndef GHS
    GHS = $(GHSROOT)/vx$(CPU)
  endif # GHS
  AR = $(WIND_BASE)/host/$(WIND_HOST_TYPE)/bin/ar386
  ARFLAGS  = rsu

  #### For GHS 1.8.8:
  #### LIBS += $(GHS)/libedgnoe.a
else
ifeq ($(CPU),ppc)
  ifndef GHSROOT
    GHSROOT = /appl/newgreen
  endif
  ifndef GHS
    GHS = $(GHSROOT)/vx$(CPU)603
  endif # GHS
  AR = ax
  ARFLAGS  = cr

  #### For GHS 1.8.8:
  #### LIBS += $(GHS)/ghsbltin.o $(GHS)/ghsmath.o
endif # ppc
endif # CPU

#### For GHS 1.8.9:
LIBS += $(GHS)/libscnoe.a $(GHS)/libsedgnoe.a $(GHS)/liblockinit_sc.o $(GHSROOT)/$(CPU)/libarch.a

CC       = cxvx$(CPU)
CXX      = $(CC)
CCFLAGS  += $(CFLAGS) --no_rtti -tnone -Dghs -I$(GHSROOT)/vxworks/include -I$(WIND_BASE)/target/h
DCFLAGS  += -g
SOFLAGS  += -shared
DLD      = $(CXX)
LD       = $(CXX)
LDFLAGS  = --no_auto_instantiation
OCFLAGS  += -OL -OM
PIC      =

ifeq (1, $(exceptions))
   CCFLAGS += --stdle --exceptions
endif # exceptions
