# $Id$

# Chorus 3.1b with GNU g++ 2.7.2

CHORUS = 1
CROSS-COMPILE = 1

debug = 1
optimize = 1
shared_libs =
static_libs = 1

#
# The following file includes definitions such as $(CC), $(XDIR), etc.
#
include         $(MERGEDIR)/dtool/htgt-cf.rf
#
# Tool PATH
#
TOOLDIR         = $(XDIR)/bin/
INC_DIR         = $(CLASSIXDIR)/include
BIN_DIR         =
LIB_DIR         = $(CLASSIXDIR)/lib
#CC             = gcc
#CXX            = g++
CXX             = $(CC)
CFLAGS          += -w
CCFLAGS         += $(CFLAGS) -fno-implicit-templates    \
                   -fno-strict-prototypes               \
                   -nostdinc                            \
                   -D_POSIX_THREADS                     \
                   -D_POSIX_THREAD_SAFE_FUNCTIONS
DCFLAGS         += -gstabs+
INCLDIRS        += -I$(INC_DIR)/posix                   \
                   -I$(INC_DIR)/stdc                    \
                   -I$(INC_DIR)/chorus/                 \
                   -I$(INC_DIR)/chorus/iom              \
                   -I$(INC_DIR)/CC
DLD             =
LD              = $(CXX)
LDFLAGS         += -r -Ur -nostdlib                     \
                   -L$(LIB_DIR)/CC                      \
                   -L$(LIB_DIR)/classix

OCFLAGS         += -O2
PIC             = -fPIC

AR              = $(TOOLDIR)ar
ARFLAGS         = srv

RANLIB          = $(TOOLDIR)ranlib
PRELIB          = @true
