# SunOS 5.x (Solaris 2.x) with SunC++ 4.x
# *not* using Orbix
CC		= gcc -w
#CXX		= g++ -w -frepo -I. -fno-strict-prototypes -D__INLINE__
CXX             = g++ -w -I. -fno-strict-prototypes -D__INLINE__
CXX		+= -D__m88k__
DLD		= $(CXX) 
LD		= $(CXX) 
INCLDIRS	+= -I$(WRAPPER_ROOT) -I.
INCLDIRS	+= -I/devel/rmm/v1.0/local/src/threads/include
LDFLAGS		+= -L $(WRAPPER_ROOT)/ace -L ./
LDFLAGS		+= -L/devel/rmm/v1.0/local/src/threads/lib
LIBS		+= -lsocket -ldl -lnsl -lgen -lstdc++ -lg++
LIBS		+= -lgthreads -lgmalloc
PIC		= -fpic
AR		= ar 
ARFLAGS		= ruv 
RANLIB		= /bin/true 
SOFLAGS		= -G $(CPPFLAGS)
SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
		  $(SOLINK.cc) -o $@ -h $@ $(LDFLAGS) $(VSHDIR)$*.o
PRELIB	= (echo "int main() { return 0; }" > gcctemp.c && \
	  $(COMPILE.cc) -o gcctemp.o gcctemp.c && \
	  $(LINK.cc) -o gcctemp gcctemp.o $^ $(LDFLAGS) $(LIBS); \
	  status=$$?; rm -f gcctemp*; exit $$status)
