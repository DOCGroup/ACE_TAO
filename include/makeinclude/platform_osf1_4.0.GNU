# This platform macros file is intended to work with Digital UNIX 4.0 (OSF/1 4.0).

CC		= cxx -g -pthread -ptr $(WRAPPER_ROOT)/ace/cxx_repository 
CXX		= cxx -w0 -g -pthread -ptr $(WRAPPER_ROOT)/ace/cxx_repository 
DLD		= $(CXX)
LD		= $(CXX)
LIBS		+= -pthread -ltli
PIC		= 
ARFLAGS		= ruvZ
# When libraries are archived, a hash index is automatically created so there
# is no need for ranlib
RANLIB		= /usr/bin/true
SOFLAGS		=  -shared
SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
 		  $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o -lACE -pthread -ltli
