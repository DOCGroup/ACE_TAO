# $Id$

# SunOS 5.x (Solaris 2.x) with SunC++ 4.2 or earlier
# *not* using Orbix
#
# Note: some ACE files might generate these kinds of warnings:
# 1) "Warning: Could not find source for " one of the following:
#    default constructor, copy constructor, destructor, or assignment
#    operator.  It appears that with +w, Sun C++ 4.x issues this warning
#    whenever it has to generate one of these functions (because it wasn't
#    defined by the user).  This isn't really a problem.
#    This warning appears to be fixed by Sun C++ 4.2 jumbo patch 104631-02.
# 2) "Warning: <foo> hides the function " or "hides the same name in an
#    outer scope".  A useful warning, perhaps, but not really a problem.
# There should be no other warnings from Sun C++ when building ACE.

debug = 1

CC              = cc
CXX             = CC
CFLAGS          += -mt
#### -pta instantiates all template members, which makes libraries bigger.
#### But, it's supposed to put each member into it's own .o, so executable
#### size should not be penalized.
CCFLAGS         += $(CFLAGS) $(EXFLAGS) -pta
DCFLAGS         += -g
DLD             = $(CXX)

#### Exception handling breaks on MP machines prior to Sun C++ 4.2
#### with jumbo patch 104631-02, see ace/config-sunos5.5-sunc++-4.x.h.
#### If you are using a version of Sun C++ prior to 4,2, or haven't
#### applied that jumbo patch to your 4.2, or don't want to pay the
#### performance penalty of exception handling, then use the default
#### of no exceptions.  If you want to enable exception handling,
#### add "exceptions=1" to your "make" invocation.
ifdef exceptions
EXFLAGS += -DACE_HAS_EXCEPTIONS
else
EXFLAGS += -noex
endif # exceptions

LD              = $(CXX)

ifdef orbix
  LDFLAGS       += $(PIC) -L$(ORBIX_ROOT)/corba2/lib -R $(ACE_ROOT)/ace -R $(ORBIX_ROOT)/corba2/lib
  LIBS          += -lITinimt -liiopmt -lorbixmt -lsocket -ldl -lnsl -lthread -lgen -lposix4
  CPPFLAGS	+= -I$(ORBIX_ROOT)/corba2/include
else
  LDFLAGS       += -R $(ACE_ROOT)/ace
  LIBS          += -lsocket -ldl -lnsl -lgen
endif # orbix

OCFLAGS         += -O
PIC             = -PIC
AR              = CC
ARFLAGS         = -xar -o
RANLIB          = echo
SOFLAGS         = -G $(CPPFLAGS)

#### Create these directories to avoid compiler warnings.
TEMPLATE_REPOSITORY = Templates.DB Templates.DB/Modules.DB

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
  SOBUILD = /bin/rm -f $@; \
           $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $< && /bin/ln $(VSHDIR)$*.o $@
endif # BIN

#### The following macro overrides enable creation of fast executables.
#### They _don't_ support fast compilation :-)
#### To use, add fast=1 to your "make" invocation.
####
#### -g is incompatible with -fast.  If you need -g, you can
#### use "-fast -O3".
####
#### Dynamic casting and RTTI can coexist with ACE, but it doesn't
#### use them.  To disable, add: -features=no%castop,no%rtti
#### Sun C++ 4.2 (and later?) only support this option.
ifdef fast
  CFLAGS += -fast
  CCFLAGS += -features=no%castop,no%rtti
  DCFLAGS =
endif # fast
