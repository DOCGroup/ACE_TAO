#----------------------------------------------------------------------------
#	@(#)Makefile	1.1	10/18/96
#
#	Makefile for ntalker demo of SOCK_Dgram_Mcast
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN = ntalker

SRC	= $(addsuffix .cpp,$(BIN))
 
LDLIBS	= 
VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD 	= $(VBIN)

#----------------------------------------------------------------------------
# 	ACE stuff
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

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/ntalker.o .shobj/ntalker.so: ntalker.cpp \
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
 $(WRAPPER_ROOT)/ace/SOCK_Dgram_Mcast.h \
 $(WRAPPER_ROOT)/ace/SOCK_Dgram.h \
 $(WRAPPER_ROOT)/ace/SOCK.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SOCK.i \
 $(WRAPPER_ROOT)/ace/SOCK_Dgram.i \
 $(WRAPPER_ROOT)/ace/SOCK_Dgram_Mcast.i \
 $(WRAPPER_ROOT)/ace/Reactor.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.h \
 $(WRAPPER_ROOT)/ace/Timer_Queue.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Time_Value.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Signal.h \
 $(WRAPPER_ROOT)/ace/Set.h \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Token.h \
 $(WRAPPER_ROOT)/ace/Pipe.h \
 $(WRAPPER_ROOT)/ace/Pipe.i \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.h \
 $(WRAPPER_ROOT)/ace/SOCK_IO.h \
 $(WRAPPER_ROOT)/ace/SOCK_IO.i \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.i \
 $(WRAPPER_ROOT)/ace/Get_Opt.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
