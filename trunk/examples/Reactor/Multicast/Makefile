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

LDLIBS	= $(addprefix $(VSHDIR),$(SHOBJ))
 
VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD 	= $(VBIN)

#----------------------------------------------------------------------------
# 	ACE stuff
#----------------------------------------------------------------------------

include		$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include		$(ACE_ROOT)/include/makeinclude/macros.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.lib.GNU
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

.obj/Log_Wrapper.o .shobj/Log_Wrapper.so: Log_Wrapper.cpp Log_Wrapper.h \
 $(ACE_ROOT)/ace/Profile_Timer.h \
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
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/High_Res_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.i \
 $(ACE_ROOT)/ace/Profile_Timer.i \
 $(ACE_ROOT)/ace/INET_Addr.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/INET_Addr.i \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.h \
 $(ACE_ROOT)/ace/SOCK.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SOCK.i \
 $(ACE_ROOT)/ace/SOCK_Dgram.i \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
