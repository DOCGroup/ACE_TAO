#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for the ACE examples/APG directory
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

DIRS  =	Active_Objects \
	Config \
	Containers \
	Logging \
	Misc_IPC \
	Proactor \
	Processes \
	Reactor \
	Shared_Memory \
	Signals \
	Sockets \
	Streams \
	Svc_Config \
	Threads \
	ThreadManagement \
	ThreadPools \
	ThreadSafety \
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

include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU
