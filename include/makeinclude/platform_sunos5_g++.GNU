# $Id$

# SunOS 5.x (Solaris 2.x) with g++ *not* using Orbix

debug = 1
optimize = 1

CC              = gcc
CXX             = g++
CFLAGS          += -Wall -Wpointer-arith -pipe #### -Winline
CCFLAGS         += $(CFLAGS) -fno-implicit-templates
DCFLAGS         += -g
DLD             = $(CXX)
LD              = $(CXX)
LDFLAGS         += # -z muldefs
LIBS            += -lsocket -ldl -lgen -lnsl -lthread -lw
OCFLAGS         += -O2
PIC             = -fpic
AR              = ar
ARFLAGS         = ruv
RANLIB          = @true
SOFLAGS         = -G $(CPPFLAGS)
# SOBUILD - compile into .so directly
SOBUILD         = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB          = @true
