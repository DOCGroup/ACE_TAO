#----------------------------------------------------------------------------
#
#       $Id$
#
#       Makefile for RTCORBA tests
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

DIRS  = Banded_Connections \
	Client_Propagated \
	Client_Protocol \
	Explicit_Binding \
	Server_Declared \
        Server_Protocol \
	Private_Connection \
	Thread_Pool \
	MT_Client_Protocol_Priority \
        RTMutex \
	ORB_init

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU
