# $Id$

CC              = gcc
CXX		= g++
CFLAGS		+= $(DCFLAGS) -w -O2
CCFLAGS		+= $(CFLAGS) -fno-implicit-templates -fno-strict-prototypes
DCFLAGS		+= -g #### should this be -gstabs?
DLD             = $(CXX)
LD		= $(CXX)
LIBS            = -lsocket
PIC             =
AR              = ar
ARFLAGS         = ruv
RANLIB          = /bin/true

SOFLAGS = $(CPPFLAGS)
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB  = @true
