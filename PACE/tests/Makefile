#----------------------------------------------------------------------------
#
# $Id$
#
#       Makefile for all the PACE tests
#----------------------------------------------------------------------------

CFLAGS += -DPACE_HAS_ALL_POSIX_FUNCS

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

# Ensure that POSIX_SP_Test is the last test built. For some platforms,
# it doesn't build (which is actually the test). This is currently the
# only file like this (i.e., which may not compile).
BIN =   Stdio_Test \
	Pthreads_Test \
	mqueue_test \
        Posix_SP_Test

#### If the PACE library wasn't built with all components, don't
#### try to build certain tests.
PACE_BUILD_COMPONENTS := $(shell sh $(ACE_ROOT)/bin/ace_components --pace)

PSRC=$(addsuffix .c,$(BIN))

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
ACELIB =
ifdef VXWORKS
  ACELIB += -lPACE
  LDFLAGS += -L$(PACE_ROOT)/pace/
endif
INCLDIRS += -I$(PACE_ROOT)
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU

ifndef SOEXT
  SOEXT=so
endif

include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

include $(ACE_ROOT)/PACE/include/makeinclude/rules.common.GNU

ifndef static_libs_only
  ifndef shared_libs_only
    static_libs_only = 1
  endif # shared_libs_only
endif # static_libs_only

# If we are inlining the PACE functions then we don't want to include
# the PACE library (and we need to define PACE_HAS_INLINE). If we're not
# inlining then we need to include the PACE library. Inlining is the default.

ifndef inline
  CFLAGS += -DPACE_HAS_INLINE
else
  ifneq (0,$(inline))
    CFLAGS += -DPACE_HAS_INLINE
  else
    LIBS += -L$(ACE_ROOT)/PACE/pace -lPACE
  endif # ! inline
endif # ! inline

# To compile in the PACE lib on platforms that compile c code
ifeq  (1, $(emulation))
  LIBS += -L$(PACE_ROOT)/pace -lPACE
endif # emulation

# To build multiple executables in the same directory on AIX, it works
# best to wipe out any previously-created tempinc directory.
# The compiler/linker isn't too smart about instantiating templates...
ifdef TEMPINCDIR
COMPILE.cc := $(RM) -rf tempinc; $(COMPILE.cc)
endif

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/Stdio_Test.o .obj/Stdio_Test.so .shobj/Stdio_Test.o .shobj/Stdio_Test.so: Stdio_Test.c $(ACE_ROOT)/PACE/pace/stdio.h \
 $(ACE_ROOT)/PACE/pace/config/defines.h \
 $(ACE_ROOT)/PACE/pace/config/platform.h \
 $(ACE_ROOT)/PACE/pace/config/config.h \
 $(ACE_ROOT)/PACE/pace/config/compiler.h \
 $(ACE_ROOT)/PACE/pace/config/constants.h \
 $(ACE_ROOT)/PACE/pace/sys/types.h \
 $(ACE_ROOT)/PACE/pace/config/defaults.h \
 $(ACE_ROOT)/PACE/pace/config/utility.h \
 $(ACE_ROOT)/PACE/pace/errno.h \
 $(ACE_ROOT)/PACE/pace/config/features.h \
 $(ACE_ROOT)/PACE/pace/unistd.h \
 $(ACE_ROOT)/PACE/pace/string.h

.obj/Posix_SP_Test.o .obj/Posix_SP_Test.so .shobj/Posix_SP_Test.o .shobj/Posix_SP_Test.so: Posix_SP_Test.c \
 $(ACE_ROOT)/PACE/pace/stdio.h \
 $(ACE_ROOT)/PACE/pace/config/defines.h \
 $(ACE_ROOT)/PACE/pace/config/platform.h \
 $(ACE_ROOT)/PACE/pace/config/config.h \
 $(ACE_ROOT)/PACE/pace/config/compiler.h \
 $(ACE_ROOT)/PACE/pace/config/constants.h \
 $(ACE_ROOT)/PACE/pace/sys/types.h \
 $(ACE_ROOT)/PACE/pace/config/defaults.h \
 $(ACE_ROOT)/PACE/pace/config/utility.h \
 $(ACE_ROOT)/PACE/pace/errno.h \
 $(ACE_ROOT)/PACE/pace/config/features.h \
 $(ACE_ROOT)/PACE/pace/unistd.h \
 $(ACE_ROOT)/PACE/pace/time.h \
 $(ACE_ROOT)/PACE/pace/sys/utsname.h

.obj/mqueue_test.o .obj/mqueue_test.so .shobj/mqueue_test.o .shobj/mqueue_test.so: mqueue_test.c \
 $(ACE_ROOT)/PACE/pace/stdio.h \
 $(ACE_ROOT)/PACE/pace/config/defines.h \
 $(ACE_ROOT)/PACE/pace/config/platform.h \
 $(ACE_ROOT)/PACE/pace/config/config.h \
 $(ACE_ROOT)/PACE/pace/config/compiler.h \
 $(ACE_ROOT)/PACE/pace/config/constants.h \
 $(ACE_ROOT)/PACE/pace/sys/types.h \
 $(ACE_ROOT)/PACE/pace/config/defaults.h \
 $(ACE_ROOT)/PACE/pace/config/utility.h \
 $(ACE_ROOT)/PACE/pace/errno.h \
 $(ACE_ROOT)/PACE/pace/config/features.h \
 $(ACE_ROOT)/PACE/pace/fcntl.h \
 $(ACE_ROOT)/PACE/pace/mqueue.h \
 $(ACE_ROOT)/PACE/pace/signal.h \
 $(ACE_ROOT)/PACE/pace/stdlib.h \
 $(ACE_ROOT)/PACE/pace/unistd.h \
 $(ACE_ROOT)/PACE/pace/string.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
