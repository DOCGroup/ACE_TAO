#----------------------------------------------------------------------------
#	$Id$
#
#	Common makefile definitions for all applications
#
#----------------------------------------------------------------------------

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

LDFLAGS := $(patsubst -L$(ACE_ROOT)/ace, -L$(TAO_ROOT)/tao -L$(ACE_ROOT)/ace, $(LDFLAGS))
CPPFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/tao/compat $(TSS_ORB_FLAG)#-H

