# $Id$

# VxWorks 5.2-5.3.1 with Green Hills 1.8.8

CC		= cxvxppc
CXX		= $(CC)
CFLAGS		+= $(DCFLAGS)
CCFLAGS		+= $(CFLAGS) --no_rtti
DCFLAGS		+= -g
DLD		= $(CXX)
LD		= $(CXX)
LIBS            += /appl/newgreen/vxppc603/indlib.o
PIC		=
AR		= ax
ARFLAGS		= rv
RANLIB		= /bin/true
PRELIB	= (echo "int main() { return 0; }" > gcctemp.c && \
	  $(COMPILE.cc) -o gcctemp.o gcctemp.c && \
	  $(CXX) $(INCLDIRS) $^ $(LDFLAGS) $(LIBS); \
	  status=$$?; rm -f gcctemp.c a.out*; exit $$status)

#### don't build shared objects:
BUILD           = $(VOBJS) $(VLIB) $(VBIN)
