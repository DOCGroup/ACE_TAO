#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------


DIRS = \
  Simple_Naming \
  Redundant_Naming

ifeq ($(corba_messaging),1)
DIRS += \
  EC_Multiple \
  EC_Throughput \
  EC_Mcast \
  EC_MT_Mcast \
  EC_Custom_Marshal
endif # corba_messaging

DIRS += \
  Property \
  Sched \
  Sched_Conf \
  Time

ifeq ($(corba_messaging),1)
DIRS += \
  Event \
  CosEvent
endif # corba_messaging

DIRS += Concurrency \
  FaultTolerance \
  ImplRepo \
  Trading \
  AVStreams \
  Notify \
  Interoperable_Naming \
  ior_corbaname \
  tests_svc_loader \
  Security \
  Miop \
  LoadBalancing \
  Log

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/macros.GNU

ifeq ($(minimum_corba),0)
    ifneq (Compilers 4.2,$(findstring Compilers 4.2,$(CC_VERSION)))
      ifeq ($(LYNXTARGET),)
        DIRS += InterfaceRepo
      endif # LYNXTARGET
    endif # findstring Compilers 4.2
endif # minimum_corba

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU
