# $Id$

debug = 1

# First, extract the OS version number.
HPVERS_WORDS := $(subst ., ,$(shell uname -r))
HPUX_VERS       := $(join $(word 2,$(HPVERS_WORDS)),$(word 3,$(HPVERS_WORDS)))
#

CC              = /bin/cc
CXX             = /usr/bin/CC -pta -ti,/bin/true -tr,/bin/true
CFLAGS          = -Aa -z +a1
CCFLAGS         = $(CFLAGS) -D_REENTRANT -DACE_LACKS_PRAGMA_ONCE -DHPUX_VERS=$(HPUX_VERS)
DCFLAGS         += -g
DLD             = /usr/bin/CC
LD              = /usr/bin/CC
PIC             = +Z
ARFLAGS         = ruv
RANLIB          = echo
LDFLAGS         = -L$(ACE_ROOT)/ace -Wl,+s
LIBS            += -lxti
SOFLAGS         = -b
SOEXT           = sl
SOBUILD     = @echo ""; \
  echo "$(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<"; \
  $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
  echo "$(SOLINK.cc) -o $@ $(VSHDIR)$*.o"; \
  $(SOLINK.cc) -o $@ $(VSHDIR)$*.o
