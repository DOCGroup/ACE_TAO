#----------------------------------------------------------------------------
#	@(#)Makefile	1.1	10/18/96
#
#	Makefile for the Reactor multicast tests
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN 	= client server

FILES	= Log_Wrapper

LSRC	= $(addsuffix .cpp,$(FILES))
LOBJ	= $(addsuffix .o,$(FILES))
SHOBJ	= $(addsuffix .so,$(FILES))

LDLIBS	= $(addprefix .shobj/,$(SHOBJ)) 
 
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

.obj/Log_Wrapper.o .shobj/Log_Wrapper.so: Log_Wrapper.cpp Log_Wrapper.h \
 $(WRAPPER_ROOT)/ace/Profile_Timer.h \
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
 $(WRAPPER_ROOT)/ace/Time_Value.h \
 $(WRAPPER_ROOT)/ace/INET_Addr.h \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/SOCK_Dgram.h \
 $(WRAPPER_ROOT)/ace/SOCK.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SOCK.i \
 $(WRAPPER_ROOT)/ace/SOCK_Dgram.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
