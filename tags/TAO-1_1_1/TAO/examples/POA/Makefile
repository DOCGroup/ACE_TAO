#----------------------------------------------------------------------------
#
# $Id$
#
# Makefile for the POA tests
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

DIRS  = Generic_Servant \
        RootPOA \
        Reference_Counted_Servant

# The following test hasn't been updated yet
# Demux_Test

#### Does not compile on UNIX:  Forwarding \

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

ifeq ($(minimum_corba),0)

# No AdapterActivator in minimum CORBA
  DIRS += Adapter_Activator

# Some POA policies and NVList unavailable in minimum CORBA
  DIRS += DSI

# Some POA policies and default servants unavailable in minimum CORBA
  DIRS += Default_Servant

# Some POA policies unavailable in minimum CORBA
  DIRS += Explicit_Activation

# No AdapterActivator in minimum CORBA
  DIRS += FindPOA

# No servant managers in minimum CORBA
  DIRS += Forwarding
  DIRS += Loader
  DIRS += On_Demand_Activation
  DIRS += On_Demand_Loading

# Some POA policies unavailable in minimum CORBA
  DIRS += NewPOA
  DIRS += TIE

endif # minimum_corba

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU
