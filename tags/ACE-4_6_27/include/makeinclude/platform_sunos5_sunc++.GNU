# $Id$

# SunOS 5.x (Solaris 2.x) with Sun C++ 4.2 and 5.x, *not* using Orbix.
#
# NOTE: Sun C++ 5.0 users might need to add this to their CCFLAGS:
#   -compat=4 and/or remove -instances=explicit.  Please note that
#   inlining is disabled by default with Sun C++ 5.0.  If you'd like
#   to try enabling it, comment out the "inline = 0" line below.
#
# With slight modification, this file could be used with Sun C++ 4.1.
# However, it's likely that you won't be able to build all of ACE
# with Sun C++ 4.1.
#
# NOTE: some ACE files might generate this warning:
#   "Warning: Could not find source for " one of the following:
#    default constructor, copy constructor, destructor, or assignment
#    operator.  It appears that with +w, Sun C++ 4.x issues this warning
#    whenever it has to generate one of these functions (because it wasn't
#    defined by the user).  This isn't really a problem.
#    This warning appears to be fixed by Sun C++ 4.2 jumbo patch 104631-02.
#
# -pta instantiates all template members, which makes libraries bigger.
# But, it's supposed to put each member into it's own .o, so executable
# size should not be penalized.  We don't use it because it's not
# necessary with automatic template instantiation.

debug = 1
distrib = 0
threads = 1

ifeq ($(threads),1)
  CFLAGS += -mt 
  LDFLAGS += -mt
endif # threads

CC              = cc
CXX             = CC

CC_VERSION = $(shell CC -V 2>&1)

ifeq (Compilers 4.2,$(findstring Compilers 4.2,$(CC_VERSION)))
  #### CC 4.2, only, supports -features=castop
  CCFLAGS         += $(CFLAGS) -features=castop

  #### RTTI can coexist with ACE, but it doesn't use it.  To enable,
  #### add rtti=1 to your make invocation.  Sun C++ 4.2, only, supports
  #### this option.
  ifdef rtti
    CCFLAGS += -features=rtti
  else  # ! rtti
    #### ACE_LACKS_RTTI is necessary with -features=castop, but
    #### without -features=rtti, to convert dynamic_cast to static_cast.
    CCFLAGS += -DACE_LACKS_RTTI
  endif # ! rtti
else  # ! 4.2
ifeq (Compilers 5,$(findstring Compilers 5,$(CC_VERSION)))
#### CC 5.0 or later
  #### Inlining appears to cause link problems with early releases of
  #### CC 5.0.
  inline = 0
  #### Support RTTI by default.  And, use explicit template instantiation
  #### because automatic instantiation appears to be broken, at least
  #### with early releases of CC 5.0.
  CCFLAGS         += $(CFLAGS) -instances=explicit
endif # CC 5.0 or later
endif # ! 4.2

DCFLAGS         += -g
DLD             = $(CXX)


#### Exception handling breaks on MP machines prior to Sun C++ 4.2
#### with jumbo patch 104631-02, see ace/config-sunos5.5.h.
#### If you are using a version of Sun C++ prior to 4,2, or haven't
#### applied that jumbo patch to your 4.2, or don't want to pay the
#### performance penalty of exception handling, then use the default
#### of no exceptions.  If you want to enable exception handling,
#### add "exceptions=1" to your "make" invocation.
ifndef exceptions
  CCFLAGS += -noex
endif # ! exceptions

LD              = $(CXX)

ifdef orbix
  LDFLAGS       += $(PIC) -L$(ORBIX_ROOT)/corba2/lib \
                   -R $(ACE_ROOT)/ace -R./ -R $(ORBIX_ROOT)/corba2/lib
  LIBS          += -lITinimt -liiopmt -lorbixmt -lsocket -ldl -lnsl \
                   -lthread -lgen -lposix4
  CPPFLAGS      += -I$(ORBIX_ROOT)/corba2/include
else
ifeq ($(distrib),0)
  LDFLAGS       += -R $(ACE_ROOT)/ace -R./
endif
  LIBS          += -lsocket -ldl -lnsl -lgen -lposix4
endif # orbix

LDFLAGS         += -xildoff

OCFLAGS         += -O
PIC             = -PIC
AR              = CC
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
  CFLAGS += -fast
  DCFLAGS =
  LDFLAGS += -fast
endif # fast
