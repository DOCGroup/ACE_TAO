# Irix 5.3 with GNU C++

CC              = gcc
CXX             = gcc
DLD             = $(CXX)
LD		= $(CXX) 
LDFLAGS         += -L$(WRAPPER_ROOT)/ace
LIBS            += -lstdc++
PIC             = -fpic
RANLIB          = echo
SOFLAGS         = -shared $(CPPFLAGS)
SOBUILD         = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
                  $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o

