#----------------------------------------------------------------------------
#	@(#)Makefile	1.1	10/18/96
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

.obj/FIFO-Msg-client.o .shobj/FIFO-Msg-client.: FIFO-Msg-client.cpp \
 $(ACE_ROOT)/ace/FIFO_Send_Msg.h \
 $(ACE_ROOT)/ace/FIFO_Send.h \
 $(ACE_ROOT)/ace/FIFO.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
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
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/FIFO.i \
 $(ACE_ROOT)/ace/FIFO_Send.i \
 $(ACE_ROOT)/ace/FIFO_Send_Msg.i
.obj/FIFO-Msg-server.o .shobj/FIFO-Msg-server.: FIFO-Msg-server.cpp \
 $(ACE_ROOT)/ace/FIFO_Recv_Msg.h \
 $(ACE_ROOT)/ace/FIFO_Recv.h \
 $(ACE_ROOT)/ace/FIFO.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
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
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/FIFO.i \
 $(ACE_ROOT)/ace/FIFO_Recv.i \
 $(ACE_ROOT)/ace/FIFO_Recv_Msg.i
.obj/FIFO-client.o .shobj/FIFO-client.: FIFO-client.cpp \
 $(ACE_ROOT)/ace/FIFO_Send.h \
 $(ACE_ROOT)/ace/FIFO.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
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
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/FIFO.i \
 $(ACE_ROOT)/ace/FIFO_Send.i
.obj/FIFO-server.o .shobj/FIFO-server.: FIFO-server.cpp \
 $(ACE_ROOT)/ace/FIFO_Recv.h \
 $(ACE_ROOT)/ace/FIFO.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
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
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/FIFO.i \
 $(ACE_ROOT)/ace/FIFO_Recv.i
.obj/FIFO-test.o .shobj/FIFO-test.: FIFO-test.cpp \
 $(ACE_ROOT)/ace/FIFO_Recv.h \
 $(ACE_ROOT)/ace/FIFO.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
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
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/FIFO.i \
 $(ACE_ROOT)/ace/FIFO_Recv.i \
 $(ACE_ROOT)/ace/FIFO_Send.h \
 $(ACE_ROOT)/ace/FIFO_Send.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
