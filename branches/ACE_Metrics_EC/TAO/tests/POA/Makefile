#----------------------------------------------------------------------------
#
# $Id$
#
# Makefile for the POA tests
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

DIRS  = POA_Destruction \
	Default_Servant \
	wait_for_completion \
	Excessive_Object_Deactivations \
	Etherealization \
	Object_Reactivation \
	Non_Servant_Upcalls \
	MT_Servant_Locator \
	Nested_Non_Servant_Upcalls \
	Single_Threaded_POA \
	Persistent_ID \
	Policies \
	Current \
	Identity 

# The following test hasn't been updated yet
# Demux_Test

#### Does not compile on UNIX:	Forwarding \

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU
