#----------------------------------------------------------------------------
#       $Id$
#
#       Makefile for the server-side ACE network services
#----------------------------------------------------------------------------

LIB     = libACEXML.a
SHLIB   = libACEXML.$(SOEXT)

FILES   =  Attributes_Def_Builder \
	   AttributesImpl \
	   CharStream \
	   DefaultHandler \
	   DTD_Manager \
	   Element_Def_Builder \
	   Env \
	   Exception \
	   FileCharStream \
	   InputSource \
	   LocatorImpl \
	   NamespaceSupport \
	   SAXExceptions \
	   Transcode \
	   Validator \
	   XMLFilterImpl

DEFS    = $(addsuffix .h,$(FILES))
LSRC    = $(addsuffix .cpp,$(FILES))

LIBS   += $(ACELIB)
CCFLAGS += -I ..

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
