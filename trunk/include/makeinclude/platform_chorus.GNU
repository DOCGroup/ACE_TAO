# Chorus 3.1b with GNU g++ 2.7.2
# *not* using Orbix
#
# Note: Change CLASSIXDIR to an appropriate directory

CLASSIXDIR	= /usr/home/chiang/chorus/merge/extended-i386at
INC_DIR         = $(CLASSIXDIR)/include
BIN_DIR		= 
LIB_DIR		= $(CLASSIXDIR)/lib

CC		= gcc -w
CXX		= g++
		
CFLAGS		+= -w
CCFLAGS		+= -w -fno-strict-prototypes     	\
		   -D_POSIX_THREADS 			\
		   -D_POSIX_THREAD_SAFE_FUNCTIONS

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
		
LIBS		+= -lC -lcx.u
PIC		= -fPIC
AR		= ar
ARFLAGS		= rv
RANLIB		= /bin/true 
PRELIB	= (echo "main() { }" > gcctemp.c && \
	  $(COMPILE.cc) -o gcctemp.o gcctemp.c && \
	  $(LD) $^ $(LDFLAGS) $(LIBS); \
	  status=$$?; rm -f gcctemp.* a.out; exit $$status)

#### don't build shared objects:
BUILD           = $(VOBJS) $(VLIB) $(VBIN)
