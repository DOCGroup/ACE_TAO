# SCO v5.0.2; gcc version 2.7-95q4(Skunkware96 distribution from media images)

CC              = gcc
CXX		= g++
CFLAGS		+= -b elf -w -O2
CCFLAGS		+= -b elf -w -O2 -Xpg4plus -fno-implicit-templates \
		   -I/usr/progressive/lib/g++-include
DLD             = $(CXX)
LD		= $(CXX)
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
