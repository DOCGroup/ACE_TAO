# $Id$
#
# Chorus 4.0 with egcs 1.1.2
#
# This version of plaform macros builds upon the previous version
# of chorus.  Many of the macros were the same so inclusion of
# the previous macros file seemed the easiest way to go.
#
# You will probably need to set the following environment variables
# or macros:
#
# Define your CPU:
# CPU               powerpc (default), sparc or ...
#
# Define these:
# CHTOOLS_DIR       <chorus install path>/4.0/chorus-$CPU/tools
# NUCLEUS_DIR       <chorus install path>/4.0/chorus-$CPU/kernel
#
# or
#
# Define MERGEDIR to your kernel build location.
#

CHORUS = 1
CHORUS_4 = 1
CROSS-COMPILE = 1

ifeq (,$(debug))
  debug     = 1
endif
exceptions  = 0
ifeq (,$(optimize))
  optimize  = 1
endif
static_libs_only = 1

#
# Directory where the Chorus Development System is installed
#
ifdef MERGEDIR
  include $(MERGEDIR)/Paths
endif
ifndef CHTOOLS_DIR
  CONFIGURATION_ERROR_CHTOOLS: CHTOOLS_DIR_must_be_set!
endif
ifndef NUCLEUS_DIR
  CONFIGURATION_ERROR_NUCLEUS: NUCLEUS_DIR_must_be_set!
endif
ifndef CPU
  CPU=powerpc
endif
ifndef CLX_TYPE
  CLX_TYPE=$(CPU)-elf
endif
ifndef CLX_CDSVERSION
  CLX_CDSVERSION = 5.00
endif
ifndef CLX_CDSHOST
  CLX_CDSHOST = solaris
endif
ifndef OS_DIR
  OS_DIR   = $(NUCLEUS_DIR)/../os
endif

XROOT   = $(CHTOOLS_DIR)/$(CPU)/$(CLX_CDSHOST)/$(CLX_CDSVERSION)
XDIR    = $(XROOT)/$(CLX_TYPE)
TOOLDIR = $(XDIR)/bin


#
# Include Dirs
#
NUCLEUS_INCLDIR = $(NUCLEUS_DIR)/include
OS_INCLDIR      = $(OS_DIR)/include

INCLDIRS += -I$(OS_INCLDIR)                 \
            -I$(NUCLEUS_INCLDIR)            \
            -I$(OS_INCLDIR)/chorus          \
            -I$(NUCLEUS_INCLDIR)/chorus     \
            -I$(OS_INCLDIR)/CC              \
            -I$(NUCLEUS_INCLDIR)/posix      \
            -I$(NUCLEUS_INCLDIR)/stdc       \
            -I$(NUCLEUS_INCLDIR)/chorus/iom \
            -I$(NUCLEUS_INCLDIR)/CC




#
# Tools
#
CC       = $(TOOLDIR)/gcc -B$(XROOT)/lib/gcc-lib/
CXX      = $(CC)
DLD             =
LD              = $(XDIR)/bin/ld
MATHLIB         = -L$(NUCLEUS_DIR)/lib/libm -lm
AR              = $(TOOLDIR)/ar
RANLIB          = $(TOOLDIR)/ranlib


# Test for template instantiation.
#
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

#
# Flag Settings
#
CFLAGS  += -w
CCFLAGS += $(CFLAGS) $(TEMPLATES_FLAG) \
           -fno-strict-prototypes               \
           -Usun                                \
           -fno-builtin                         \
           -fconserve-space                     \
           -fcheck-new                          \
           -nostdinc                            \
           -pipe                                \
           -fno-rtti                            \
           -fwritable-strings                   \
           -fsigned-char                        \
           "-msoft-float"                       \
           -DSOFT_FLOAT                         \
           -D_POSIX_THREADS                     \
           -D_POSIX_THREAD_SAFE_FUNCTIONS	\
           "-DNO_FPU_SUPPORT"

ifeq ($(CPU),powerpc)
CCFLAGS += -mstrict-align                       \
           -mno-sdata
endif

CPPFLAGS += -DCHORUS_4
CCFLAGS  += -D__CHORUS__=400                    \
            -DCHORUS_VERSION_MAJOR=4            \
            -DCHORUS_VERSION_MINOR=0            \
            -DCHORUS_PATCH_LEVEL=0              \
            -DCHORUS_4                          \
            -D_POSIX_THREAD_PROCESS_SHARED

ifeq ($(exceptions),1)
else  # ! exceptions
  CCFLAGS += -fno-exceptions
endif # ! exceptions

OCFLAGS        += -O
PIC             = -fPIC
ARFLAGS         = srv
#
# Adding debug symbols makes the binaries too
# large to load into memory.
#DCFLAGS  += -g



LINK.cc.override = $(CXX) -u _main -u _premain
LINK.cc = override

CHORUSLINK = true
PRELIB     = @true

#
# Special link process to get the right libraries and
# build up the static constructors and destructors.
#
POSTLINK= -r $(OS_DIR)/lib/CC/libC.a $(OS_DIR)/lib/libbsd/libbsd.a          \
             $(OS_DIR)/lib/classix/libcx.a                                  \
             $(NUCLEUS_DIR)/lib/classix/libsys.u.a                          \
             -nostdlib -lgcc-chorus;                                        \
        mv $@ $@.xp.o;                                                      \
        $(CHTOOLS_DIR)/host/bin/mkctors $@.xp.o > $@.ct.s;                  \
        $(CXX) -c $@.ct.s -o $@.ct.o;                                       \
        $(RM) $@.ct.s;                                                      \
        $(LD) $(LDOPTIONS) -e _start -r -dn -o $@                           \
              $(NUCLEUS_DIR)/lib/crt/crth.u.o $(NUCLEUS_DIR)/lib/crt/crt0.o \
              $(NUCLEUS_DIR)/lib/crt/crti.o $@.xp.o $@.ct.o                 \
              $(NUCLEUS_DIR)/lib/crt/crtn.o;                                \
        $(RM) $@.xp.o;                                                      \
        $(RM) $@.ct.o
