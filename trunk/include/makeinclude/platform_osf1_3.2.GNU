# $Id$

# This platform macros file is intended to work with Digital UNIX 3.2 (OSF/1 3.2).

CC		= cxx
CXX		= $(CC)
CFLAGS          += $(DCFLAGS) -ptr $(ACE_ROOT)/ptrepository
CCFLAGS		+= $(CFLAGS) -x cxx -w0
DCFLAGS		+= -g -threads
DLD		= ld -shared -expect_unresolved '*'
LD		= $(CXX)
LIBS		+= -lpthreads -lmach -lsys5 -lxti -ltli -lcxx -lexc -lc -lrt
PIC		= -pic
ARFLAGS		= ruv
RANLIB		= ranlib
SOFLAGS		= -shared 
SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
		  $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o -lcxx -ltask -lexc -lc

