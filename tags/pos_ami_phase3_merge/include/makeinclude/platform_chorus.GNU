# $Id$

# Chorus 3.1b with GNU g++ 2.7.2

CHORUS = 1
CROSS-COMPILE = 1

debug = 1
exceptions =
optimize = 1
shared_libs =
static_libs = 1

#
# The following file includes definitions such as $(CC), $(XDIR), etc.
#
include         $(MERGEDIR)/dtool/htgt-cf.rf
CLASSIXDIR=$(MERGEDIR)
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
                   -Usun                                \
                   -fno-builtin                         \
                   -fconserve-space                     \
                   -fcheck-new                          \
                   -nostdinc                            \
                   -pipe                                \
                   -fno-rtti                            \
                   -D_POSIX_THREADS                     \
                   -D_POSIX_THREAD_SAFE_FUNCTIONS

#DCFLAGS         += -gstabs+
INCLDIRS        += -I$(INC_DIR)/posix                   \
                   -I$(INC_DIR)/stdc                    \
                   -I$(INC_DIR)/chorus/                 \
                   -I$(INC_DIR)/chorus/iom              \
                   -I$(INC_DIR)/CC
DLD             =
LD              = $(XDIR)/bin/ld
LDFLAGS         += -r $(CLASSIXDIR)/lib/CC/libC.a $(CLASSIXDIR)/lib/classix/libcx.u.a
MATHLIB         = -L$(CLASSIXDIR)/lib/libm -lm

LINK.cc.override        =       $(LD) -u _main -u _premain
LINK.cc=override

CHORUSLINK=true

#       $(CLASSIXDIR)/dtool/mkctors helloCxx_u.xp.o > helloCxx_u.ct.s
#       $(CXX) -c helloCxx_u.ct.s
#       $(RM) helloCxx_u.ct.s
#       $(LD) $(LDOPTIONS) -e _start -o helloCxx_u \
#       $(CLASSIXDIR)/lib/crt/crth.u.o $(CLASSIXDIR)/lib/crt/crt0.o $(CLASSIXDIR)/lib/crt/crti.o helloCxx_u.xp.o helloCxx_u.ct.o $(CLASSIXDIR)/lib/crt/crtn.o \
#       -dn -T $(CLASSIXDIR)/src/act/slots/act_u.ld
#       $(RM) helloCxx_u.xp.o
#       $(RM) helloCxx_u.ct.o


OCFLAGS         += -O
PIC             = -fPIC

AR              = $(TOOLDIR)ar
ARFLAGS         = srv

RANLIB          = $(TOOLDIR)ranlib
PRELIB          = @true
