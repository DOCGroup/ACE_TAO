#----------------------------------------------------------------------------
#	$Id$
#
#	Common makefile definitions for all applications
#
#----------------------------------------------------------------------------

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

LDFLAGS +=  -L$(TAO_ROOT)/tao
INCLDIRS += -I$(TAO_ROOT)
