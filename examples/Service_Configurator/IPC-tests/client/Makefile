#----------------------------------------------------------------------------
#	@(#)Makefile	1.1	10/18/96
#
#	Makefile for the client-side of the primary Reactor tests
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN   	= local_dgram_client_test \
  	  local_stream_client_test \
	  local_pipe_client_test \
	  remote_stream_client_test \
	  remote_thr_stream_client_test \
	  remote_dgram_client_test \
	  local_fifo_client_test \
	  broadcast_client_test \
	  local_spipe_client_test \
	  remote_service_directory_test

LSRC  	= $(addsuffix .cpp,$(BIN))

LDLIBS	= 
 
VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD 	= $(VBIN)

INSTALL =

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

.obj/local_dgram_client_test.o .shobj/local_dgram_client_test.so: local_dgram_client_test.cpp \
 $(WRAPPER_ROOT)/ace/LSOCK_CODgram.h \
 $(WRAPPER_ROOT)/ace/LSOCK.h \
 $(WRAPPER_ROOT)/ace/SOCK.h \
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
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SOCK.i \
 $(WRAPPER_ROOT)/ace/LSOCK.i \
 $(WRAPPER_ROOT)/ace/SOCK_CODgram.h \
 $(WRAPPER_ROOT)/ace/SOCK_IO.h \
 $(WRAPPER_ROOT)/ace/SOCK_IO.i \
 $(WRAPPER_ROOT)/ace/SOCK_CODgram.i \
 $(WRAPPER_ROOT)/ace/LSOCK_CODgram.i \
 $(WRAPPER_ROOT)/ace/LSOCK_Dgram.h \
 $(WRAPPER_ROOT)/ace/SOCK_Dgram.h \
 $(WRAPPER_ROOT)/ace/SOCK_Dgram.i \
 $(WRAPPER_ROOT)/ace/LSOCK_Dgram.i \
 $(WRAPPER_ROOT)/ace/UNIX_Addr.h \
 $(WRAPPER_ROOT)/ace/Get_Opt.h
.obj/local_stream_client_test.o .shobj/local_stream_client_test.so: local_stream_client_test.cpp \
 $(WRAPPER_ROOT)/ace/LSOCK_Connector.h \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.h \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.h \
 $(WRAPPER_ROOT)/ace/SOCK_IO.h \
 $(WRAPPER_ROOT)/ace/SOCK.h \
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
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SOCK.i \
 $(WRAPPER_ROOT)/ace/SOCK_IO.i \
 $(WRAPPER_ROOT)/ace/INET_Addr.h \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.i \
 $(WRAPPER_ROOT)/ace/Time_Value.h \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.i \
 $(WRAPPER_ROOT)/ace/LSOCK_Stream.h \
 $(WRAPPER_ROOT)/ace/UNIX_Addr.h \
 $(WRAPPER_ROOT)/ace/LSOCK.h \
 $(WRAPPER_ROOT)/ace/LSOCK.i \
 $(WRAPPER_ROOT)/ace/LSOCK_Stream.i \
 $(WRAPPER_ROOT)/ace/LSOCK_Connector.i \
 $(WRAPPER_ROOT)/ace/Get_Opt.h
.obj/local_pipe_client_test.o .shobj/local_pipe_client_test.so: local_pipe_client_test.cpp \
 $(WRAPPER_ROOT)/ace/LSOCK_Connector.h \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.h \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.h \
 $(WRAPPER_ROOT)/ace/SOCK_IO.h \
 $(WRAPPER_ROOT)/ace/SOCK.h \
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
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SOCK.i \
 $(WRAPPER_ROOT)/ace/SOCK_IO.i \
 $(WRAPPER_ROOT)/ace/INET_Addr.h \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.i \
 $(WRAPPER_ROOT)/ace/Time_Value.h \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.i \
 $(WRAPPER_ROOT)/ace/LSOCK_Stream.h \
 $(WRAPPER_ROOT)/ace/UNIX_Addr.h \
 $(WRAPPER_ROOT)/ace/LSOCK.h \
 $(WRAPPER_ROOT)/ace/LSOCK.i \
 $(WRAPPER_ROOT)/ace/LSOCK_Stream.i \
 $(WRAPPER_ROOT)/ace/LSOCK_Connector.i \
 $(WRAPPER_ROOT)/ace/Get_Opt.h
.obj/remote_stream_client_test.o .shobj/remote_stream_client_test.so: remote_stream_client_test.cpp \
 $(WRAPPER_ROOT)/ace/Mem_Map.h \
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
 $(WRAPPER_ROOT)/ace/SOCK_Connector.h \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.h \
 $(WRAPPER_ROOT)/ace/SOCK_IO.h \
 $(WRAPPER_ROOT)/ace/SOCK.h \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SOCK.i \
 $(WRAPPER_ROOT)/ace/SOCK_IO.i \
 $(WRAPPER_ROOT)/ace/INET_Addr.h \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.i \
 $(WRAPPER_ROOT)/ace/Time_Value.h \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.i \
 $(WRAPPER_ROOT)/ace/SOCK_CODgram.h \
 $(WRAPPER_ROOT)/ace/SOCK_CODgram.i \
 $(WRAPPER_ROOT)/ace/Get_Opt.h
.obj/remote_thr_stream_client_test.o .shobj/remote_thr_stream_client_test.so: remote_thr_stream_client_test.cpp \
 $(WRAPPER_ROOT)/ace/Mem_Map.h \
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
 $(WRAPPER_ROOT)/ace/TLI_Connector.h \
 $(WRAPPER_ROOT)/ace/TLI_Stream.h \
 $(WRAPPER_ROOT)/ace/TLI.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/TLI.i \
 $(WRAPPER_ROOT)/ace/INET_Addr.h \
 $(WRAPPER_ROOT)/ace/TLI_Stream.i \
 $(WRAPPER_ROOT)/ace/TLI_Connector.i \
 $(WRAPPER_ROOT)/ace/Get_Opt.h
.obj/remote_dgram_client_test.o .shobj/remote_dgram_client_test.so: remote_dgram_client_test.cpp \
 $(WRAPPER_ROOT)/ace/Mem_Map.h \
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
 $(WRAPPER_ROOT)/ace/SOCK_Dgram.h \
 $(WRAPPER_ROOT)/ace/SOCK.h \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SOCK.i \
 $(WRAPPER_ROOT)/ace/SOCK_Dgram.i \
 $(WRAPPER_ROOT)/ace/INET_Addr.h \
 $(WRAPPER_ROOT)/ace/Get_Opt.h
.obj/local_fifo_client_test.o .shobj/local_fifo_client_test.so: local_fifo_client_test.cpp \
 $(WRAPPER_ROOT)/ace/Mem_Map.h \
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
 $(WRAPPER_ROOT)/ace/FIFO_Send_Msg.h \
 $(WRAPPER_ROOT)/ace/FIFO_Send.h \
 $(WRAPPER_ROOT)/ace/FIFO.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/FIFO_Send.i \
 $(WRAPPER_ROOT)/ace/FIFO_Send_Msg.i \
 $(WRAPPER_ROOT)/ace/Get_Opt.h
.obj/broadcast_client_test.o .shobj/broadcast_client_test.so: broadcast_client_test.cpp \
 $(WRAPPER_ROOT)/ace/INET_Addr.h \
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
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/SOCK_Dgram_Bcast.h \
 $(WRAPPER_ROOT)/ace/SOCK_Dgram.h \
 $(WRAPPER_ROOT)/ace/SOCK.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SOCK.i \
 $(WRAPPER_ROOT)/ace/SOCK_Dgram.i \
 $(WRAPPER_ROOT)/ace/SOCK_Dgram_Bcast.i \
 $(WRAPPER_ROOT)/ace/Get_Opt.h
.obj/local_spipe_client_test.o .shobj/local_spipe_client_test.so: local_spipe_client_test.cpp \
 $(WRAPPER_ROOT)/ace/Mem_Map.h \
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
 $(WRAPPER_ROOT)/ace/SPIPE_Addr.h \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/SPIPE.i \
 $(WRAPPER_ROOT)/ace/SPIPE_Stream.i \
 $(WRAPPER_ROOT)/ace/SPIPE_Connector.i \
 $(WRAPPER_ROOT)/ace/Get_Opt.h
.obj/remote_service_directory_test.o .shobj/remote_service_directory_test.so: remote_service_directory_test.cpp \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.h \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.h \
 $(WRAPPER_ROOT)/ace/SOCK_IO.h \
 $(WRAPPER_ROOT)/ace/SOCK.h \
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
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SOCK.i \
 $(WRAPPER_ROOT)/ace/SOCK_IO.i \
 $(WRAPPER_ROOT)/ace/INET_Addr.h \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.i \
 $(WRAPPER_ROOT)/ace/Time_Value.h \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.i \
 $(WRAPPER_ROOT)/ace/Get_Opt.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
