#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME  = libTAO_IOR_CORBANAME
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

# These are headers for things which are exported and must be
# installed.  (Currently not used).
PUB_HDRS = \
	CORBANAME_Parser
FILES= \
	CORBANAME_Parser

LSRC    = $(addsuffix .cpp,$(FILES))

DEFS    = $(addsuffix .h,$(PUB_HDRS))

ACE_SHLIBS = -lACE

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets (and local hacks)
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
