CC		= /bin/cc
CXX		= /usr/bin/CC
CFLAGS		= $(DCFLAGS) -Aa -z
CCFLAGS		= $(CFLAGS) -pta -ptb
DCFLAGS		+= -g
DLD		= $(CXX)
LD		= $(CXX) 
PIC		= +Z
ARFLAGS		= ruv
RANLIB		= echo
LDFLAGS		= -L$(WRAPPER_ROOT)/ace -Wl,+s
LIBS		+= -lxti
SOFLAGS		= -b
SOEXT		= sl
SOBUILD     = @echo ""; \
  echo "$(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<"; \
  $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
  echo "$(SOLINK.cc) -o $@ $(VSHDIR)$*.o"; \
  $(SOLINK.cc) -o $@ $(VSHDIR)$*.o
