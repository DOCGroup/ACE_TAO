# platform_macosx.GNU,v 4.6 2001/08/14 18:57:28 bala Exp
# platform_macosx.GNU
# preliminary support for Mac OS X DP4, some stuff still doesn't work.

CC              = cc
CXX             = c++  
CFLAGS          += -Wall -Wpointer-arith -pipe 
CCFLAGS         += $(CFLAGS) -fimplicit-templates -fcheck-new 
DCFLAGS         += -g
DLD             = libtool
LD              = $(CXX)
LDFLAGS		+= -flat_namespace
LDONLYFLAGS     += -flat-namespace
LIBS            += -lcc_dynamic -lstdc++ -lpcap
OCFLAGS         += -O3
RANLIB          = ranlib
SOEXT		= dylib
SOFLAGS = -dynamic
SOBUILD = -o $(VSHDIR)$*.dylib $<


