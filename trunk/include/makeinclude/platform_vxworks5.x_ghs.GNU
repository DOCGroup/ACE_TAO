# VxWorks 5.2 with Green Hills 1.8.8
# *not* using Orbix
CC		= cxvxppc
CXX		= $(CC)
CCFLAGS		+= --no_rtti # -D__ACE_INLINE__
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
