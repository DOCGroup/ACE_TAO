#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for the ACE examples/APG directory
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

DIRS  =	Config \
	Containers \
	Logging \
	Misc_IPC \
	Proactor \
	Processes \
	Reactor \
	Shared_Memory \
	Signals \
	Sockets \
	Svc_Config \
	Timers

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU

# The Naming examples use std::list, so don't attempt it with explicit
# templates turned on.
ifeq ($(templates),implicit)
DIRS += Naming
endif
# The following directories needs support from the synchronization 
# primitives in ACE. Do not attempt single threaded builds for the 
# following
ifeq ($(threads),1)
DIRS += Active_Objects \
	Streams \
	Threads \
	ThreadManagement \
	ThreadPools \
	ThreadSafety 
endif
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU
