# $Id$

#----------------------------------------------------------------------------
#       Makefile for the libACE_IPC
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME   = libACE_IPC
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)


LIB_FILES = \
	Addr\
	DEV\
	DEV_Addr\
	DEV_Connector\
	DEV_IO\
	FIFO\
	FIFO_Recv\
	FIFO_Recv_Msg\
	FIFO_Send\
	FIFO_Send_Msg\
	FILE\
	FILE_Addr\
	FILE_Connector\
	FILE_IO\
	INET_Addr\
	IOStream\
	IOStream_T\
	IO_SAP\
	Pipe\
	SPIPE\
	SPIPE_Acceptor\
	SPIPE_Addr\
	SPIPE_Connector\
	SPIPE_Stream\
	SV_Message\
	SV_Message_Queue\
	SV_Semaphore_Complex\
	SV_Semaphore_Simple\
	SV_Shared_Memory\
	Signal\
	TLI\
	TLI_Acceptor\
	TLI_Connector\
	TLI_Stream\
	TTY_IO\
	UNIX_Addr\
	UPIPE_Acceptor\
	UPIPE_Connector\
	UPIPE_Stream\
	XTI_ATM_Mcast

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

