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

.obj/local_dgram_client_test.o .shobj/local_dgram_client_test.so: local_dgram_client_test.cpp \
 $(ACE_ROOT)/ace/LSOCK_CODgram.h \
 $(ACE_ROOT)/ace/LSOCK.h \
 $(ACE_ROOT)/ace/SOCK.h \
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
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SOCK.i \
 $(ACE_ROOT)/ace/LSOCK.i \
 $(ACE_ROOT)/ace/SOCK_CODgram.h \
 $(ACE_ROOT)/ace/SOCK_IO.h \
 $(ACE_ROOT)/ace/SOCK_IO.i \
 $(ACE_ROOT)/ace/SOCK_CODgram.i \
 $(ACE_ROOT)/ace/LSOCK_CODgram.i \
 $(ACE_ROOT)/ace/LSOCK_Dgram.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.i \
 $(ACE_ROOT)/ace/LSOCK_Dgram.i \
 $(ACE_ROOT)/ace/UNIX_Addr.h \
 $(ACE_ROOT)/ace/UNIX_Addr.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i
.obj/local_stream_client_test.o .shobj/local_stream_client_test.so: local_stream_client_test.cpp \
 $(ACE_ROOT)/ace/LSOCK_Connector.h \
 $(ACE_ROOT)/ace/SOCK_Connector.h \
 $(ACE_ROOT)/ace/SOCK_Stream.h \
 $(ACE_ROOT)/ace/SOCK_IO.h \
 $(ACE_ROOT)/ace/SOCK.h \
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
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SOCK.i \
 $(ACE_ROOT)/ace/SOCK_IO.i \
 $(ACE_ROOT)/ace/INET_Addr.h \
 $(ACE_ROOT)/ace/INET_Addr.i \
 $(ACE_ROOT)/ace/SOCK_Stream.i \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/SOCK_Connector.i \
 $(ACE_ROOT)/ace/LSOCK_Stream.h \
 $(ACE_ROOT)/ace/UNIX_Addr.h \
 $(ACE_ROOT)/ace/UNIX_Addr.i \
 $(ACE_ROOT)/ace/LSOCK.h \
 $(ACE_ROOT)/ace/LSOCK.i \
 $(ACE_ROOT)/ace/LSOCK_Stream.i \
 $(ACE_ROOT)/ace/LSOCK_Connector.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i
.obj/local_pipe_client_test.o .shobj/local_pipe_client_test.so: local_pipe_client_test.cpp \
 $(ACE_ROOT)/ace/LSOCK_Connector.h \
 $(ACE_ROOT)/ace/SOCK_Connector.h \
 $(ACE_ROOT)/ace/SOCK_Stream.h \
 $(ACE_ROOT)/ace/SOCK_IO.h \
 $(ACE_ROOT)/ace/SOCK.h \
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
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SOCK.i \
 $(ACE_ROOT)/ace/SOCK_IO.i \
 $(ACE_ROOT)/ace/INET_Addr.h \
 $(ACE_ROOT)/ace/INET_Addr.i \
 $(ACE_ROOT)/ace/SOCK_Stream.i \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/SOCK_Connector.i \
 $(ACE_ROOT)/ace/LSOCK_Stream.h \
 $(ACE_ROOT)/ace/UNIX_Addr.h \
 $(ACE_ROOT)/ace/UNIX_Addr.i \
 $(ACE_ROOT)/ace/LSOCK.h \
 $(ACE_ROOT)/ace/LSOCK.i \
 $(ACE_ROOT)/ace/LSOCK_Stream.i \
 $(ACE_ROOT)/ace/LSOCK_Connector.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i
.obj/remote_stream_client_test.o .shobj/remote_stream_client_test.so: remote_stream_client_test.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
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
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/SOCK_Connector.h \
 $(ACE_ROOT)/ace/SOCK_Stream.h \
 $(ACE_ROOT)/ace/SOCK_IO.h \
 $(ACE_ROOT)/ace/SOCK.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SOCK.i \
 $(ACE_ROOT)/ace/SOCK_IO.i \
 $(ACE_ROOT)/ace/INET_Addr.h \
 $(ACE_ROOT)/ace/INET_Addr.i \
 $(ACE_ROOT)/ace/SOCK_Stream.i \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/SOCK_Connector.i \
 $(ACE_ROOT)/ace/SOCK_CODgram.h \
 $(ACE_ROOT)/ace/SOCK_CODgram.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i
.obj/remote_thr_stream_client_test.o .shobj/remote_thr_stream_client_test.so: remote_thr_stream_client_test.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
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
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/TLI_Connector.h \
 $(ACE_ROOT)/ace/TLI_Stream.h \
 $(ACE_ROOT)/ace/TLI.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/TLI.i \
 $(ACE_ROOT)/ace/INET_Addr.h \
 $(ACE_ROOT)/ace/INET_Addr.i \
 $(ACE_ROOT)/ace/TLI_Stream.i \
 $(ACE_ROOT)/ace/TLI_Connector.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i
.obj/remote_dgram_client_test.o .shobj/remote_dgram_client_test.so: remote_dgram_client_test.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
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
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/SOCK_Dgram.h \
 $(ACE_ROOT)/ace/SOCK.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SOCK.i \
 $(ACE_ROOT)/ace/SOCK_Dgram.i \
 $(ACE_ROOT)/ace/INET_Addr.h \
 $(ACE_ROOT)/ace/INET_Addr.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i
.obj/local_fifo_client_test.o .shobj/local_fifo_client_test.so: local_fifo_client_test.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
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
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/FIFO_Send_Msg.h \
 $(ACE_ROOT)/ace/FIFO_Send.h \
 $(ACE_ROOT)/ace/FIFO.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/FIFO.i \
 $(ACE_ROOT)/ace/FIFO_Send.i \
 $(ACE_ROOT)/ace/FIFO_Send_Msg.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i
.obj/broadcast_client_test.o .shobj/broadcast_client_test.so: broadcast_client_test.cpp \
 $(ACE_ROOT)/ace/INET_Addr.h \
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
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/INET_Addr.i \
 $(ACE_ROOT)/ace/SOCK_Dgram_Bcast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.h \
 $(ACE_ROOT)/ace/SOCK.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SOCK.i \
 $(ACE_ROOT)/ace/SOCK_Dgram.i \
 $(ACE_ROOT)/ace/SOCK_Dgram_Bcast.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i
.obj/local_spipe_client_test.o .shobj/local_spipe_client_test.so: local_spipe_client_test.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
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
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/SPIPE_Connector.h \
 $(ACE_ROOT)/ace/SPIPE_Stream.h \
 $(ACE_ROOT)/ace/SPIPE.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SPIPE_Addr.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/SPIPE_Addr.i \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/SPIPE.i \
 $(ACE_ROOT)/ace/SPIPE_Stream.i \
 $(ACE_ROOT)/ace/SPIPE_Connector.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i
.obj/remote_service_directory_test.o .shobj/remote_service_directory_test.so: remote_service_directory_test.cpp \
 $(ACE_ROOT)/ace/SOCK_Connector.h \
 $(ACE_ROOT)/ace/SOCK_Stream.h \
 $(ACE_ROOT)/ace/SOCK_IO.h \
 $(ACE_ROOT)/ace/SOCK.h \
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
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SOCK.i \
 $(ACE_ROOT)/ace/SOCK_IO.i \
 $(ACE_ROOT)/ace/INET_Addr.h \
 $(ACE_ROOT)/ace/INET_Addr.i \
 $(ACE_ROOT)/ace/SOCK_Stream.i \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/SOCK_Connector.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
