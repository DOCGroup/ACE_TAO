# $Id$

# SunOS 5.x (Solaris 2.x) with g++ *not* using Orbix

# NOTE: with g++ 2.8.0, you'll need to disable optimization in order to
#       instantiate ACE_Map_Manager (ace/Map_Manager.cpp).  The easiest
#       way to do that is to set "optimize = 0", either below or on
#       your make command line.

# NOTE: On Solaris86, you'll need to use GNU as instead of /usr/ccs/bin/as,
#       if you want -gstabs+ and -pipe support.

debug = 1
optimize = 1
threads = 1

ifeq ($(threads),0)
  CFLAGS += -DACE_MT_SAFE=0
endif # threads

ifeq ($(shell /bin/uname -i),i86pc)
  #### gcc on Solaris86 doesn't use -g
  DCFLAGS         += -gstabs+
else  # ! i86pc
  DCFLAGS         += -g
endif # ! i86pc

CC              = gcc
CXX             = g++
CFLAGS          += -Wall -Wpointer-arith -pipe #### -Winline
CCFLAGS         += $(CFLAGS) -fno-implicit-templates
DLD             = $(CXX)
LD              = $(CXX)
LDFLAGS         +=
LIBS            += -lsocket -ldl -lgen -lnsl -lposix4 -lthread
OCFLAGS         += -O2
PIC             = -fPIC
AR              = ar
ARFLAGS         = ruv
RANLIB          = @true
SOFLAGS         = -G $(CPPFLAGS)
SOBUILD         = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB          = @true
