#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for the simple DEV writer test file
#       for serial devices
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN	= writer

SRC   	= $(addsuffix .cpp,$(BIN))
OBJ   	= $(SRC:%.cpp=$(VDIR)%.o)

LSRC   	= $(addsuffix .cpp,$(FILES))
LOBJ    = $(addsuffix .o,$(FILES))
SHOBJ   = $(addsuffix .so,$(FILES))

VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD 	= $(VBIN)

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

.obj/writer.o .obj/writer.so .shobj/writer.o .shobj/writer.so: writer.cpp \
 $(ACE_ROOT)/ace/DEV_Connector.h \
 $(ACE_ROOT)/ace/DEV_IO.h \
 $(ACE_ROOT)/ace/DEV.h \
 $(ACE_ROOT)/ace/IO_SAP.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/IO_SAP.i \
 $(ACE_ROOT)/ace/DEV_Addr.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/DEV_Addr.i \
 $(ACE_ROOT)/ace/DEV.i \
 $(ACE_ROOT)/ace/DEV_IO.i \
 $(ACE_ROOT)/ace/DEV_Connector.i \
 $(ACE_ROOT)/ace/TTY_IO.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
