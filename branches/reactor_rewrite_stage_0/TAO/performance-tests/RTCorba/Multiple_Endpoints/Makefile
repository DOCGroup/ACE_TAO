#--------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------
ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

TAO_IDLFLAGS += -Ge 1

DIRS  = Common \
	Single_Endpoint \
        Orb_Per_Priority

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------
