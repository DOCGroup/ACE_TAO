# VxWorks 5.2 with Green Hills 1.8.8
# *not* using Orbix
CC		= cxvxppc
#### CXX		= cxvxppc -I. -D__INLINE__
CXX		= cxvxppc -I.
CCFLAGS		+= --no_rtti
INCLDIRS	+= -I$(WRAPPER_ROOT) -I.
DLD		= $(CXX) 
LD		= $(CXX)
LDFLAGS		+= -L$(WRAPPER_ROOT)/ace -L./
PIC		=
AR		= ax 
ARFLAGS		= rv
RANLIB		= /bin/true 
SOFLAGS		= -G $(CPPFLAGS)
SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
		  $(SOLINK.cc) -o $@ -h $@ $(LDFLAGS) $(VSHDIR)$*.o
PRELIB	= (echo "int main() { return 0; }" > gcctemp.c && \
	  $(COMPILE.cc) -o gcctemp.o gcctemp.c && \
	  $(CXX) $(INCLDIRS) $^ $(LDFLAGS) $(LIBS); \
	  status=$$?; rm -f gcctemp.c; exit $$status)
