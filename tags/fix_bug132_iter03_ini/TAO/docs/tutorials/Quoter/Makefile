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

DIRS  = idl \
        Simple  \
        AMI \
        Naming_Service \
        On_Demand_Activation \
        Event_Service \
        RT_Event_Service 

include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU

else

$(TARGETS_LOCAL):
	@echo "The Quoter tutorial can only be compiled on platforms with"
	@echo "native C++ exception support, please read ../../../TAO-INSTALL.html"
	@echo "You may need to recompile ACE+TAO with exceptions=1"

include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU

endif # exceptions

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
