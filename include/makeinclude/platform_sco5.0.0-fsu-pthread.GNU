# $Id$

# Requires FSU pthreads and gcc 2.7.2 installed in /usr/progressive
# to produce libACE.so correctly changed the specs file for gcc 2.7.2
# for not link -lc and -lgcc and other not shared libraries

debug = 1
optimize = 1

CC              = gcc
# for threads
# for no threads
#CXX		= gcc -fno-implicit-templates -w -fno-strict-prototypes
# LDFLAGS	+= -L $(ACE_ROOT)/ace -L ./ ${FSU_PTHREADS_DIR}/init/init.o # -z muldefs
# LDLIBS	+= -L $(ACE_ROOT)/ace -L ./ ${FSU_PTHREADS_DIR}/init/init.o -lACE # -z muldefs
LDLIBS	+= -L $(ACE_ROOT)/ace # -z muldefs
CXX		= g++
CFLAGS		+= -b elf -w
CCFLAGS		+= -b elf -fpic -Xpg4plus -fno-implicit-templates -I ${FSU_PTHREADS_DIR}/include -I /usr/progressive/lib/g++-include
DCFLAGS		+= -gstabs
DCCFLAGS	+= $(DCFLAGS)
DLD             = $(CXX)
LD              = $(CXX)
LIBS            = -L${FSU_PTHREADS_DIR}/lib -lnsl -lsocket -lgthreads
OCFLAGS		+= -O2
OCCFLAGS        += $(OCFLAGS)
PIC             = -fpic
AR              = ar
ARFLAGS         = ruv
RANLIB          = /bin/true

SOFLAGS = $(CPPFLAGS) -b elf -fpic -G 
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
# SOLINK	= $(SOLINK.cc) -o $@ $(LDFLAGS) $<
PRELIB  = (echo "main() { }" > gcctemp.c && \
          $(COMPILE.cc) -o gcctemp.o gcctemp.c && \
          $(LINK.cc) -o gcctemp gcctemp.o $^ $(LDFLAGS) $(LDLIBS) $(LIBS); \
          status=$$?; exit $$status)
