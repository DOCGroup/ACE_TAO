# $Id$
#
# This is for the HP-UX aC++ compiler.  Most of the difference for threaded
# vs. non-threaded is contained in ace/config-hpux-10.x.h.  However, there
# is a line to verify in this file - if you are using DCE threads, "-lcma"
# needs to be on the SHLIBBUILD line, below.  If you are not using DCE threads,
# then use the SHLIBBUILD without -lcma.
CC		= /bin/cc
CXX		= /opt/aCC/bin/aCC
CFLAGS		+= $(DCFLAGS)
CCFLAGS		+= $(CFLAGS)
DCFLAGS		+= -g -D_THREAD_SAFE -D_REENTRANT
DLD		= $(CXX)
LD		= $(CXX) 
PIC		= +Z
ARFLAGS		= ruv
RANLIB		= echo
LDFLAGS		= -Wl,+s
#LIBS		+=
PRELIB		= $(CXX) -c +inst_close $^
PRELIB_USES_OBJ_ONLY = 1
SOFLAGS		= -b +inst_none -g
SOEXT		= sl
SHLIBBUILD 	= $(SOLINK.cc) -o $@ $(VSHOBJS) $(LDFLAGS) -lxti -lcma
#SHLIBBUILD	= $(SOLINK.cc) -o $@ $(VSHOBJS) $(LDFLAGS) -lxti
SOBUILD     = @echo ""; \
  echo "$(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<"; \
  $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<

# No need to instantiate templates in each .o file - wait and do all of
# them at once with +inst_close
#  echo "$(SOLINK.cc) -o $@ ./$(VSHDIR)$*.o"; \
#  $(SOLINK.cc) -o $@ ./$(VSHDIR)$*.o
