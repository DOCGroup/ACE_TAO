#----------------------------------------------------------------------------
#	@(#)Makefile	1.1	10/18/96
#
#	Makefile for tests of the miscellaneous ACE performance tests
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN     = basic_perf \
	  childbirth_time \
	  context_switch_time \
	  preempt \
	  test_mutex \
	  test_singleton \
	  test_naming
# Only build static library for optimum run-time speed.
LIB	= libPerf.a

FILES   = basic_func

LSRC	= $(addsuffix .cpp,$(FILES)) 
LOBJ	= $(addsuffix .o,$(FILES))
SHOBJ	= $(addsuffix .so,$(FILES))

LDLIBS 	= -lPerf
VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD = $(VLIB) $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.lib.GNU
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

.obj/basic_func.o .shobj/basic_func.so: basic_func.cpp basic_func.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
