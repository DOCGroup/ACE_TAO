#----------------------------------------------------------------------------
#
#       $Id$
#
#       Makefile for the Quoter tutorials
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
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

ifeq ($(exceptions),1)

DIRS = \
  idl \
  Simple  \
  Naming_Service

  # The AMI and On_Demand_Activation tutorials both require AMI support.
  ifeq ($(ami),1)
    DIRS += \
      AMI
  endif  # ami

  # Event_Service, RT_Event_Service directories require the
  # PortableServer::POA::ImplicitActivationPolicy, which is not in
  # available in the minimum CORBA configuration.
  #
  # On_Demand_Activation requires ServantLocators and AMI, neither of
  # which is available in the minimum CORBA configuration.
  ifeq ($(minimum_corba),0)
    DIRS += \
      On_Demand_Activation \
      Event_Service \
      RT_Event_Service
  endif  # minimum_corba

include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU

else

$(TARGETS_LOCAL):
	@echo "The Quoter tutorial can only be compiled on platforms with"
	@echo "native C++ exception support.  Please read ../../../TAO-INSTALL.html"
	@echo "You may need to recompile ACE+TAO with \"exceptions=1\""

include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU

endif # exceptions

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
