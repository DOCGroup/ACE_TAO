# $Id$
# VxWorks with Diab 4.2a/4.3.

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
#### 3) If you're building on a WIN32 host, be sure that $(CC) is on
####    your PATH (or is a full pathname).

VXWORKS = 1
CROSS-COMPILE = 1

ifndef debug
  debug = 1
endif # ! debug
ifndef optimize
  optimize = 1
endif # ! optimize
ifndef rtti
  rtti = 0
endif # ! rtti

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

ifeq (,$(PERL_PATH))
  PERL_PATH = perl
endif # ! PERL_PATH

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
  CFLAGS += -tPPC604EH:tornado -DCPU=PPC604
  CCFLAGS +=
  TOOLENV = ppc
else
ifeq ($(CPU),PPC603)
  BINXSYM_NAME = xsym
  CFLAGS += -tPPC604EH:tornado -DCPU=PPC603
  CCFLAGS +=
  TOOLENV = ppc
else
ifeq ($(CPU),PPC860)
  BINXSYM_NAME = xsym
  CFLAGS += -tPPC860EH:tornado -DCPU=PPC860
  TOOLENV = ppc
else
ifeq ($(CPU),I80486)
  BINXSYM_NAME = xsymDec
  CFLAGS += -tI486EH:tornado -DCPU=I80486  #### not sure about that
  TOOLENV = 386
else
ifeq ($(CPU),I80386)
  BINXSYM_NAME = xsymDec
  CFLAGS += -tI386EH:tornado -DCPU=I80386  #### not sure about that
  TOOLENV = 386
else
  BINXSYM_NAME = xsym
endif # I80386
endif # I80486
endif # PPC860
endif # PPC603
endif # PPC604

#### END target CPU-specific settings


ifeq ($(shared_libs),1)
  SHARED_LIBS = 1
endif
ifeq ($(shared_libs_only),1)
  SHARED_LIBS = 1
endif

ifeq ($(SHARED_LIBS),1)
  ACELIB = -L./
  #### There's no difference between non-shared and shared object code.
  VSHDIR = .obj/
endif # shared_libs

ifeq ($(static_libs),1)
  AR              = ar$(TOOLENV)
  ARFLAGS         = rv
else  # ! static_libs
  AR              = echo
  ARFLAGS         = "build shared lib only"
endif # ! static_libs

CC              = dplus
CXX             = $(CC)
CFLAGS          += -Xsmall-data=0 -Xsmall-const=0 -Xno-implicit-templates \
                   -Xstrings-in-text=0 -tPPC750EN:tornado
CCFLAGS         += $(CFLAGS) -Xenum-is-int -Xbool-is-int
ifeq (0,$(rtti))
  #### Don't use RTTI, because Tornado 1.0.x libraries don't support it.
  CCFLAGS += -DACE_LACKS_RTTI
endif # rtti
DCFLAGS         += -g
DLD             = $(LD)
INCLDIRS        += -I$(WIND_BASE)/target/h
LD              = $(PERL_PATH) $(ACE_ROOT)/bin/ace_ld \
                  -C $(COMPILE.c) -traditional -- \
                  -m "ddump -M" -n nm$(TOOLENV) dld
LDFLAGS         += -r4
#### WindRiver only recommends -O, not -O2, with some CPUs, including
#### the i386 family.  And -O2 causes some compilation failures.
OCFLAGS         += -XO
PIC             =

#### ld can't handle INCLDIRS, so override LINK definitions.
LINK.c           = override
LINK.c.override  = $(LD) $(LDFLAGS) $(LDLIBS) $(LIBS)

LINK.cc          = override
LINK.cc.override = $(LD)
