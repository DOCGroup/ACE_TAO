#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

DIRS  = orbsvcs \
	Naming_Service \
	Scheduling_Service

ifeq ($(corba_messaging),1)
DIRS += Event_Service 
endif

DIRS += Dump_Schedule \
        Notify_Service \
        Time_Service

ifeq ($(corba_messaging),1)
DIRS += CosEvent_Service 
endif

DIRS += Concurrency_Service \
	ImplRepo_Service \
        LifeCycle_Service \
	Logging_Service \
	Trading_Service \
	LoadBalancer \
	TAO_Service \
	PSS \
        tests \
	performance-tests \
        examples

ifeq ($(minimum_corba),0)
    ifneq (Compilers 4.2,$(findstring Compilers 4.2,$(CC_VERSION)))
      ifeq ($(LYNXTARGET),)
        DIRS += IFR_Service
      endif # LYNXTARGET
    endif # findstring Compilers 4.2
endif # minimum_corba

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU
