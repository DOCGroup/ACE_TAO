#----------------------------------------------------------------------------
#	$Id$
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INFO    = README

LIB     = libTQTD.a
SHLIB   = libTQTD.$(SOEXT)

BIN     = main_async \
	  main_reactor \
	  main_thread

SRC   	= $(addsuffix .cpp,$(BIN))
OBJ   	= $(SRC:%.cpp=$(VDIR)%.o)

LSRC  	= Async_Timer_Queue_Test.cpp \
	Reactor_Timer_Queue_Test.cpp \
	Thread_Timer_Queue_Test.cpp \
	Driver.cpp

LDLIBS 	= -lTQTD

VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD = $(VLIB) $(VSHLIB) $(SHLIBA) $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include		$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include		$(ACE_ROOT)/include/makeinclude/macros.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include         $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

