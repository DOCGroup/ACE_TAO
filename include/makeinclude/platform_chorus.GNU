# Chorus 3.1b with GNU g++ 2.7.2

include         $(CLASSIXDIR)/dtool/htgt-cf.rf
INC_DIR         = $(CLASSIXDIR)/include
BIN_DIR		= 
LIB_DIR		= $(CLASSIXDIR)/lib
#CC		= gcc
#CXX		= g++
CXX             = $(CC)
CFLAGS		+= $(DCFLAGS) -w -O2
CCFLAGS		+= $(CFLAGS) -fno-implicit-templates	\
		   -fno-strict-prototypes     		\
		   -nostdinc 				\
		   -D_POSIX_THREADS 			\
		   -D_POSIX_THREAD_SAFE_FUNCTIONS
DCFLAGS		+= -g
INCLDIRS	+= -I$(INC_DIR)/posix			\
		   -I$(INC_DIR)/stdc			\
		   -I$(INC_DIR)/chorus/ 		\
		   -I$(INC_DIR)/chorus/iom		\
		   -I$(INC_DIR)/CC
DLD             =
LD              = $(CXX)
LDFLAGS		+= -r -Ur -nostdlib			\
		   -L$(LIB_DIR)/CC			\
		   -L$(LIB_DIR)/classix
#LIBS		+= -lC -lcx.u
PIC		= -fPIC
#AR		= ar
AR		= $(XDIR)/bin/ar
#ARFLAGS		= rv
ARFLAGS		= srv
#RANLIB		= /bin/true
RANLIB		= $(XDIR)/bin/ranlib
PRELIB	= (echo "main() { }" > gcctemp.c && \
	  $(COMPILE.cc) -o gcctemp.o gcctemp.c && \
	  $(LD) $^ $(LDFLAGS) $(LIBS); \
	  status=$$?; rm -f gcctemp.* a.out; exit $$status)
#### don't build shared objects:
BUILD           = $(VOBJS) $(VLIB) $(VBIN)

