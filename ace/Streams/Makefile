# $Id$

#----------------------------------------------------------------------------
#       Makefile for the libACE_Streams
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIB_NAME   = libACE_Streams
LIB      = $(LIB_NAME).a
SHLIB    = $(LIB_NAME).$(SOEXT)


FILES = \
	CDR_Base\
	CDR_Stream\
	Codeset_IBM1047\
	Message_Queue\
	Message_Queue_T\
	Module\
	Notification_Strategy\
	Read_Buffer\
	Stream\
	Stream_Modules\
	Task\
	Task_T\
	Typed_SV_Message\
	Typed_SV_Message_Queue

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

####
#### ACE_COMPONENTS support.
####

LSRC  = $(addsuffix .cpp,$(FILES))

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

INSTALL =

clean:
	$(RM) -f $(LIB_NAME).a $(LIB_NAME).so

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------



#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

