# platform_vxworks5.x_g++.GNU,v 4.83 2001/05/16 07:41:05 mk1 Exp
#
# VxWorks 5.3.1 and later with GNU g++ that is distributed by Wind River.

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

ifndef exceptions
  exceptions = 0
endif # ! exceptions
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

ifeq (,$(findstring $(HOST_DIR)/bin,$(PATH)))
  #### Use := instead of += because += inserts a space.
  PATH := $(PATH):$(HOST_DIR)/bin
endif # PATH

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

ifeq ($(CPU),SH7700)
  BINXSYM_NAME = xsym
  CFLAGS += -B$(GCCLIB_DIR)/ -DCPU=SH7700 -m3
  CCFLAGS += -fno-implicit-templates
  GCCLIB_DIR = $(HOST_DIR)/lib/gcc-lib/sh-wrs-vxworks/gcc-2.96
  TOOLENV = sh
else
ifeq ($(CPU),SH7700LE)
  BINXSYM_NAME = xsym
  CFLAGS += -B$(GCCLIB_DIR)/ -DCPU=SH7700 -m3 -ml
  CCFLAGS += -fno-implicit-templates
  GCCLIB_DIR = $(HOST_DIR)/lib/gcc-lib/sh-wrs-vxworks/gcc-2.96
  TOOLENV = sh
else
ifeq ($(CPU),SH7750)
  BINXSYM_NAME = xsym
  CFLAGS += -B$(GCCLIB_DIR)/ -DCPU=SH7750 -m4
  CCFLAGS += -fno-implicit-templates
  GCCLIB_DIR = $(HOST_DIR)/lib/gcc-lib/sh-wrs-vxworks/gcc-2.96
  TOOLENV = sh
else
ifeq ($(CPU),SH7750LE)
  BINXSYM_NAME = xsym
  CFLAGS += -B$(GCCLIB_DIR)/ -DCPU=SH7750 -m4 -ml -mspace
  CCFLAGS += -fno-implicit-templates
  GCCLIB_DIR = $(HOST_DIR)/lib/gcc-lib/sh-wrs-vxworks/gcc-2.96
  TOOLENV = sh
else
ifeq ($(CPU),PPC403)
  BINXSYM_NAME = xsym
  CFLAGS += -B$(GCCLIB_DIR)/ -DCPU=PPC403 -mcpu=403
  CCFLAGS += -fno-implicit-templates
  GCCLIB_DIR = $(HOST_DIR)/lib/gcc-lib/powerpc-wrs-vxworks/cygnus-2.7.2-960126
  TOOLENV = ppc
else
ifeq ($(CPU),PPC604)
  BINXSYM_NAME = xsym
  CFLAGS += -B$(GCCLIB_DIR)/ -DCPU=PPC604
  CCFLAGS += -fno-implicit-templates -mlongcall
  GCCLIB_DIR = $(HOST_DIR)/lib/gcc-lib/powerpc-wrs-vxworks/cygnus-2.7.2-960126
  TOOLENV = ppc
else
ifeq ($(CPU),PPC603)
  BINXSYM_NAME = xsym
  CFLAGS += -B$(GCCLIB_DIR)/ -DCPU=PPC603
  CCFLAGS += -fno-implicit-templates -mlongcall
  GCCLIB_DIR = $(HOST_DIR)/lib/gcc-lib/powerpc-wrs-vxworks/cygnus-2.7.2-960126
  TOOLENV = ppc
else
ifeq ($(CPU),PPC860)
  BINXSYM_NAME = xsym
  CFLAGS += -B$(GCCLIB_DIR)/ -DCPU=PPC860 -mcpu=860
  CCFLAGS += -fno-implicit-templates -mlongcal
  GCCLIB_DIR = $(HOST_DIR)/lib/gcc-lib/powerpc-wrs-vxworks/cygnus-2.7.2-960126
  TOOLENV = ppc
else
ifeq ($(CPU),SIMNT)
  BINXSYM_NAME = xsym
  CFLAGS += -U__WINNT -UWIN32 -U__WINNT__ -UWINNT -U__MINGW32__ -U_WIN32 \
            -U__WIN32 -U__WIN32__ -U__WIN32 -B$(GCCLIB_DIR) -DCPU=SIMNT
  CCFLAGS += -fno-implicit-templates
  GCCLIB_DIR = \
    $(HOST_DIR)/lib/gcc-lib/powerpc-wrs-vxworks/cygnus-2.7.2-960126
  TOOLENV = simpc
else
ifeq ($(CPU),PENTIUMPRO)
  BINXSYM_NAME = xsymDec
  CFLAGS += -DCPU=PENTIUMPRO
  TOOLENV = 386
else
ifeq ($(CPU),PENTIUM)
  BINXSYM_NAME = xsymDec
  CFLAGS += -DCPU=PENTIUM
  TOOLENV = 386
else
ifeq ($(CPU),I80486)
  #### Note: -fno-implicit-templates doesn't seem to work well with g++
  #### cygnus-2.7.2-960126 for pc486 target.  Some member functions don't
  #### get instantiated.  Wind River knows about this problem (SPR 8483).
  #### -fno-implicit-templates should be supported on Tornado II, so
  #### it is added for that platform below.
  BINXSYM_NAME = xsymDec
  CFLAGS += -DCPU=I80486 -m486
  TOOLENV = 386
else
ifeq ($(CPU),I80386)
  BINXSYM_NAME = xsymDec
  CFLAGS += -DCPU=I80386
  TOOLENV = 386
else
ifeq ($(CPU),SIMSPARCSOLARIS)
  BINXSYM_NAME = xsym
  CFLAGS += -B$(GCCLIB_DIR)/ -DCPU=SIMSPARCSOLARIS
  CCFLAGS += -fno-implicit-templates
  GCCLIB_DIR = \
    $(HOST_DIR)/lib/gcc-lib/sparc-sun-solaris2.5.1/cygnus-2.7.2-960126
  TOOLENV = simso
else
ifeq ($(CPU),SPARC)
  BINXSYM_NAME = xsym
  CFLAGS += -B$(GCCLIB_DIR)/ -DCPU=SPARC
  CCFLAGS += -fno-implicit-templates
  GCCLIB_DIR = $(HOST_DIR)/lib/gcc-lib/sparc-wrs-vxworks/cygnus-2.7.2-960126
  TOOLENV = sparc
else
ifeq ($(CPU),MC68020)
  CFLAGS += -B$(GCCLIB_DIR)/ -DCPU=MC68020 -m68020 -m68881
  CCFLAGS += -fno-implicit-templates -DACE_VXWORKS=0x540
  GCCLIB_DIR = $(HOST_DIR)/lib/gcc-lib/m68k-wrs-vxworks/cygnus-2.7.2-960126
  TOOLENV = 68k
else
ifeq ($(CPU),MC68030)
  BINXSYM_NAME = xsym
  CFLAGS += -B$(GCCLIB_DIR)/ -DCPU=MC68030 -m68030 -m68881
  CCFLAGS += -fno-implicit-templates -DACE_VXWORKS=0x540
  GCCLIB_DIR = $(HOST_DIR)/lib/gcc-lib/m68k-wrs-vxworks/cygnus-2.7.2-960126
  TOOLENV = 68k
else
ifeq ($(CPU),MC68040)
  BINXSYM_NAME = xsym
  CFLAGS += -B$(GCCLIB_DIR)/ -DCPU=MC68040 -m68040 -m68881
  CCFLAGS += -fno-implicit-templates -DACE_VXWORKS=0x540
  GCCLIB_DIR = $(HOST_DIR)/lib/gcc-lib/m68k-wrs-vxworks/cygnus-2.7.2-960126
  TOOLENV = 68k
else
ifeq ($(CPU),MC68060)
  BINXSYM_NAME = xsym
  CFLAGS += -B$(GCCLIB_DIR)/ -DCPU=MC68060 -m68040 -m68881
  CCFLAGS += -fno-implicit-templates -DACE_VXWORKS=0x540
  GCCLIB_DIR = $(HOST_DIR)/lib/gcc-lib/m68k-wrs-vxworks/cygnus-2.7.2-960126
  TOOLENV = 68k
else
  BINXSYM_NAME = xsym
  CCFLAGS += -fno-implicit-templates
endif # MC68060
endif # MC68040
endif # MC68030
endif # MC68020
endif # SPARC
endif # SIMSPARCSOLARIS
endif # I80386
endif # I80486
endif # PENTIUM
endif # PENTIUMPRO
endif # SIMNT
endif # PPC860
endif # PPC603
endif # PPC604
endif # PPC403
endif # SH7750LE
endif # SH7750
endif # SH7700LE
endif # SH7700

#### END target CPU-specific settings


#### Set up Win32 vs. Unix host specific macros.
ifeq ($(WIND_HOST_TYPE),x86-win32)
  CXX      = cc$(TOOLENV)
else
ifeq ($(TOOLENV),simso)
  #### vxsim needs to use ccsimso, not g++simso.
  CXX      = cc$(TOOLENV)
else
  CXX      = g++$(TOOLENV)
  PIPE_OPT = -pipe
endif # simso
endif # x86-win32

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

CC              = cc$(TOOLENV)

#### Tornado II uses egcs
ifndef ACE_CC
  ifndef COMSPEC
    ifdef ComSpec
      #### ACE+TAO use COMSPEC, but ComSpec is defined on some WIN32's.
      COMSPEC = $(ComSpec)
    endif # ComSpec
  endif # ! COMPSPEC

  ifdef COMSPEC
    #### Assume we're on a WIN32 host.  Further, assume that $(CC)
    #### is on the user's PATH.
    ACE_CC := $(CC)
  else  # ! COMSPEC
    #### Don't use this on WIN32 host, because the shell there seems
    #### to have trouble exec'ing sed.
    ACE_CC := $(shell PATH=${PATH} type $(CC) | sed -e 's/.* is //')
  endif # ! COMSPEC
endif # ! ACE_CC
ifndef ACE_CC_VERSION
  ACE_CC_VERSION := $(shell $(ACE_CC) --version)
endif # ! ACE_CC_VERSION
ifeq (egcs,$(findstring egcs,$(ACE_CC_VERSION)))
  ACE_HAS_GNUG_PRE_2_8 := 0
  CFLAGS += -DACE_VXWORKS=0x540
  ifeq ($(TOOLENV),386)
    #### Tornado II should support -fno-implicit-templates for pc486.
    CCFLAGS += -fno-implicit-templates
  endif # 386
  ifneq (1,$(exceptions))
    #### With exceptions, this is applied in wrapper_macros.GNU.
    #### Without, we do it here.
    CFLAGS += -Wno-uninitialized
  endif # ! exceptions
endif # egcs

CFLAGS          += -D_REENTRANT -ansi -fno-builtin -fno-defer-pop \
                   -fvolatile -nostdinc -nostdlib $(PIPE_OPT) -W -Wall
CCFLAGS         += $(CFLAGS)
ifeq (0,$(rtti))
  #### Don't use RTTI even with egcs, because Tornado 1.0.x libraries
  #### don't support it.
  CCFLAGS += -fno-rtti -DACE_LACKS_RTTI
endif # rtti
DCFLAGS         += -g
DLD             = $(LD)
INCLDIRS        += -I$(WIND_BASE)/target/h
LD              = $(PERL_PATH) $(ACE_ROOT)/bin/ace_ld \
                  -C $(COMPILE.c) -traditional -- \
                  -m munch -n nm$(TOOLENV) ld$(TOOLENV)
LDFLAGS         += -X -r
#### WindRiver only recommends -O, not -O2, with some CPUs, including
#### the i386 family.  And -O2 causes some compilation failures.
OCFLAGS         += -O
PIC             =


#### ld can't handle INCLDIRS, so override LINK definitions.
LINK.c           = override
LINK.c.override  = $(LD) $(LDFLAGS) $(LDLIBS) $(LIBS)

LINK.cc          = override
LINK.cc.override = $(LD)


#### To extract the symbol table from each executable, uncomment the POSTLINK
#### definition below.
## POSTLINK         = ; VX_CPU_FAMILY=$(TOOLENV) $(BINXSYM_NAME) < $@ > $@.sym

ifeq (1,$(exceptions))
  #### The VxWorks libraries don't support exceptions.  And, exceptions
  #### give the (Tornado II) compiler fits, e.g., with
  #### ACE_wrappers/TAO/tao/IORManipulation.cpp.
  CONFIGURATION_ERROR: exception_handling_is_not_supported_on_VxWorks!
endif # exceptions
