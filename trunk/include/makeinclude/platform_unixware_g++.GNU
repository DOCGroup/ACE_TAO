# UnixWare V2.01 with g++ *not* using Orbix
#
# SOBUILD - compile into .so directly
#
CC		= gcc -w
CXX		= g++ -I. # -D__ACE_INLINE__      # -w -fno-strict-prototypes
DLD		= $(CXX)
LD		= $(CXX)
LDFLAGS		+= -L$(WRAPPER_ROOT)/ace -L./ -L/usr/lib
LIBS		+= -lsocket -lnsl -lstdc++ -lgen -lthread -lm
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
