CC              = gcc
CXX		= g++
CFLAGS		+= -w -O2
CCFLAGS		+= -w -O2 -fno-implicit-templates -fno-strict-prototypes
DLD             = $(CXX)
LD		= $(CXX)
LIBS            = -lsocket
PIC             =
AR              = ar
ARFLAGS         = ruv
RANLIB          = /bin/true

SOFLAGS = $(CPPFLAGS)
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB  = (echo "int main() { return 0; }" > gcctemp.c && \
          $(COMPILE.cc) -o gcctemp.o gcctemp.c && \
          $(LINK.cc) -o gcctemp gcctemp.o $^ $(LDFLAGS) $(LIBS); \
          status=$$?; exit $$status)
