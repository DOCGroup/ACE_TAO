#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

DIRS  = orbsvcs \
        Naming_Service \
        Scheduling_Service \
        Event_Service \
        Dump_Schedule \
        Notify_Service \
        Time_Service \
        CosEvent_Service \
        Concurrency_Service \
	ImplRepo_Service \
        LifeCycle_Service \
	Logging_Service \
	Trading_Service \
	LoadBalancer \
	TAO_Service \
        tests \
	performance-tests \
        examples

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU

ifeq ($(minimum_corba),0)
  ifneq ($(ACE_HAS_GNUG_PRE_2_8),1)
    ifneq (Compilers 4.2,$(findstring Compilers 4.2,$(CC_VERSION)))
      ifeq ($(LYNXTARGET),)
        DIRS += IFR_Service
      endif # LYNXTARGET
    endif # findstring Compilers 4.2
  endif # ACE_HAS_GNUG_PRE_2_8
endif # minimum_corba

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU
