# $Id$

# UnixWare V2.2.1.2 with g++ version 2.7.2.2 *not* using Orbix
#
# SOBUILD - compile into .so directly
#
CC              = gcc
CXX             = g++
CFLAGS          += $(DCFLAGS) -I/usr/include -D__STDC__=0 #-w
CCFLAGS         += $(CFLAGS) -fno-implicit-templates # -fno-strict-prototypes
DCFLAGS         += -g
DLD             = $(CXX)
LD              = $(CXX)
LDFLAGS         += -L/usr/lib
LIBS            += -lsocket -lnsl -lgen -lthread
PIC             = -fpic
AR              = ar
ARFLAGS         = ruv
RANLIB          = /bin/true
#SOFLAGS        = -assert pure-text
SOFLAGS         = -shared
SOBUILD         = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
# SOLINK        = $(SOLINK.cc) -o $@ $(LDFLAGS) $<
PRELIB          = (echo "int main() { return 0; }" > gcctemp.c && \
                  $(COMPILE.cc) -o gcctemp.o gcctemp.c && \
                  $(LINK.cc) -o gcctemp gcctemp.o $^ $(LDFLAGS) $(LIBS); \
                  status=$$?; rm -f gcctemp*; exit $$status)
