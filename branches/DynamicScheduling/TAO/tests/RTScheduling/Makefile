#----------------------------------------------------------------------------
#
#       $Id$
#
#       Makefile for RTScheduling tests
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

DIRS  = \
	DT_Spawn \
	Scheduling_Interceptor \
	VoidData \
	Current \
	Thread_Cancel

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
