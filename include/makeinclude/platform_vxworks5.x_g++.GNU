# $Id$
# VxWorks 5.2-5.3.1 with GNU g++ 2.7.2

#### Notes:
#### 1) This file requires that the WIND_BASE and WIND_HOST_TYPE environment
####    variables be set.  If the target CPU is not a PPC604, then your CPU
####    environment variable must be set.  If perl is not on your path,
####    you'll also need to set your PERL_PATH environment variable to
####    the full path to perl.
#### 2) If you have problems with munch output not being compilable
####    because it contains identifiers with ".", e.g., ".cpp", in them:
####    add a global variable or function to that .cpp file.  See
####    ace/IOStream_T.cpp for an explanation and example.

VXWORKS = 1
CROSS-COMPILE = 1

debug = 1
optimize = 1
shared_libs =
static_libs = 1


ifeq (,$(WIND_BASE))
  default:
	@ERROR: you must set your WIND_BASE environment variable
endif # WIND_BASE

ifeq (,$(WIND_HOST_TYPE))
  default:
	@ERROR: you must set your WIND_HOST_TYPE environment variable
endif # WIND_HOST_TYPE

HOST_DIR = $(WIND_BASE)/host/$(WIND_HOST_TYPE)

ifeq (,$(findstring $(HOST_DIR)/bin,$PATH))
  #### Use := instead of += because += inserts a space.
  PATH := $(PATH):$(HOST_DIR)/bin
endif # PATH

PERL_PATH = perl

ifeq (win32,$(findstring win32,$(WIND_HOST_TYPE)))
  #### GNU make on WIN32 needs quoted double quotes.
  ACE_QUOTE ='"'
else  # ! win32 host
  ACE_QUOTE ="
endif # ! win32 host


#### BEGIN target CPU-specific settings


#### This is kind of messy so that it can support multiple targets.
#### You'll need to set CPU to a supported CPU using one of these
#### methods (unless your CPU is a PowerPC 604):
#### 1) create a $ACE_ROOT/include/makeinclude/platform_macros.GNU
####    that contains something like:
####
####  WIND_BASE = /project/doc/pkg/wind
####  WIND_HOST_TYPE = sun4-solaris2
####  CPU = I80486
####
####  include $(ACE_ROOT)/include/makeinclude/platform_vxworks5.x_g++.GNU
####
#### 2) modify the code below to set CPU, or
#### 3) set CPU on the command line, e.g., "make CPU=I80486"
ifeq ($(CPU),)
  CPU = PPC604
endif # default CPU

ifeq ($(CPU),PPC604)
  BINXSYM_NAME = xsym
  CFLAGS += -B$(GCCLIB_DIR)/ -DCPU=PPC604
  CCFLAGS += -fno-implicit-templates
  GCCLIB_DIR = $(HOST_DIR)/lib/gcc-lib/powerpc-wrs-vxworks/cygnus-2.7.2-960126
  TOOLENV = ppc
else
ifeq ($(CPU),PPC860)
  BINXSYM_NAME = xsym
  CFLAGS += -B$(GCCLIB_DIR)/ -DCPU=PPC860 -mcpu=860
  CCFLAGS += -fno-implicit-templates
  GCCLIB_DIR = $(HOST_DIR)/lib/gcc-lib/powerpc-wrs-vxworks/cygnus-2.7.2-960126
  TOOLENV = ppc
else
ifeq ($(CPU),I80486)
  #### Note: -fno-implicit-templates doesn't seem to work well with g++
  #### cygnus-2.7.2-960126 for pc486 target.  Some member functions don't
  #### get instantiated.  Wind River knows about this problem (SPR 8483).
  BINXSYM_NAME = xsymDec
  CFLAGS += -DCPU=I80486 -m486
  TOOLENV = 386
else
ifeq ($(CPU),I80386)
  BINXSYM_NAME = xsymDec
  CFLAGS += -DCPU=I80386
  TOOLENV = 386
else
  BINXSYM_NAME = xsym
  CCFLAGS += -fno-implicit-templates
endif # I80386
endif # I80486
endif # PPC860
endif # PPC604


#### END target CPU-specific settings


#### Set up Win32 vs. Unix host specific macros.
ifeq ($(WIND_HOST_TYPE),x86-win32)
  CXX      = cc$(TOOLENV)
else  # ! x86-win32
  CXX      = g++$(TOOLENV)
  PIPE_OPT = -pipe
endif # ! x86-win32


CC              = cc$(TOOLENV)
CFLAGS          += -D_REENTRANT -ansi -fno-builtin -fno-defer-pop \
                   -fvolatile -nostdinc -nostdlib $(PIPE_OPT) -Wall
CCFLAGS         += $(CFLAGS)
DCFLAGS         += -g
INCLDIRS        += -I$(WIND_BASE)/target/h
LD              = $(PERL_PATH) $(ACE_ROOT)/bin/ace_ld -c \
                  $(ACE_QUOTE)$(COMPILE.c) -traditional$(ACE_QUOTE) \
                  -m "munch" -n "nm$(TOOLENV)" ld$(TOOLENV)
LDFLAGS         += -X -r
#### WindRiver only recommends -O, not -O2, with some CPUs, including
#### the i386 family.  And -O2 causes some compilation failures.
OCFLAGS         += -O
PIC             =
AR              = ar$(TOOLENV)
ARFLAGS         = rv


#### ld can't handle INCLDIRS, so override LINK definitions.
LINK.c           = override
LINK.c.override  = $(LD) $(LDFLAGS) $(LDLIBS) $(LIBS)

LINK.cc          = override
LINK.cc.override = $(LD)


#### To extract the symbol table from each executable, uncomment the POSTLINK
#### definition below.
## POSTLINK         = ; VX_CPU_FAMILY=$(TOOLENV) $(BINXSYM_NAME) < $@ > $@.sym
