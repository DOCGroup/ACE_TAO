#----------------------------------------------------------------------------
#	@(#)Makefile	1.1	10/18/96
#
#	Makefile for Mem_Map IO tests
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN   	= test_io

FILES   = IO_Test

LSRC    = $(addsuffix .cpp,$(FILES))
LOBJ	= $(addsuffix .o,$(FILES)) 
SHOBJ	= $(addsuffix .so,$(FILES))

LDLIBS	=  $(addprefix .shobj/,$(SHOBJ)) 

VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD 	= $(VBIN) 

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(WRAPPER_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/macros.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.common.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.lib.GNU
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

.obj/IO_Test.o .shobj/IO_Test.so: IO_Test.cpp \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/Mem_Map.h IO_Test.h \
 $(WRAPPER_ROOT)/ace/Profile_Timer.h \
 $(WRAPPER_ROOT)/ace/Time_Value.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
