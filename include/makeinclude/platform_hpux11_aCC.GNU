# $Id$
#
# This is for the HP-UX 11.x using the aC++ compiler and kernel threads.

debug = 1

# First, extract the OS version number.
HPVERS_WORDS := $(subst ., ,$(shell uname -r))
HPUX_VERS	:= $(join $(word 2,$(HPVERS_WORDS)),$(word 3,$(HPVERS_WORDS)))
#
CC		= /bin/cc
CXX		= /opt/aCC/bin/aCC

# If building for 64-bit, add +DA2.0W to CCFLAGS
CCFLAGS		+= $(CFLAGS) -D_THREAD_SAFE -D_REENTRANT -DHPUX_VERS=$(HPUX_VERS)
DCFLAGS		+= -g
DLD		= $(CXX)
LD		= $(CXX) 
PIC		= +Z
ARFLAGS		= ruv
RANLIB		= echo
LDFLAGS		= -Wl,+s
PRELIB		= $(CXX) -c -g +inst_close $^
PRELIB_USES_OBJ_ONLY = 1
SOFLAGS		= -b +inst_none -g
SOEXT		= sl
#
# If building for 64 bits (whether building on 32 or 64 bit) replace
# -ldld with -ldl
SHLIBBUILD 	= $(SOLINK.cc) -o $@ $(VSHOBJS) $(LDFLAGS) -lxti -lpthread -lrt -ldld
SOBUILD     = @echo ""; \
  echo "$(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<"; \
  $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<
