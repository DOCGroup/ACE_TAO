#----------------------------------------------------------------------------
#	@(#)Makefile	1.1	10/18/96
#
#	Makefile for IPC_SAP test
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INFO  = README

BIN   = CPP-inclient \
	CPP-unclient \
	CPP-inserver \
	CPP-inserver-poll \
	CPP-unserver \
	FD-unclient \
	FD-unserver \
	C-inclient \
	C-inserver 

LSRC   = $(addsuffix .cpp,$(BIN))

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

.obj/CPP-inclient.o .shobj/CPP-inclient.so: CPP-inclient.cpp \
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
 $(WRAPPER_ROOT)/ace/SOCK_Connector.i
.obj/CPP-unclient.o .shobj/CPP-unclient.so: CPP-unclient.cpp \
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
 $(WRAPPER_ROOT)/ace/LSOCK_Connector.i
.obj/CPP-inserver.o .shobj/CPP-inserver.so: CPP-inserver.cpp \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.h \
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
 $(WRAPPER_ROOT)/ace/Handle_Set.h
.obj/CPP-inserver-poll.o .shobj/CPP-inserver-poll.so: CPP-inserver-poll.cpp \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.h \
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
 $(WRAPPER_ROOT)/ace/Time_Value.h
.obj/CPP-unserver.o .shobj/CPP-unserver.so: CPP-unserver.cpp \
 $(WRAPPER_ROOT)/ace/LSOCK_Acceptor.h \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.h \
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
 $(WRAPPER_ROOT)/ace/UNIX_Addr.h \
 $(WRAPPER_ROOT)/ace/LSOCK_Stream.h \
 $(WRAPPER_ROOT)/ace/LSOCK.h \
 $(WRAPPER_ROOT)/ace/LSOCK.i \
 $(WRAPPER_ROOT)/ace/LSOCK_Stream.i
.obj/FD-unclient.o .shobj/FD-unclient.so: FD-unclient.cpp \
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
 $(WRAPPER_ROOT)/ace/LSOCK_Connector.i
.obj/FD-unserver.o .shobj/FD-unserver.so: FD-unserver.cpp \
 $(WRAPPER_ROOT)/ace/LSOCK_Acceptor.h \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.h \
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
 $(WRAPPER_ROOT)/ace/UNIX_Addr.h \
 $(WRAPPER_ROOT)/ace/LSOCK_Stream.h \
 $(WRAPPER_ROOT)/ace/LSOCK.h \
 $(WRAPPER_ROOT)/ace/LSOCK.i \
 $(WRAPPER_ROOT)/ace/LSOCK_Stream.i
.obj/C-inclient.o .shobj/C-inclient.so: C-inclient.cpp \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Log_Record.i
.obj/C-inserver.o .shobj/C-inserver.so: C-inserver.cpp \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Log_Record.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
