# According to Bryon G. Rigg <bgrigg@opus.bcbnet.com>, this file
# should allow ACE to be built on Linux.

CC              = gcc
# for threads
# for no threads
#CXX		= gcc -fno-implicit-templates -w -fno-strict-prototypes
CXX		= g++
CFLAGS		+= -b elf -w
CCFLAGS		+= -b elf -Xpg4plus -fno-implicit-templates -I ${MITOSYS_DIR}/socketrpc/netsel -I ${MITOSYS_DIR}/socketrpc -I ${MITOSYS_DIR}/librerias/include -I/usr/local/lib/g++-include
DLD             = $(CXX)
LIBS            = -lsocket
PIC             = -fpic -shared
AR              = ar
ARFLAGS         = ruv
RANLIB          = /bin/true

SOFLAGS = $(CPPFLAGS) -b elf -fpic -G
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB  = (echo "main() { }" > gcctemp.c && \
          $(COMPILE.cc) -o gcctemp.o gcctemp.c && \
          $(LINK.cc) -o gcctemp gcctemp.o $^ $(LDFLAGS) $(LIBS); \
          status=$$?; exit $$status)
