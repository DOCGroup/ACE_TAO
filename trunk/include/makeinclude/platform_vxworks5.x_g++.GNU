# $Id$
# VxWorks 5.2-5.3.1 with GNU g++ 2.7.2

#### Notes:
#### 1) This file assumes that the WIND_BASE environment variable is set.
####
#### 2) If you have problems with munch output not being compilable
####    because it contains identifiers with ".", e.g., ".cpp", in them:
####    add a global variable or function to that .cpp file.  See
####    ace/IOStream_T.cpp for an explanation and example.

debug = 1
optimize = 1
static_libs_only = 1


#### BEGIN CPU-specific settings

#### This is kind of messy so that it can support multiple
#### targets.  You'll need to set CPU to a supported CPU
#### using one of these methods (unless your CPU is a PowerPC 604):
#### 1) create a $ACE_ROOT/include/makeinclude/platform_macros.GNU
####    that contains something like:
####  CPU = I80486
####  include $(ACE_ROOT)/include/makeinclude/platform_vxworks5.2_g++.GNU
#### 2) modify the code below to set CPU, or
#### 3) set CPU on the command line, e.g., "make CPU=I80486"
ifeq ($(CPU),)
  CPU = PPC604
endif # default CPU

ifeq ($(CPU),PPC604)
  CFLAGS += -B$(GCCLIB_DIR)/ -DCPU=PPC604
  GCCLIB_DIR = $(HOST_DIR)/lib/gcc-lib/powerpc-wrs-vxworks/cygnus-2.7.2-960126
  TOOLENV = ppc
else
ifeq ($(CPU),I80486)
  CFLAGS += -DCPU=I80486 -m486
  TOOLENV = 386
endif # I80486
endif # PPC604

#### END CPU-specific settings


TARGET_DIR      = $(WIND_BASE)/target
HOST_DIR        = $(WIND_BASE)/host/sun4-solaris2
BIN_DIR         = $(HOST_DIR)/bin

CC              = cc$(TOOLENV)
CXX             = g++$(TOOLENV)
CFLAGS          += -DVXWORKS -D_REENTRANT -ansi -fno-builtin -fno-defer-pop -fvolatile -nostdinc -nostdlib -pipe -Wall
#### Note: -f-no-implicit-templates doesn't seem to work well with
####       g++ cygnus-2.7.2-960126 for pc486 target.  Some
####       member functions don't get instantiated.  Wind River knows
####       about this problem (SPR 8483).
CCFLAGS         += $(CFLAGS) #### -fno-implicit-templates
DCFLAGS         += -g
INCLDIRS        += -I$(WIND_BASE)/target/h
LD              = ld$(TOOLENV)
LDFLAGS         += -X -r .obj/__ctordtor.o
#### WindRiver only recommends -O, not -O2, with some CPUs, including
#### the i386 family.
OCFLAGS         += -O
PIC             =
AR              = ar$(TOOLENV)
ARFLAGS         = rv
RANLIB          = /bin/true
PRELINK         = nm$(TOOLENV) .obj/$@.o $(MUNCH) $(ACE_ROOT)/ace/libACE.a | munch | egrep -v '\.cpp' > __ctordtor.c; $(COMPILE.c) -traditional -o .obj/__ctordtor.o __ctordtor.c; /bin/rm __ctordtor.c;

#### ld can't handle INCLDIRS, so override LINK definitions.
LINK.c           = override
LINK.c.override  = $(LD) $(LDFLAGS) $(LDLIBS) $(LIBS)

LINK.cc          = override
LINK.cc.override = $(PRELINK) $(LD) $(PTDIRS)
POSTLINK         = ; VX_CPU_FAMILY=$(TOOLENV) xsymDec < $@ > $@.sym
