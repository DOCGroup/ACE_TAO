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

include		$(WRAPPER_ROOT)/include/makeinclude/wrapper_macros.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/macros.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.common.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.nonested.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.bin.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/client.o .shobj/client.so: client.cpp \
 $(WRAPPER_ROOT)/ace/SPIPE_Addr.h \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/SPIPE_Connector.h \
 $(WRAPPER_ROOT)/ace/SPIPE_Stream.h \
 $(WRAPPER_ROOT)/ace/SPIPE.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SPIPE.i \
 $(WRAPPER_ROOT)/ace/SPIPE_Stream.i \
 $(WRAPPER_ROOT)/ace/SPIPE_Connector.i \
 shared.h
.obj/server.o .shobj/server.so: server.cpp \
 $(WRAPPER_ROOT)/ace/SPIPE_Addr.h \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/SPIPE_Acceptor.h \
 $(WRAPPER_ROOT)/ace/SPIPE_Stream.h \
 $(WRAPPER_ROOT)/ace/SPIPE.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SPIPE.i \
 $(WRAPPER_ROOT)/ace/SPIPE_Stream.i \
 shared.h
.obj/consumer_msg.o .shobj/consumer_msg.so: consumer_msg.cpp \
 $(WRAPPER_ROOT)/ace/SPIPE_Addr.h \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/SPIPE_Acceptor.h \
 $(WRAPPER_ROOT)/ace/SPIPE_Stream.h \
 $(WRAPPER_ROOT)/ace/SPIPE.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SPIPE.i \
 $(WRAPPER_ROOT)/ace/SPIPE_Stream.i \
 shared.h
.obj/consumer_read.o .shobj/consumer_read.so: consumer_read.cpp \
 $(WRAPPER_ROOT)/ace/SPIPE_Addr.h \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/SPIPE_Acceptor.h \
 $(WRAPPER_ROOT)/ace/SPIPE_Stream.h \
 $(WRAPPER_ROOT)/ace/SPIPE.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SPIPE.i \
 $(WRAPPER_ROOT)/ace/SPIPE_Stream.i \
 shared.h
.obj/producer_msg.o .shobj/producer_msg.so: producer_msg.cpp \
 $(WRAPPER_ROOT)/ace/SPIPE_Addr.h \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/SPIPE_Connector.h \
 $(WRAPPER_ROOT)/ace/SPIPE_Stream.h \
 $(WRAPPER_ROOT)/ace/SPIPE.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SPIPE.i \
 $(WRAPPER_ROOT)/ace/SPIPE_Stream.i \
 $(WRAPPER_ROOT)/ace/SPIPE_Connector.i \
 shared.h
.obj/producer_read.o .shobj/producer_read.so: producer_read.cpp \
 $(WRAPPER_ROOT)/ace/SPIPE_Addr.h \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/SPIPE_Connector.h \
 $(WRAPPER_ROOT)/ace/SPIPE_Stream.h \
 $(WRAPPER_ROOT)/ace/SPIPE.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SPIPE.i \
 $(WRAPPER_ROOT)/ace/SPIPE_Stream.i \
 $(WRAPPER_ROOT)/ace/SPIPE_Connector.i \
 shared.h
.obj/NPClient.o .shobj/NPClient.so: NPClient.cpp \
 $(WRAPPER_ROOT)/ace/SPIPE_Addr.h \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/SPIPE_Connector.h \
 $(WRAPPER_ROOT)/ace/SPIPE_Stream.h \
 $(WRAPPER_ROOT)/ace/SPIPE.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SPIPE.i \
 $(WRAPPER_ROOT)/ace/SPIPE_Stream.i \
 $(WRAPPER_ROOT)/ace/SPIPE_Connector.i
.obj/NPServer.o .shobj/NPServer.so: NPServer.cpp \
 $(WRAPPER_ROOT)/ace/SPIPE_Addr.h \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/SPIPE_Acceptor.h \
 $(WRAPPER_ROOT)/ace/SPIPE_Stream.h \
 $(WRAPPER_ROOT)/ace/SPIPE.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SPIPE.i \
 $(WRAPPER_ROOT)/ace/SPIPE_Stream.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
