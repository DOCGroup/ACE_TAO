#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for ATM_SAP test
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN   = CPP-client \
	CPP-server

LSRC   = $(addsuffix .cpp,$(BIN))

VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD = $(VBIN)

INSTALL = 

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

LDFLAGS  += -R$(FORE_ROOT)/lib -L$(FORE_ROOT)/lib

# These are the libraries that FORE used to use. These might be different
#  when FORE actually supports Solaris 7
#LIBS     += -lans -lpmp
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

