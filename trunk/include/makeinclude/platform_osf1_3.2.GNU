# This platform macros file is intended to work with Digital UNIX 3.2 (OSF/1 3.2).

CC		= cxx
CXX		= $(CC)
CFLAGS		+= $(DCFLAGS) -non_shared
CCFLAGS		+= $(CFLAGS) -x cxx -w0
DCFLAGS		+= -g -threads
#DLD		= ld -non_shared
DLD		= ld -shared -expect_unresolved '*'
LD		= $(CXX)
LIBS		+= -lpthreads -lmach -lsys5 -lxti -ltli -lcxx -lexc -lc
PIC		= -pic
ARFLAGS		= ruv
RANLIB		= ranlib
SOFLAGS		= -shared -assert 
SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
		  $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o -lcxx -ltask -lexc -lc
