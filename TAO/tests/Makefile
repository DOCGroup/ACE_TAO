#----------------------------------------------------------------------------
#
#       $Id$
#
#       Makefile for the TAO tests
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

DIRS  = CDR \
	IDL_Test \
        OctetSeq \
        InterOp-Naming \
        NestedUpcall \
        Multiple_Inheritance \
	MT_Client \
	MT_Server \
	FL_Cube \
	POA \
	IORManipulation \
	Timeout \
	Connection_Purging \
	ORB_init

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

ifndef minimum_corba

# No DynAny in minimum CORBA
  DIRS += DynAny_Test

# No NVList and Context in minimum CORBA
  DIRS += Param_Test \
	DSI_Gateway

# No Implicit activation in minimum CORBA
  DIRS += Faults

endif # minimum_corba

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU
