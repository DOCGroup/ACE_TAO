# $Id$
#
# Chorus 4.0 with egcs 1.1.2
#
# This version of plaform macros builds upon the previous version
# of chorus.  Many of the macros were the same so inclusion of
# the previous macros file seemed the easiest way to go.
#
# You will probably need to set the following environment variables
# or macros:
#
# CPU               powerpc or sparc or ...
# CLX_CDSPATH       <chorus install path>/4.0/chorus-$CPU/tools
# CLX_CDSVERSION    5.00
# MERGEDIR          <chorus install path>/4.0/chorus-$CPU/kernel
#

include $(ACE_ROOT)/include/makeinclude/platform_chorus.GNU

OS_DIR     = $(CLASSIXDIR)/../os
OS_INCLDIR = $(OS_DIR)/include

#
# Put the new include paths in the front to avoid including
# the wrong version of some of the system header files.
#
INCLDIRS := -I$(INC_DIR)           \
            -I$(OS_INCLDIR)        \
            -I$(OS_INCLDIR)/chorus \
            -I$(OS_INCLDIR)/CC     \
            $(INCLDIRS)

#
# Adding debug symbols makes the binaries too
# large to load into memory.
#DCFLAGS  += -g
#
CCFLAGS  += -DCHORUS_4 -D_POSIX_THREAD_PROCESS_SHARED

#
# Special link process to get the right libraries and 
# build up the static constructors and destructors.
#
POSTLINK= -r $(OS_DIR)/lib/CC/libC.a $(OS_DIR)/lib/libbsd/libbsd.a    \
             $(OS_DIR)/lib/classix/libcx.a                            \
             $(CLASSIXDIR)/lib/classix/libsys.u.a;                    \
        mv $@ $@.xp.o;                                                \
        $(CLX_CDSPATH)/host/bin/mkctors $@.xp.o > $@.ct.s;            \
        $(CXX) -c $@.ct.s;                                            \
        $(RM) $@.ct.s;                                                \
        $(LD) $(LDOPTIONS) -e _start -r -dn -o $@                     \
              $(MERGEDIR)/lib/crt/crth.u.o $(MERGEDIR)/lib/crt/crt0.o \
              $(MERGEDIR)/lib/crt/crti.o $@.xp.o $@.ct.o              \
              $(MERGEDIR)/lib/crt/crtn.o;                             \
        $(RM) $@.xp.o;                                                \
        $(RM) $@.ct.o

