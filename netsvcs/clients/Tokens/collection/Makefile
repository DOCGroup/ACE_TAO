#----------------------------------------------------------------------------
#	@(#)Makefile	1.1	10/18/96
#
#	Makefile for repeating token client application
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN   	= collection

FILES 	= collection

LSRC   	= $(addsuffix .cpp,$(FILES))
LOBJ   	= $(LSRC:%.cpp=$(VDIR)%.o)
SHOBJ	= $(addsuffix .so,$(FILES))

BUILD 	= $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include		$(WRAPPER_ROOT)/include/makeinclude/wrapper_macros.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/macros.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.common.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.nonested.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.lib.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.bin.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/collection.o .shobj/collection.so: collection.cpp \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/Get_Opt.h \
 $(WRAPPER_ROOT)/ace/Local_Tokens.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Stack.h \
 $(WRAPPER_ROOT)/ace/Synch_Options.h \
 $(WRAPPER_ROOT)/ace/Map_Manager.h \
 $(WRAPPER_ROOT)/ace/Token_Collection.h \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/Remote_Tokens.h \
 $(WRAPPER_ROOT)/ace/INET_Addr.h \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.h \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.h \
 $(WRAPPER_ROOT)/ace/SOCK_IO.h \
 $(WRAPPER_ROOT)/ace/SOCK.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SOCK.i \
 $(WRAPPER_ROOT)/ace/SOCK_IO.i \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.i \
 $(WRAPPER_ROOT)/ace/Time_Value.h \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.i \
 $(WRAPPER_ROOT)/ace/Token_Request_Reply.h \
 $(WRAPPER_ROOT)/ace/Thread_Manager.h \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Service_Config.h \
 $(WRAPPER_ROOT)/ace/Service_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.h \
 $(WRAPPER_ROOT)/ace/Signal.h \
 $(WRAPPER_ROOT)/ace/Set.h \
 $(WRAPPER_ROOT)/ace/Reactor.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.h \
 $(WRAPPER_ROOT)/ace/Timer_Queue.h \
 $(WRAPPER_ROOT)/ace/Token.h \
 $(WRAPPER_ROOT)/ace/Pipe.h \
 $(WRAPPER_ROOT)/ace/Pipe.i \
 $(WRAPPER_ROOT)/ace/Proactor.h \
 $(WRAPPER_ROOT)/ace/Message_Block.h \
 $(WRAPPER_ROOT)/ace/Malloc.h \
 $(WRAPPER_ROOT)/ace/Malloc_T.h \
 $(WRAPPER_ROOT)/ace/Memory_Pool.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.h \
 $(WRAPPER_ROOT)/ace/ReactorEx.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.h \
 $(WRAPPER_ROOT)/ace/IO_Cntl_Msg.h \
 $(WRAPPER_ROOT)/ace/Strategies.h \
 $(WRAPPER_ROOT)/ace/Strategies_T.h \
 $(WRAPPER_ROOT)/ace/Svc_Conf_Tokens.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
