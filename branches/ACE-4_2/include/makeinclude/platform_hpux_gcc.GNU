CC		= gcc
CXX		= g++
CFLAGS		+= $(DCFLAGS) -w -O2
CCFLAGS		+= $(CFLAGS) -fno-implicit-templates
DCFLAGS		+= -g
DLD		= $(CXX)
LD		= $(CXX)
PIC		= -fPIC
ARFLAGS		= ruv
RANLIB		= echo
LDFLAGS		= -L$(WRAPPER_ROOT)/ace -Wl,+s
LIBS		+= -lxti
SOFLAGS		= $(CPPFLAGS) -shared -nostdlib
SOEXT		= sl
SOBUILD 	= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
SHLIBBUILD 	= $(SOLINK.cc) -o $@ $(VSHOBJS1)
PRELIB = (echo "main() { }" > gcctemp.c && \
          $(COMPILE.cc) $(PIC) -o gcctemp.o gcctemp.c && \
          $(LINK.cc) -o gcctemp gcctemp.o $^ $(LDFLAGS) $(LIBS); \
          status=$$?; exit $$status)
