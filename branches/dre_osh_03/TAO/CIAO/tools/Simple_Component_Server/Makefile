#----------------------------------------------------------------------------
#
#	$Id$
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT
ifndef CIAO_ROOT
  CIAO_ROOT = $(ACE_ROOT)/TAO/CIAO
endif # ! CIAO_ROOT

LDLIBS = -lTAO_IFR_Client \
	 -lCIAO_Client \
	 -lCIAO_Container \
	 -lCIAO_Server \
	 -lTAO_CosNaming \
	 -lTAO_PortableServer -lTAO_Valuetype -lTAO_ObjRefTemplate -lTAO_IORInterceptor \
         -lTAO

SRC = Simple_Component_Server.cpp

BIN = Simple_Component_Server

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

INCLDIRS += -I$(CIAO_ROOT) -I$(TAO_ROOT)/orbsvcs/orbsvcs

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
