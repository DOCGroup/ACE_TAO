#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

DIRS  = Simple_Naming \
        Event_Latency \
        EC_Multiple \
        EC_Throughput \
        EC_Mcast \
        EC_Custom_Marshal \
        EC_Basic \
        Property \
        CosEC_Basic \
        CosEC_Multiple \
        Sched \
        Sched_Conf \
        Time \
        Event \
	CosEvent \
	Concurrency

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

ifeq ($(minimum_corba),0)

# ImplRepo uses AdapterActivator which is disabled when compiling with
# minimum CORBA support.
  DIRS += ImplRepo

# Trader uses DynAny which is disabled when compiling with minimum
# CORBA support.  LifeCycle and AV depend on Trader so don't build
# them either.
  DIRS += Trading #AVStreams

endif # minimum_corba

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU
