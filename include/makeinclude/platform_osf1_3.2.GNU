# This platform macros file is intended to work with Digital UNIX 3.2 (OSF/1 3.2).

CC		= cxx -non_shared
CXX		= cxx -x cxx -w0 -non_shared
#DLD		= ld -non_shared
DLD		= ld -shared
LIBS		+= -lpthreads -lmach -lsys5 -lxti -ltli -lcxx -lexc -lc
PIC		= -pic
ARFLAGS		= ruv
RANLIB		= ranlib
SOFLAGS		= -shared -assert 
SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
		  $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o -lcxx -ltask -lexc -lc
