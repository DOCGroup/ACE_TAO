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

# Build POSIX_SP_Test conditionally. For some platforms, it doesn't build
# (which is actually the test of support for the #defines). 
BIN =	Cond_Var_Test \
	Pthread_Storage_Test \
	Pthreads_Test \
	Stdio_Test \
	Stat_Test

BIN2 =  mqueue_test \
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

# Don't include Posix_SP_Test and mqueue_test for a VxWorks build.
# Posix_SP_Test doesn't compile (which is the test - seeing if the symbols
# are available). So VxWorks obviously doesn't pass this test.
# SA_RESTART is not defined for VxWorks so mqueue_test doesn't build
# on that platform.
ifdef VXWORKS
  ACELIB += -lPACE
  LDFLAGS += -L$(PACE_ROOT)/pace/
else
  BIN += $(BIN2)
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


.obj/Cond_Var_Test.o .obj/Cond_Var_Test.o .obj/Cond_Var_Test.o .obj/Cond_Var_Test.o: Cond_Var_Test.c \
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
 $(ACE_ROOT)/PACE/pace/stdlib.h \
 $(ACE_ROOT)/PACE/pace/unistd.h \
 $(ACE_ROOT)/PACE/pace/pthread.h

.obj/mqueue_test.o .obj/mqueue_test.o .obj/mqueue_test.o .obj/mqueue_test.o: mqueue_test.c \
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

.obj/Pthread_Storage_Test.o .obj/Pthread_Storage_Test.o .obj/Pthread_Storage_Test.o .obj/Pthread_Storage_Test.o: Pthread_Storage_Test.c \
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
 $(ACE_ROOT)/PACE/pace/stdlib.h \
 $(ACE_ROOT)/PACE/pace/unistd.h \
 $(ACE_ROOT)/PACE/pace/time.h \
 $(ACE_ROOT)/PACE/pace/pthread.h

.obj/Pthreads_Test.o .obj/Pthreads_Test.o .obj/Pthreads_Test.o .obj/Pthreads_Test.o: Pthreads_Test.c \
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
 $(ACE_ROOT)/PACE/pace/stdlib.h \
 $(ACE_ROOT)/PACE/pace/unistd.h \
 $(ACE_ROOT)/PACE/pace/pthread.h

.obj/Stdio_Test.o .obj/Stdio_Test.o .obj/Stdio_Test.o .obj/Stdio_Test.o: Stdio_Test.c \
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
 $(ACE_ROOT)/PACE/pace/string.h

.obj/Stat_Test.o .obj/Stat_Test.o .obj/Stat_Test.o .obj/Stat_Test.o: Stat_Test.c $(ACE_ROOT)/PACE/pace/stdio.h \
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
 $(ACE_ROOT)/PACE/pace/fcntl.h \
 $(ACE_ROOT)/PACE/pace/sys/stat.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
