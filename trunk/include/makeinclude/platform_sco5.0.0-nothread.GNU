# According to Bryon G. Rigg <bgrigg@opus.bcbnet.com>, this file
# should allow ACE to be built on Linux.

#CC              = gcc -w
CC              = gcc  -b elf -w
# for threads
# for no threads
#CXX		= gcc -fno-implicit-templates -w -I. -fno-strict-prototypes -D__ACE_INLINE__
CXX		= gcc -b elf -Xpg4plus -fno-implicit-templates -I ${MITOSYS_DIR}/socketrpc/netsel -I ${MITOSYS_DIR}/socketrpc -I ${MITOSYS_DIR}/librerias/include -I/usr/local/lib/g++-include -I. -D__ACE_INLINE__
DLD             = $(CXX)
LIBS            = -lstdc++ -lsocket
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
