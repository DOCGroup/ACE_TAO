# This platform macros file is intended to work with Digital UNIX 4.0 (OSF/1 4.0).

CC		= cxx -g -pthread -ptr $(WRAPPER_ROOT)/ace/cxx_repository 
CXX		= cxx -w0 -g -pthread -ptr $(WRAPPER_ROOT)/ace/cxx_repository 
DLD		= $(CXX)
#LIBS		+= -lpthreads -lmach -lsys5 -lxti -ltli -lcxx -lexc -lc
LIBS		+= -ltli -lc_r
PIC		= 
ARFLAGS		= ruvZ
# When libraries are archived, a hash index is automatically created so there
# is no need for ranlib
RANLIB		= /usr/bin/true
SOFLAGS		=  -shared
SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
		  $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o -lACE -ltli -lc_r
#		  $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o -lcxx -ltask -lexc -lc 
