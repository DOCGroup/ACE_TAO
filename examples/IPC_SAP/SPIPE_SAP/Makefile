#----------------------------------------------------------------------------
#	@(#)Makefile	1.1	10/18/96
#
#	Makefile for the simple STREAM pipe client/server test file
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN   = client \
        server \
	consumer_msg \
	consumer_read \
	producer_msg \
	producer_read \
	NPClient \
	NPServer

LSRC    = $(addsuffix .cpp,$(BIN)) 

VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD = $(VBIN)

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

.obj/client.o .shobj/client.: client.cpp \
 $(ACE_ROOT)/ace/SPIPE_Addr.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/stdcpp.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/SPIPE_Addr.i \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/SPIPE_Connector.h \
 $(ACE_ROOT)/ace/SPIPE_Stream.h \
 $(ACE_ROOT)/ace/SPIPE.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SPIPE.i \
 $(ACE_ROOT)/ace/SPIPE_Stream.i \
 $(ACE_ROOT)/ace/SPIPE_Connector.i \
 shared.h
.obj/server.o .shobj/server.: server.cpp \
 $(ACE_ROOT)/ace/SPIPE_Addr.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/stdcpp.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/SPIPE_Addr.i \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/SPIPE_Acceptor.h \
 $(ACE_ROOT)/ace/SPIPE_Stream.h \
 $(ACE_ROOT)/ace/SPIPE.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SPIPE.i \
 $(ACE_ROOT)/ace/SPIPE_Stream.i \
 shared.h
.obj/consumer_msg.o .shobj/consumer_msg.: consumer_msg.cpp \
 $(ACE_ROOT)/ace/SPIPE_Addr.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/stdcpp.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/SPIPE_Addr.i \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/SPIPE_Acceptor.h \
 $(ACE_ROOT)/ace/SPIPE_Stream.h \
 $(ACE_ROOT)/ace/SPIPE.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SPIPE.i \
 $(ACE_ROOT)/ace/SPIPE_Stream.i \
 shared.h
.obj/consumer_read.o .shobj/consumer_read.: consumer_read.cpp \
 $(ACE_ROOT)/ace/SPIPE_Addr.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/stdcpp.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/SPIPE_Addr.i \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/SPIPE_Acceptor.h \
 $(ACE_ROOT)/ace/SPIPE_Stream.h \
 $(ACE_ROOT)/ace/SPIPE.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SPIPE.i \
 $(ACE_ROOT)/ace/SPIPE_Stream.i \
 shared.h
.obj/producer_msg.o .shobj/producer_msg.: producer_msg.cpp \
 $(ACE_ROOT)/ace/SPIPE_Addr.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/stdcpp.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/SPIPE_Addr.i \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/SPIPE_Connector.h \
 $(ACE_ROOT)/ace/SPIPE_Stream.h \
 $(ACE_ROOT)/ace/SPIPE.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SPIPE.i \
 $(ACE_ROOT)/ace/SPIPE_Stream.i \
 $(ACE_ROOT)/ace/SPIPE_Connector.i \
 shared.h
.obj/producer_read.o .shobj/producer_read.: producer_read.cpp \
 $(ACE_ROOT)/ace/SPIPE_Addr.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/stdcpp.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/SPIPE_Addr.i \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/SPIPE_Connector.h \
 $(ACE_ROOT)/ace/SPIPE_Stream.h \
 $(ACE_ROOT)/ace/SPIPE.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SPIPE.i \
 $(ACE_ROOT)/ace/SPIPE_Stream.i \
 $(ACE_ROOT)/ace/SPIPE_Connector.i \
 shared.h
.obj/NPClient.o .shobj/NPClient.: NPClient.cpp \
 $(ACE_ROOT)/ace/SPIPE_Addr.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/stdcpp.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/SPIPE_Addr.i \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/SPIPE_Connector.h \
 $(ACE_ROOT)/ace/SPIPE_Stream.h \
 $(ACE_ROOT)/ace/SPIPE.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SPIPE.i \
 $(ACE_ROOT)/ace/SPIPE_Stream.i \
 $(ACE_ROOT)/ace/SPIPE_Connector.i
.obj/NPServer.o .shobj/NPServer.: NPServer.cpp \
 $(ACE_ROOT)/ace/SPIPE_Addr.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/stdcpp.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/SPIPE_Addr.i \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/SPIPE_Acceptor.h \
 $(ACE_ROOT)/ace/SPIPE_Stream.h \
 $(ACE_ROOT)/ace/SPIPE.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SPIPE.i \
 $(ACE_ROOT)/ace/SPIPE_Stream.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
