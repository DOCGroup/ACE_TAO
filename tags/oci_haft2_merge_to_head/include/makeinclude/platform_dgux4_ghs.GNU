# $Id$

# DG/UX 4.x using Green Hills Multi's C++ compiler.

debug = 1
#shared_libs_only = 0
#static_libs_only = 1
#static_libs = 1

CC		= cc
CXX		= ghcx
CCFLAGS		+= $(CFLAGS) -elf -template=noauto # -ansicxx
# There was a link-time failure if -g (debugging) was turned on, and Green
# Hills was not able to try to fix it, so debugging is via ACE_OS::printf :-(
DCFLAGS		+= # -g
DLD		= $(CXX) -shared
LD		= $(CXX)
LIBS		+= -lthread -lrte -lgen -lsocket -ldl -lnsl # -lc
PIC		= -pic
AR		= $(CXX) -archive -o
ARFLAGS		=
RANLIB		= echo
#SOFLAGS		= -G $(CPPFLAGS)
#SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
#		  $(SOLINK.cc) -o $@ -h $@ $(LDFLAGS) $(VSHDIR)$*.o
#SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
#		 $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o
