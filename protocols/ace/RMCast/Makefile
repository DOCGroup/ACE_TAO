#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIB      = libACE_RMCast.a
SHLIB    = libACE_RMCast.$(SOEXT)

FILES= \
	RMCast \
	RMCast_Module \
	RMCast_Module_Factory \
	RMCast_Fragment \
	RMCast_Reassembly \
	RMCast_Partial_Message \
	RMCast_Proxy \
	RMCast_Membership \
	RMCast_Retransmission \
	RMCast_Resend_Worker \
	RMCast_Ack_Worker \
	RMCast_Reordering \
	RMCast_Reliable_Factory \
	RMCast_Singleton_Factory \
	RMCast_Resend_Handler \
	RMCast_Receiver_Module \
	RMCast_Sequencer \
	RMCast_Fork \
	\
	RMCast_IO_UDP \
	RMCast_UDP_Event_Handler \
	RMCast_UDP_Proxy \
	RMCast_UDP_Reliable_Receiver \
	RMCast_UDP_Reliable_Sender

LIBS=$(ACELIB)

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

LSRC  = $(addsuffix .cpp,$(FILES))

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
