# $Id$
#
# First, extract the OS version number.
HPVERS_WORDS := $(subst ., ,$(shell uname -r))
HPUX_VERS	:= $(join $(word 2,$(HPVERS_WORDS)),$(word 3,$(HPVERS_WORDS)))
#

CC		= /bin/cc
CXX		= /usr/bin/CC -pta -ti,/bin/true -tr,/bin/true
CFLAGS		= $(DCFLAGS) -Aa -z
CCFLAGS		= $(CFLAGS)
DCFLAGS		+= -g -DHPUX_VERS=$(HPUX_VERS)
DLD		= /usr/bin/CC
LD		= /usr/bin/CC
PIC		= +Z
ARFLAGS		= ruv
RANLIB		= echo
LDFLAGS		= -L$(ACE_ROOT)/ace -Wl,+s
LIBS		+= -lxti
SOFLAGS		= -b
SOEXT		= sl
SOBUILD     = @echo ""; \
  echo "$(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<"; \
  $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
  echo "$(SOLINK.cc) -o $@ $(VSHDIR)$*.o"; \
  $(SOLINK.cc) -o $@ $(VSHDIR)$*.o
