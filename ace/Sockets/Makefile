# $Id$

#----------------------------------------------------------------------------
#       Makefile for the libACE_Sockets
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBOS   = libACE_Sockets
LIB      = $(LIBOS).a
SHLIB    = $(LIBOS).$(SOEXT)


OS_FILES = \
	IPC_SAP\
	LSOCK_Acceptor\
	LSOCK_CODgram\
	LSOCK_Connector\
	LSOCK\
	LSOCK_Dgram\
	LSOCK_Stream\
	SOCK_Acceptor\
	SOCK_CODgram\
	SOCK_Connector\
	SOCK\
	SOCK_Dgram_Bcast\
	SOCK_Dgram\
	SOCK_Dgram_Mcast\
	SOCK_IO\
	SOCK_Stream

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

####
#### ACE_COMPONENTS support.
####
FILES += $(OS_FILES)

LSRC  = $(addsuffix .cpp,$(FILES))

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

INSTALL =

clean:
	$(RM) -f $(LIBOS).a $(LIBOS).so

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------



#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


