#----------------------------------------------------------------------------
#	%W%	%G%
#
#	Makefile for the ACE Adapter Web Server (JAWS) client
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN   	= http_client

FILES   = http_handler

LSRC	= $(addsuffix .cpp,$(FILES))
LOBJ	= $(addsuffix .o,$(FILES))
SHOBJ	= $(addsuffix .so,$(FILES))

LDLIBS	= $(addprefix .shobj/,$(SHOBJ))
 
VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD 	= $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
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



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
