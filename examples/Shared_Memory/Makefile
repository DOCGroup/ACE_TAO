#----------------------------------------------------------------------------
#	@(#)Makefile	1.1	10/18/96
#
#	Makefile for tests of the Shared_Malloc wrappers
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN     = test_MM \
	  test_SV 

LSRC    = test_MM.cpp  \
	  test_SV.cpp 

LDLIBS  = 

VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD   = $(VBIN)

INSTALL =

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

.obj/test_MM.o .shobj/test_MM.so: test_MM.cpp \
 $(WRAPPER_ROOT)/ace/Shared_Memory_MM.h \
 $(WRAPPER_ROOT)/ace/Shared_Memory.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Mem_Map.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.i \
 $(WRAPPER_ROOT)/ace/Shared_Memory_MM.i
.obj/test_SV.o .shobj/test_SV.so: test_SV.cpp \
 $(WRAPPER_ROOT)/ace/Shared_Memory_SV.h \
 $(WRAPPER_ROOT)/ace/Shared_Memory.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/SV_Shared_Memory.h \
 $(WRAPPER_ROOT)/ace/SV_Shared_Memory.i \
 $(WRAPPER_ROOT)/ace/Shared_Memory_SV.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
