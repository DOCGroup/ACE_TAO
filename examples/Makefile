#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for the ACE examples directory
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INFO  = README

DIRS  =	ASX \
	Connection \
	IPC_SAP \
	IOStream \
	Logger \
	Log_Msg \
	Map_Manager \
	Mem_Map \
	Misc \
	Naming \
	Reactor	\
	Service_Configurator \
	Shared_Malloc \
	Shared_Memory \
	System_V_IPC \
	Synch \
	Threads \
	Timer_Queue

# Makefiles TBD:
#	ttcp
#       CORBA


#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

ifneq ($(ACE_HAS_GNUG_PRE_2_8),1)
  ifneq ($(rmcast),0)
    DIRS += RMCast
  endif # rmcast
endif # ACE_HAS_GNUG_PRE_2_8

include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU
