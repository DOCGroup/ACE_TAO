# $Id$

# Chorus 3.1b with GNU g++ 2.7.2

CHORUS = 1
CROSS-COMPILE = 1

debug = 1
exceptions = 0
optimize = 1
shared_libs =
static_libs = 1

#
# Directory where the Chorus Development System is installed
#
ifndef CLX_CDSPATH
        CLX_CDSPATH = /vob/k_bbansw/chorus_r321_bin/CDS
endif #CLX_CDSPATH
ifndef CLX_CDSVERSION
        CLX_CDSVERSION = 4.3
endif #CLX_CDSVERSION
ifndef CLX_CDSHOST
        CLX_CDSHOST = solaris
endif #CLX_CDSHOST
ifndef CLASSIXDIR
        CLASSIXDIR=$(MERGEDIR)
endif #CLASSIXDIR
ifndef CPU
        CPU=powerpc
endif

XROOT = $(CLX_CDSPATH)/$(CPU)/$(CLX_CDSHOST)/$(CLX_CDSVERSION)
XDIR = $(XROOT)/$(CPU)-elf
CC = $(XDIR)/bin/gcc -B$(XROOT)/lib/gcc-lib/

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
                   -fwritable-strings                   \
                   -fsigned-char                        \
                   "-msoft-float"                       \
                   -D_POSIX_THREADS                     \
                   -D_POSIX_THREAD_SAFE_FUNCTIONS	\
                   "-DNO_FPU_SUPPORT"

ifeq ($(CPU),powerpc)
CCFLAGS         += -mstrict-align                       \
                   -mno-sdata
endif

ifeq ($(exceptions),1)
else  # ! exceptions
  CCFLAGS += -fno-exceptions
endif # ! exceptions     

#DCFLAGS         += -gstabs+
INCLDIRS        += -I$(INC_DIR)/posix                   \
                   -I$(INC_DIR)/stdc                    \
                   -I$(INC_DIR)/chorus/                 \
                   -I$(INC_DIR)/chorus/iom              \
                   -I$(INC_DIR)/CC
DLD             =
LD              = $(XDIR)/bin/ld
#LDFLAGS         += -r $(CLASSIXDIR)/lib/CC/libC.a $(CLASSIXDIR)/lib/classix/libcx.u.a
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

POSTLINK= -r $(CLASSIXDIR)/lib/CC/libC.a $(CLASSIXDIR)/lib/classix/libcx.u.a; \
	mv $@ $@.xp.o;					\
        $(CLASSIXDIR)/dtool/mkctors $@.xp.o > $@.ct.s;	\
        $(CXX) -c $@.ct.s;				\
        $(RM) $@.ct.s;					\
        $(LD) $(LDOPTIONS) -e _start -o $@ $(MERGEDIR)/lib/crt/crth.u.o $(MERGEDIR)/lib/crt/crt0.o $(MERGEDIR)/lib/crt/crti.o $@.xp.o $@.ct.o $(MERGEDIR)/lib/crt/crtn.o -dn -T $(MERGEDIR)/src/act/slots/act_u.ld;	\
        $(RM) $@.xp.o;					\
        $(RM) $@.ct.o   
	

OCFLAGS         += -O
PIC             = -fPIC

AR              = $(TOOLDIR)ar
ARFLAGS         = srv

RANLIB          = $(TOOLDIR)ranlib
PRELIB          = @true
