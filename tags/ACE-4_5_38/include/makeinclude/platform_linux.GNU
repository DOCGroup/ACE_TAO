# $Id$

# According to Bryon G. Rigg <bgrigg@opus.bcbnet.com>, this file
# should allow ACE to be built on Linux.

debug = 1
optimize = 1

CC      = gcc
CXX     = g++
CFLAGS  += -Wall -Wpointer-arith -pipe
CCFLAGS += $(CFLAGS) -fno-implicit-templates
DCFLAGS += -g
DLD     = $(CXX)
LD      = $(CXX)
LIBS    += -ldl
OCFLAGS += -O3
PIC     = -fPIC
AR      = ar
ARFLAGS = rsuv
RANLIB  = @true
SOFLAGS = $(CPPFLAGS) -shared
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB  = @true

#### GNU gas has a string limit of 4096 characters.  On Alphas,
#### builds will fail due to running over that limit.  There are
#### at least two workarounds:
#### 1) Change the limit to 8192 characters and rebuild gas.  See
####    ACE-INSTALL.html for more information.
#### 2) Don't use -g when compiling those files.
#### If you're building on an Alpha and you haven't hacked and
#### rebuilt gas, you might need to uncomment the following.
#### ifeq ($(debug),1)
####   SUPPRESS_DASH_G = 1
#### endif # debug
