CC		= /bin/cc
CXX		= /opt/aCC/bin/aCC
CFLAGS		+= $(DCFLAGS)
CCFLAGS		+= $(CFLAGS)
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
SHLIBBUILD 	= $(SOLINK.cc) -o $@ $(VSHOBJS1)
SOBUILD     = @echo ""; \
  echo "$(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<"; \
  $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
  echo "$(SOLINK.cc) -o $@ ./$(VSHDIR)$*.o"; \
  $(SOLINK.cc) -o $@ ./$(VSHDIR)$*.o
