# Requires MIT pthreads and gcc 2.7.2 installed in /usr/progressive
# to produce libACE.so correctly changed the specs file for gcc 2.7.2
# for not link -lc and -lgcc and other not shared libraries
#CC              = gcc -w
CC              = gcc  -b elf -w
# for threads
# for no threads
#CXX		= gcc -fno-implicit-templates -w -I. -fno-strict-prototypes -D__ACE_INLINE__
# LDFLAGS	+= -L $(WRAPPER_ROOT)/ace -L ./ ${MIT_PTHREADS_DIR}/init/init.o # -z muldefs
# LDLIBS	+= -L $(WRAPPER_ROOT)/ace -L ./ ${MIT_PTHREADS_DIR}/init/init.o -lACE # -z muldefs
LDLIBS	+= -L $(WRAPPER_ROOT)/ace -L ./ -lACE # -z muldefs
CXX		= gcc -b elf -fpic -Xpg4plus -fno-implicit-templates -I. -I ${MIT_PTHREADS_DIR}/include -I /usr/progressive/lib/g++-include -D__ACE_INLINE__
DLD             = $(CXX)
LIBS            = -L${MIT_PTHREADS_DIR} -lstdc++ -lm -lnsl -lpthread
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
