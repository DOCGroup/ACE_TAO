# $Id$
# LynxOS with g++

debug = 1
static_libs_only = 1

CC              = gcc
CXX             = g++
CFLAGS          += -mthreads -pipe      #### -Wall -Wpointer-arith -Winline
CCFLAGS         += $(CFLAGS) -fno-implicit-templates -fno-strict-prototype
DCFLAGS         += -g
DLD             = $(CXX)
LD              = $(CXX)
LIBS            += -lsocket -ldl -lgen -lnsl -lthread -lw
OCFLAGS         += -O2
AR              = ar
ARFLAGS         = ruv
RANLIB          = @true
PRELIB          = @true
