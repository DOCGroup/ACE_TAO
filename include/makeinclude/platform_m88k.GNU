# $Id$

# SunOS 5.x (Solaris 2.x) with SunC++ 4.x
# *not* using Orbix
CC		= gcc
CXX             = g++
CFLAGS		+= $(DCFLAGS) -w -O2
CCFLAGS		+= $(CFLAGS) -D__m88k__ -fno-implicit-templates -fno-strict-prototypes
DCFLAGS		+= -g
DLD		= $(CXX) 
LD		= $(CXX) 
INCLDIRS	+= -I$(ACE_ROOT) -I.
INCLDIRS	+= -I/devel/rmm/v1.0/local/src/threads/include
LDFLAGS		+= -L $(ACE_ROOT)/ace -L ./
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
