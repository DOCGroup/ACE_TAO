# Irix 5.3 with GNU C++

CC              = gcc
CXX             = g++
CFLAGS		+= $(DCFLAGS)
CCFLAGS		+= $(CFLAGS) -O2 -fno-implicit-templates
DCFLAGS		+= -g
DLD             = $(CXX)
LD		= $(CXX) 
LDFLAGS         += -L$(WRAPPER_ROOT)/ace
PIC             = -fpic
RANLIB          = echo
SOFLAGS         = -shared $(CPPFLAGS)
SOBUILD         = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
                  $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o

