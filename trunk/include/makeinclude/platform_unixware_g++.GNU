# UnixWare V2.01 with g++ *not* using Orbix
#
# SOBUILD - compile into .so directly
#
CC		= gcc
CXX		= g++
CFLAGS          += -w
CCFLAGS         += -w -fno-implicit-templates       # -fno-strict-prototypes
DLD		= $(CXX)
LD		= $(CXX)
LDFLAGS		+= -L/usr/lib
LIBS		+= -lsocket -lnsl -lgen -lthread
PIC		= -fpic
AR		= ar
ARFLAGS		= ruv
RANLIB		= /bin/true
#SOFLAGS	= -assert pure-text 
SOFLAGS 	= -shared
SOBUILD 	= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
# SOLINK	= $(SOLINK.cc) -o $@ $(LDFLAGS) $<
PRELIB		= (echo "int main() { return 0; }" > gcctemp.c && \
		  $(COMPILE.cc) -o gcctemp.o gcctemp.c && \
		  $(LINK.cc) -o gcctemp gcctemp.o $^ $(LDFLAGS) $(LIBS); \
		  status=$$?; rm -f gcctemp*; exit $$status)
