#----------------------------------------------------------------------------
#
# $Id$
#
#       Makefile for all the ACE SSL ``one-button' tests
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

BIN =   Thread_Pool_Reactor_SSL_Test

PSRC=$(addsuffix .cpp,$(BIN))
LDLIBS = -lACE_SSL
CPPFLAGS += -I..

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU

ifdef purify
  #### SIGINT is used by Thread_Manager_Test.
  #### SIGHUP and SIGTERM are used by Signal_Test.
  PURELINK += -ignore-signals=SIGHUP,SIGINT,SIGTERM
endif # purify

include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

# To build multiple executables in the same directory on AIX, it works
# best to wipe out any previously-created tempinc directory.
# The compiler/linker isn't too smart about instantiating templates...
ifdef TEMPINCDIR
COMPILE.cc := $(RM) -rf tempinc; $(COMPILE.cc)
endif

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

ifdef VXWORKS
all: run_test.vxworks

run_test.vxworks: run_test.lst
	run_test.pl -v -Config STATIC -Config CHECK_RESOURCES -o run_test.vxworks
endif

realclean: clean
	-$(RM) libDLL_Test.$(SOEXT) libService_Config_DLL.*
	-$(RM) log/compilations.log*

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
