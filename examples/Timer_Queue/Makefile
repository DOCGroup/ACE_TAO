#----------------------------------------------------------------------------
#	$Id$
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INFO  = README

BIN   = main_async 
FILES   = Async_Timer_Queue_Test
LSRC    = $(addsuffix .cpp,$(FILES))
LOBJ    = $(addsuffix .o,$(FILES))
BUILD   = $(VBIN)

BIN     = main_reactor
FILES   = Reactor_Timer_Queue_Test 
LSRC    = $(addsuffix .cpp,$(FILES))
LOBJ    = $(addsuffix .o,$(FILES))
BUILD   = $(VBIN)

BIN     = main_thread
FILES   = Thread_Timer_Queue_Test 
LSRC    = $(addsuffix .cpp,$(FILES))
LOBJ    = $(addsuffix .o,$(FILES))
BUILD = $(VBIN)

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

