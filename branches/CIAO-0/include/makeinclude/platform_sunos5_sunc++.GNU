# $Id$

# SunOS 5.x (Solaris 2.x) with Sun C++ 4.2 and 5.x
#
# NOTE: Sun C++ 5.0 users might need to add this to their CCFLAGS:
#   -compat=4 and/or remove -instances=explicit.  Please note that
#   inlining is disabled by default with Sun C++ 5.0.  If you'd like
#   to try enabling it, comment out the "inline = 0" line below.
#
# To enable compilation of 64-bit binaries with Sun CC 5.0, a
# platform_macros.GNU file such as this can be used:
#     fast=1
#     include $(ACE_ROOT)/include/makeinclude/platform_sunos5_sunc++.GNU
#     CCFLAGS += -xtarget=ultra2 -xarch=v9a #### CPU specific!
#     SOFLAGS += -xtarget=ultra2 -xarch=v9a
#     LDFLAGS += -L/opt/SUNWspro/SC5.0/lib/v9 -L/usr/lib/sparcv9
# Please note that the xtarget and xarch settings are specific to
# the target CPU.  The /opt/SUNWspro/ Sun C++ installation directory
# is site-specific.
#
# To build 64-bit binaries with Forte 6, build with the "buildbits=64"
# option (make buildbits=64).
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

ifeq (,$(compat4))
  compat4 = 0
endif
ifeq (,$(debug))
  debug = 1
endif
ifeq (,$(distrib))
  distrib = 0
endif
ifeq (,$(stdcpplib))
  stdcpplib = 1
endif
ifeq (,$(threads))
  threads = 1
endif

tk_reactor = 0
ace_with_x11 = 0

PLATFORM_TK_CPPFLAGS=
PLATFORM_TK_LIBS=-ltk -ltcl
PLATFORM_TK_LDFLAGS=

PLATFORM_X11_CPPFLAGS= -I/usr/openwin/include -I/usr/dt/include
PLATFORM_X11_LIBS =-lX11
PLATFORM_X11_LDFLAGS=

ifeq ($(threads),1)
  CFLAGS += -mt
  LDFLAGS += -mt
endif # threads

CC              = cc
CXX             = CC

CC_VERSION = $(shell $(CXX) -V 2>&1)

ifeq (Compilers 4.2,$(findstring Compilers 4.2,$(CC_VERSION)))
  #### CC 4.2, only, supports -features=castop
  CCFLAGS      += $(CFLAGS) -features=castop

  #### RTTI can coexist with ACE, but it doesn't use it.  To enable,
  #### add rtti=1 to your make invocation.  Sun C++ 4.2, only, supports
  #### this option.
  ifeq (1,$(rtti))
    CCFLAGS += -features=rtti
  else  # ! rtti
    #### ACE_LACKS_RTTI is necessary with -features=castop, but
    #### without -features=rtti, to convert dynamic_cast to static_cast.
    CCFLAGS += -DACE_LACKS_RTTI
  endif # ! rtti

  #### Create template repository to avoid compiler warning.
  TEMPLATE_REPOSITORY = Templates.DB
else  # ! 4.2
  ifeq (C++ 5,$(findstring C++ 5,$(CC_VERSION)))
    #### CC 5.0 or later
    #### Inlining appears to cause link problems with early releases of
    #### CC 5.0.
    ifeq (C++ 5.0,$(findstring C++ 5.0,$(CC_VERSION)))
        inline = 0
    else
      ifeq (C++ 5.1,$(findstring C++ 5.1,$(CC_VERSION)))
        inline = 0
      else
        inline = 1
        exceptions = 1
      endif #!5.1
    endif #!5.0

    CCFLAGS += $(CFLAGS)

    #### If compat=4 is desired, set up for that. There are syntactic diffs
    #### in the compatibility modes, but here all we need to be concerned with
    #### is turning on -compat=4, and not trying to enable the new libraries.
    ifeq ($(compat4),1)
      CCFLAGS += -compat=4 -instances=explicit

      #### RTTI can coexist with ACE, but it doesn't use it.  To enable,
      #### add rtti=1 to your make invocation.  Sun C++ 4.2, only, supports
      #### this option.
      ifeq (1,$(rtti))
        CCFLAGS += -features=rtti
      else  # ! rtti
        #### ACE_LACKS_RTTI is necessary with -features=castop, but
        #### without -features=rtti, to convert dynamic_cast to static_cast.
        CCFLAGS += -DACE_LACKS_RTTI
      endif # ! rtti
    else
      #### Support RTTI by default.  And, use explicit template instantiation
      #### because automatic instantiation appears to be broken, at least
      #### with early releases of CC 5.0.  And, use old iostreams.
      CCFLAGS += -instances=explicit
      ifeq (C++ 5.0,$(findstring C++ 5.0,$(CC_VERSION)))
        ifeq (1,$(stdcpplib))
          CCFLAGS += -library=Cstd
        else
          CCFLAGS += -library=iostream,no%Cstd -DACE_USES_OLD_IOSTREAMS
        endif # stdcpplib
      else
        ifeq (0,$(stdcpplib))
          CCFLAGS += -library=iostream -DACE_USES_OLD_IOSTREAMS
        endif # stdcpplib

        # If 64-bit build is desired, turn that on. If no code gen options
        # are given to Forte 6, the default build is 32-bit, even when
        # building on a 64-bit system. Specifying xarch=v9 generates code
        # that runs on any Sparc v9 (64-bit) processor. If you want to
        # tune this further, you should extract and insert from CCFLAGS
        # in a private platform_macros.GNU file.
        ifeq (64,$(buildbits))
          CCFLAGS += -xarch=v9
          LDFLAGS += -xarch=v9
        endif # buildbits
      endif # CC 5.0
    endif # compat4 requested
  endif # CC 5.0 or later
endif # ! 4.2

DCFLAGS         += -g
DCCFLAGS        += -g
DLD             = $(CXX)


#### Sun CC-specific "Pure virtual finction called" bug fix trigger.
CPPFLAGS       += -DSUN_CC_HAS_PVFC_BUG

#### Exception handling breaks on MP machines prior to Sun C++ 4.2
#### with jumbo patch 104631-02, see ace/config-sunos5.5.h.
#### If you are using a version of Sun C++ prior to 4,2, or haven't
#### applied that jumbo patch to your 4.2, or don't want to pay the
#### performance penalty of exception handling, then use the default
#### of no exceptions.  If you want to enable exception handling,
#### add "exceptions=1" to your "make" invocation.
ifeq (0,$(exceptions))
  CCFLAGS += -noex
endif # ! exceptions

LD              = $(CXX)
ifeq ($(distrib),0)
  LDFLAGS       += -R $(ACE_ROOT)/ace -R./
endif
  LIBS          += -lsocket -ldl -lnsl -lgen -lposix4

LDFLAGS         += -xildoff

OCFLAGS         += -O
PIC             = -KPIC
AR              = CC
ARFLAGS         = -xar -o
RANLIB          = echo
SOFLAGS         = -G $(CPPFLAGS)

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
ifeq (1,$(fast))
  CFLAGS += -fast
  DCFLAGS =
  DCCFLAGS =
  LDFLAGS += -fast
endif # fast
