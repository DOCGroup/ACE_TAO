# VxWorks 5.2 with Green Hills 1.8.8
# *not* using Orbix
CC		= cxvxppc
#### CXX		= cxvxppc -I. -D__INLINE__
CXX		= cxvxppc -I.
DLD		= $(CXX) 
INCLDIRS	+= -I$(WRAPPER_ROOT) -I.
LD		= lx
LDFLAGS		+= -r -I/appl/newgreen/vxworks/ccincl -I/appl/newgreen/vxworks/include	-I/appl/newgreen/ansi -I/appl/newgreen/vxworks/usrinc -L$(WRAPPER_ROOT)/ace -L./
LIBS            += /appl/newgreen/vxppc603/indlib.o	
PIC		=
AR		= ax 
ARFLAGS		= rv
RANLIB		= /bin/true 
SOFLAGS		= -G $(CPPFLAGS)
SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
		  $(SOLINK.cc) -o $@ -h $@ $(LDFLAGS) $(VSHDIR)$*.o
PRELIB	= (echo "main() { }" > gcctemp.c && \
	  $(COMPILE.cc) -o gcctemp.o gcctemp.c && \
	  $(CXX) $(INCLDIRS) $^ $(LDFLAGS) $(LIBS); \
	  status=$$?; rm -f gcctemp.c; exit $$status)
