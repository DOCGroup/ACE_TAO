# This platform macros file is intended to work with Digital UNIX 4.0 (OSF/1 4.0).

CC		= cxx
CXX		= $(CC)
CFLAGS          += $(DCFLAGS) -pthread -ptr $(WRAPPER_ROOT)/ace/cxx_repository
CCFLAGS         += $(CFLAGS) -w0
DCFLAGS		+= -g
DLD		= $(CXX)
LD		= $(CXX)
LIBS		+= -pthread -ltli -lrt
PIC		= 
ARFLAGS		= ruvZ
# When libraries are archived, a hash index is automatically created so there
# is no need for ranlib
RANLIB		= /usr/bin/true
SOFLAGS		=  -shared
SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
 		  $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o -lACE $(LIBS)
