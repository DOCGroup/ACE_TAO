# $Id$

#----------------------------------------------------------------------------
#       Makefile for the libACE_RMCAST
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME   = libACE_RMCAST
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)


LIB_FILES = \
	RMCast\
	RMCast_Ack_Worker\
	RMCast_Copy_On_Write\
	RMCast_Fork\
	RMCast_Fragment\
	RMCast_IO_UDP\
	RMCast_Membership\
	RMCast_Module\
	RMCast_Module_Factory\
	RMCast_Partial_Message\
	RMCast_Proxy\
	RMCast_Reassembly\
	RMCast_Receiver_Module\
	RMCast_Reliable_Factory\
	RMCast_Reordering\
	RMCast_Resend_Handler\
	RMCast_Resend_Worker\
	RMCast_Retransmission\
	RMCast_Sequencer\
	RMCast_Singleton_Factory\
	RMCast_UDP_Event_Handler\
	RMCast_UDP_Proxy\
	RMCast_UDP_Reliable_Receiver\
	RMCast_UDP_Reliable_Sender\
	RMCast_Worker

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

####
#### ACE_COMPONENTS support.
####
FILES += $(LIB_FILES)

LSRC  = $(addsuffix .cpp,$(FILES))

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

INSTALL =

clean:
	$(RM) -f $(LIBNAME).a $(LIBNAME).so

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

