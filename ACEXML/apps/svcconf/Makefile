#----------------------------------------------------------------------------
#       $Id$
#
#       Makefile for the server-side ACE network services
#----------------------------------------------------------------------------

LIB     = libACEXML_XML_Svc_Conf_Parser.a
SHLIB   = libACEXML_XML_Svc_Conf_Parser.$(SOEXT)

FILES   = Svcconf_Handler \
	  Svcconf

DEFS    = $(addsuffix .h,$(FILES))
LSRC    = $(addsuffix .cpp,$(FILES))

LIBS   +=  -lACEXML_Parser -lACEXML $(ACELIB)
CCFLAGS += -I../..

BUILD   = $(VLIB) $(VSHLIB)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
