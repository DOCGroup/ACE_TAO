# $Id$  -*- Makefile -*-

# SunOS 5.x (Solaris 2.x) with KAI C++ 3.3e, *not* using Orbix.
#

debug = 1
distrib = 0
threads = 1

ifeq ($(threads),1)
  CFLAGS += --thread-safe
  LDFLAGS += --thread-safe
endif # threads

CC              = KCC
CXX             = KCC

DCFLAGS         += -g
DLD             = $(CXX)

ifneq ($exceptions,0)
#  CCFLAGS += --exexceptions
endif # ! exceptions

LD              = $(CXX)

LIBS          += -lsocket -ldl -lnsl -lgen -lposix4

OCFLAGS         += -K2
PIC             = -KPIC
AR              = ar
ARFLAGS         = -xar -o
RANLIB          = echo
SOFLAGS         = -G $(CPPFLAGS)

#### Create template repository to avoid compiler warning.
TEMPLATE_REPOSITORY = Templates.DB

ifdef BIN
  #### Builds .shobj/$*.o file, then .shobj/$*.so file.  The .o files
  #### are used for building libraries and executables.  But, it seems
  #### to be necessary to build the .so files (with -G) in order to get
  #### all template instantiations.
  SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
            $(SOLINK.cc) -o $@ -h $@ $(LDFLAGS) $(VSHDIR)$*.o
else
  #### Optimize builds when no executables are built in the current
  #### directory.  Only a library is being created, and -G is used
  #### in that step.  Therefore, all templates instantations are
  #### included in the library.  This optimization saves almost 11 Mb
  #### (2.6 percent) and 27 minutes (22 percent) on the entire ACE
  #### build on a 168 MHz Sun Ultra2.
  ####
  #### Sun C++ won't allow the output from a compile to be named with
  #### a .so extension.  Rather than muck with the ACE build rules and
  #### risk upsetting builds on other platforms, just ln the output
  #### after building it.
  SOBUILD = $(RM) $@; $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $< && \
                      /bin/ln $(VSHDIR)$*.o $@
endif # BIN

#### The following macro overrides enable creation of fast executables.
#### They _don't_ support fast compilation :-)
#### To use, add fast=1 to your "make" invocation.
####
#### -g is incompatible with -fast.  If you need -g, you can
#### use "-fast -O3".
####
ifdef fast
  CFLAGS += -fast +K3
  DCFLAGS =
  LDFLAGS += -fast +K3
endif # fast
