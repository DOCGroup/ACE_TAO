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

ifdef AMI
TAO_IDLFLAGS += -GC
endif # AMI

IDL_SRCS = test
IDL_FILES = \
        $(addsuffix S, $(IDL_SRCS)) \
        $(addsuffix C, $(IDL_SRCS))

BIN   = idl_stubs

DIRS  = Single_Endpoint \
        Orb_Per_Priority \
        Endpoint_Per_Priority

all: all.local all.nested
#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

idl_stubs: $(addsuffix .h, $(IDL_FILES))

realclean:
	-$(RM) $(foreach file, $(IDL_SRCS), $(foreach ext, $(IDL_EXT), $(file)$(ext)))
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
