# VxWorks 5.2 with Green Hills 1.8.8
# *not* using Orbix
CC		= cxvxppc
#### CXX		= cxvxppc -D__INLINE__
CXX		= cxvxppc
CCFLAGS		+= --no_rtti
DLD		= $(CXX) 
LD		= $(CXX)
LDFLAGS		+= -L$(WRAPPER_ROOT)/ace -L./
LIBS            += /appl/newgreen/vxppc603/indlib.o
PIC		=
AR		= ax 
ARFLAGS		= rv
RANLIB		= /bin/true 
PRELIB	= (echo "int main() { return 0; }" > gcctemp.c && \
	  $(COMPILE.cc) -o gcctemp.o gcctemp.c && \
	  $(CXX) $(INCLDIRS) $^ $(LDFLAGS) $(LIBS); \
	  status=$$?; rm -f gcctemp.c; exit $$status)
