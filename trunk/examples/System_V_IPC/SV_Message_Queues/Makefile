#----------------------------------------------------------------------------
#	@(#)Makefile	1.1	10/18/96
#
#	Makefile for testing the typed and untyped Message Queue abstraction
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN     = MQ_Server  \
	  MQ_Client  \
	  TMQ_Server \
	  TMQ_Client 

LSRC    = $(addsuffix .cpp,$(BIN))

LDLIBS  = 

VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD   = $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(WRAPPER_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/macros.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.common.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.bin.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------
 
#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/MQ_Server.o .shobj/MQ_Server.so: MQ_Server.cpp \
 $(WRAPPER_ROOT)/ace/SV_Message_Queue.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/Time_Value.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/SV_Message.h \
 $(WRAPPER_ROOT)/ace/SV_Message_Queue.i \
 test.h
.obj/MQ_Client.o .shobj/MQ_Client.so: MQ_Client.cpp \
 $(WRAPPER_ROOT)/ace/SV_Message_Queue.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/Time_Value.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/SV_Message.h \
 $(WRAPPER_ROOT)/ace/SV_Message_Queue.i \
 test.h
.obj/TMQ_Server.o .shobj/TMQ_Server.so: TMQ_Server.cpp \
 $(WRAPPER_ROOT)/ace/Typed_SV_Message_Queue.h \
 $(WRAPPER_ROOT)/ace/SV_Message_Queue.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/Time_Value.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/SV_Message.h \
 $(WRAPPER_ROOT)/ace/SV_Message_Queue.i \
 $(WRAPPER_ROOT)/ace/Typed_SV_Message.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/Synch_T.h test.h
.obj/TMQ_Client.o .shobj/TMQ_Client.so: TMQ_Client.cpp \
 $(WRAPPER_ROOT)/ace/Typed_SV_Message_Queue.h \
 $(WRAPPER_ROOT)/ace/SV_Message_Queue.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/Time_Value.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/SV_Message.h \
 $(WRAPPER_ROOT)/ace/SV_Message_Queue.i \
 $(WRAPPER_ROOT)/ace/Typed_SV_Message.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/Synch_T.h test.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
