# According to Bryon G. Rigg <bgrigg@opus.bcbnet.com>, this file
# should allow ACE to be built on Linux.

CC              = gcc -w
CXX		= g++ -w -I. -fno-strict-prototypes 
DLD             = $(CXX)
LD		= $(CXX) 
LIBS            = -lstdc++
PIC             = -fPIC
AR              = ar
ARFLAGS         = ruv
RANLIB          = ranlib

SOFLAGS = $(CPPFLAGS) -shared
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB  = (echo "int main() { return 0; }" > gcctemp.c && \
          $(COMPILE.cc) -o gcctemp.o gcctemp.c && \
          $(LINK.cc) -o gcctemp gcctemp.o $^ $(LDFLAGS) $(LIBS); \
          status=$$?; exit $$status)
