# $Id$
# LynxOS with g++

CROSS-COMPILE = 1
debug = 1
shared_libs =
static_libs = 1

CC              = gcc
CXX             = g++
CFLAGS          += -mthreads -pipe -ansi -Wall -Wpointer-arith -Winline
CCFLAGS         += $(CFLAGS) -fno-implicit-templates
DCFLAGS         += -g
DLD             = $(CXX)
LD              = $(CXX)
LIBS            += -lnetinet -lnsl
OCFLAGS         += -O2
AR              = ar
ARFLAGS         = ruv
RANLIB          = @true
PRELIB          = @true

# To save much disk space, strip all executables.  Comment the
# following line out if you want to debug.
POSTLINK        = ; strip $@
