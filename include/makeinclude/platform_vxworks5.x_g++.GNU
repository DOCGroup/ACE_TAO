# VxWorks 5.2 with GNU g++ 2.7.2
# *not* using Orbix

TOOL_DIR	= /opt/wind
TARGET_DIR	= $(TOOL_DIR)/target
HOST_DIR	= $(TOOL_DIR)/host/sun4-solaris2
BIN_DIR		= $(HOST_DIR)/bin
LIB_DIR		= $(HOST_DIR)/powerpc-wrs-vxworks/lib
GCCLIB_DIR	= $(HOST_DIR)/lib/gcc-lib/powerpc-wrs-vxworks/cygnus-2.7.2-960126

CC		= $(BIN_DIR)/g++ppc
CXX		= $(CC)
CCFLAGS		+= -B$(GCCLIB_DIR)/ -DCPU=PPC604 -DVXWORKS
INCLDIRS	+= -I$(TARGET_DIR)/h
LD		= $(BIN_DIR)/ldppc
LDFLAGS		+= -r -L$(LIB_DIR) -L$(GCCLIB_DIR)
LIBS		+= -lstdc++ -lgcc
PIC		=
AR		= arppc
ARFLAGS		= rv
RANLIB		= /bin/true 
PRELIB	= (echo "main() { }" > gcctemp.c && \
	  $(COMPILE.cc) -o gcctemp.o gcctemp.c && \
	  $(LD) $^ $(LDFLAGS) $(LIBS); \
	  status=$$?; rm -f gcctemp.* a.out; exit $$status)

#### don't build shared objects:
BUILD           = $(VOBJS) $(VLIB) $(VBIN)
