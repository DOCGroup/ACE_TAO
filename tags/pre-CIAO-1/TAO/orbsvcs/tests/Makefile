#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

DIRS = \
  Simple_Naming \
  EC_Multiple \
  EC_Throughput \
  EC_Mcast \
  EC_Custom_Marshal \
  Property \
  Sched \
  Sched_Conf \
  Time \
  Event \
  CosEvent \
  Concurrency \
  FaultTolerance \
  ImplRepo \
  Trading \
  AVStreams \
  Notify \
  Interoperable_Naming \
  LoadBalancing \
  ior_corbaname \
  tests_svc_loader \
  Security \
  Miop

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
        DIRS += InterfaceRepo
      endif # LYNXTARGET
    endif # findstring Compilers 4.2
  endif # ACE_HAS_GNUG_PRE_2_8
endif # minimum_corba

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU
