# VxWorks 5.2-5.3.1 with GNU g++ 2.7.2

#### This file assumes that the WIND_BASE environment variable is set.

#### BEGIN CPU-specific settings

#### This is kind of messy so that it can support multiple
#### targets.  You'll need to set CPU to a supported CPU
#### using one of these methods (unless your CPU is a PowerPC 604):
#### 1) create a $WRAPPER_ROOT/include/makeinclude/platform_macros.GNU
####    that contains something like:
####  CPU = I80486
####  include $(WRAPPER_ROOT)/include/makeinclude/platform_vxworks5.2_g++.GNU
#### 2) modify the code below to set CPU, or
#### 3) set CPU on the command line, e.g., "make CPU=I80486"
ifeq ($(CPU),)
  CPU = PPC604
endif # default CPU

ifeq ($(CPU),PPC604)
  CCFLAGS += -B$(GCCLIB_DIR)/ -DCPU=PPC604
  GCCLIB_DIR = $(HOST_DIR)/lib/gcc-lib/powerpc-wrs-vxworks/cygnus-2.7.2-960126
  LIB_DIR = $(HOST_DIR)/powerpc-wrs-vxworks/lib
  TOOLENV = ppc
else
ifeq ($(CPU),I80486)
  CCFLAGS += -B$(GCCLIB_DIR)/ -DCPU=I80486 -m486
  GCCLIB_DIR = $(HOST_DIR)/lib/gcc-lib/i386-wrs-vxworks/cygnus-2.7.2-960126
  LIB_DIR = $(HOST_DIR)/i386-wrs-vxworks/lib
  TOOLENV = 386
endif # I80486
endif # PPC604

#### END CPU-specific settings


TARGET_DIR	= $(WIND_BASE)/target
HOST_DIR	= $(WIND_BASE)/host/sun4-solaris2
BIN_DIR		= $(HOST_DIR)/bin


CC		= $(BIN_DIR)/g++$(TOOLENV)
CXX		= $(CC)
CFLAGS		+= -DVXWORKS -I$(WIND_BASE)/target/h -ansi -nostdinc -O -fvolatile -nostdlib -fno-builtin -fno-defer-pop -Wall
CCFLAGS		+= $(DCFLAGS) $(CFLAGS) -fno-implicit-templates
DCFLAGS		+= -g
INCLDIRS	+=
LD		= $(CXX)
LDFLAGS		+= -r -L$(LIB_DIR) -L$(GCCLIB_DIR)
PIC		=
AR		= ar$(TOOLENV)
ARFLAGS		= rv
RANLIB		= /bin/true
PRELIB	= (echo "main() { }" > gcctemp.c && \
	  $(COMPILE.cc) -o gcctemp.o gcctemp.c && \
	  $(LD) $^ $(CFLAGS) $(LDFLAGS) $(LIBS); \
	  status=$$?; rm -f gcctemp.* a.out; exit $$status)

#### don't build shared objects:
BUILD           = $(VOBJS) $(VLIB) $(VBIN)

COMPILE.cc	=$(CXX) $(CCFLAGS) $(CPPFLAGS) $(PTDIRS) -c
LINK.cc		=$(PRELINK) $(LD) $(CCFLAGS) $(CPPFLAGS) $(PTDIRS)
