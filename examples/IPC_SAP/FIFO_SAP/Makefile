#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for simple FIFO test
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN	= FIFO-Msg-client \
	  FIFO-Msg-server \
	  FIFO-client \
	  FIFO-server \
	  FIFO-test

LSRC    = $(addsuffix .cpp,$(BIN)) 

VLDLIBS	= $(LDLIBS:%=%$(VAR))

BUILD	= $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include		$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include		$(ACE_ROOT)/include/makeinclude/macros.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/FIFO-Msg-client.o .obj/FIFO-Msg-client.so .shobj/FIFO-Msg-client.o .shobj/FIFO-Msg-client.so: FIFO-Msg-client.cpp \
 $(ACE_ROOT)/ace/FIFO_Send_Msg.h \
 $(ACE_ROOT)/ace/FIFO_Send.h \
 $(ACE_ROOT)/ace/FIFO.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/FIFO.i \
 $(ACE_ROOT)/ace/FIFO_Send.i \
 $(ACE_ROOT)/ace/FIFO_Send_Msg.i
.obj/FIFO-Msg-server.o .obj/FIFO-Msg-server.so .shobj/FIFO-Msg-server.o .shobj/FIFO-Msg-server.so: FIFO-Msg-server.cpp \
 $(ACE_ROOT)/ace/FIFO_Recv_Msg.h \
 $(ACE_ROOT)/ace/FIFO_Recv.h \
 $(ACE_ROOT)/ace/FIFO.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/FIFO.i \
 $(ACE_ROOT)/ace/FIFO_Recv.i \
 $(ACE_ROOT)/ace/FIFO_Recv_Msg.i
.obj/FIFO-client.o .obj/FIFO-client.so .shobj/FIFO-client.o .shobj/FIFO-client.so: FIFO-client.cpp \
 $(ACE_ROOT)/ace/FIFO_Send.h \
 $(ACE_ROOT)/ace/FIFO.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/FIFO.i \
 $(ACE_ROOT)/ace/FIFO_Send.i
.obj/FIFO-server.o .obj/FIFO-server.so .shobj/FIFO-server.o .shobj/FIFO-server.so: FIFO-server.cpp \
 $(ACE_ROOT)/ace/FIFO_Recv.h \
 $(ACE_ROOT)/ace/FIFO.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/FIFO.i \
 $(ACE_ROOT)/ace/FIFO_Recv.i
.obj/FIFO-test.o .obj/FIFO-test.so .shobj/FIFO-test.o .shobj/FIFO-test.so: FIFO-test.cpp \
 $(ACE_ROOT)/ace/FIFO_Recv.h \
 $(ACE_ROOT)/ace/FIFO.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/FIFO.i \
 $(ACE_ROOT)/ace/FIFO_Recv.i \
 $(ACE_ROOT)/ace/FIFO_Send.h \
 $(ACE_ROOT)/ace/FIFO_Send.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
