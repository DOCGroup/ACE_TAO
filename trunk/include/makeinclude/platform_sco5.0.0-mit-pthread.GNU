# $Id$

# Requires MIT pthreads and gcc 2.7.2 installed in /usr/progressive
# to produce libACE.so correctly changed the specs file for gcc 2.7.2
# for not link -lc and -lgcc and other not shared libraries
CC              = gcc
# for threads
# for no threads
#CXX		= gcc -fno-implicit-templates -w -fno-strict-prototypes
# LDFLAGS	+= -L $(ACE_ROOT)/ace -L ./ ${MIT_PTHREADS_DIR}/init/init.o # -z muldefs
# LDLIBS	+= -L $(ACE_ROOT)/ace -L ./ ${MIT_PTHREADS_DIR}/init/init.o -lACE # -z muldefs
LDLIBS	+= -L $(ACE_ROOT)/ace -L ./ -lACE # -z muldefs
CXX		= g++
CFLAGS		+= $(DCFLAGS) -b elf -w -O2
CCFLAGS		+= $(DCCFLAGS) -b elf -O2 -fpic -Xpg4plus -fno-implicit-templates -I ${MIT_PTHREADS_DIR}/include -I /usr/progressive/lib/g++-include
DCFLAGS		+= -gstabs
DCCFLAGS	+= $(DCFLAGS)
DLD             = $(CXX)
LD              = $(CXX)
LIBS            = -L${MIT_PTHREADS_DIR} -lnsl -lpthread
PIC             = -fpic
AR              = ar
ARFLAGS         = ruv
RANLIB          = /bin/true

SOFLAGS = $(CPPFLAGS) -b elf -fpic -G 
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
# SOLINK	= $(SOLINK.cc) -o $@ $(LDFLAGS) $<
PRELIB  = (echo "main() { }" > gcctemp.c && \
          $(COMPILE.cc) -o gcctemp.o gcctemp.c && \
          $(LINK.cc) -o gcctemp gcctemp.o $^ $(LDFLAGS) $(LDLIBS) $(VLDLIBS); \
          status=$$?; exit $$status)
