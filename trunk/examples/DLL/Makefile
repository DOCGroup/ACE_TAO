#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for the miscellaneous Reactor examples
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN    	= test_dll
SHLIB	= libToday.$(SOEXT) \
	  libNewsweek.$(SOEXT)

FILES 	= Today \
	  Newsweek

PSRC   	= $(addsuffix .cpp,$(BIN))
LSRC	= $(addsuffix .cpp,$(FILES))

VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD	= $(VSHLIB) $(VBIN) 

#----------------------------------------------------------------------------
#	Include macros and targets
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
