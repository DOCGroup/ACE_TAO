#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

DIRS  =	orbsvcs \
	Naming_Service \
	Scheduling_Service \
	Event_Service \
	Dump_Schedule \
	Time_Service \
	CosEvent_Service \
	tests \
	examples

# Not compiled by default
#	Concurrency_Service \


#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

ifndef minimum_corba

# ImplRepo uses AdapterActivator which is disabled when compiling with 
# minimum CORBA support
  DIRS += ImplRepo_Service

# Trader uses DynAny which is disabled when compiling with minimum
# CORBA support.  LifeCycle and AV depend on Trader so don't build
# them either.
  DIRS += LifeCycle_Service Trading_Service

endif # minimum_corba

include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU
