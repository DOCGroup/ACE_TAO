#----------------------------------------------------------------------------
#	@(#)Makefile	1.1	10/18/96
#
#	Makefile for Mem_Map file reverse test
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN   	= file-reverse

LSRC    = $(addsuffix .cpp,$(BIN)) 

LDLIBS  = 

VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD 	= $(VBIN) 

INSTALL = 

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------
 
#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/file-reverse.o .shobj/file-reverse.: file-reverse.cpp \
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
 $(ACE_ROOT)/ace/Mem_Map.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
