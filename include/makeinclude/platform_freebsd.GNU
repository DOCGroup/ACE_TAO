# platform_freebsd.GNU

# ***** Notice: This file is still under testing. *****
# To be used on FreeBSD platform without pthread support.

CC              = gcc -w
CXX		= gcc -w -I. -fno-strict-prototypes -D__ACE_INLINE__
DLD             = ld
LD		= $(CXX) 
LIBS            += -lstdc++ -lcompat
PIC             = -fpic -DPIC
AR              = ar
ARFLAGS         = ruv
RANLIB          = ranlib

SOFLAGS = -Bshareable -x
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB  = (echo "int main() { return 0; }" > gcctemp.c && \
          $(COMPILE.cc) -o gcctemp.o gcctemp.c && \
          $(LINK.cc) -o gcctemp gcctemp.o $^ $(LDFLAGS) $(LIBS); \
          status=$$?; exit $$status)
