#----------------------------------------------------------------------------
#
#       $Id$
#
#       Makefile for the TAO Content_Server set of examples.
#
#----------------------------------------------------------------------------

DIRS  = SMI_Iterator

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

ifneq ($(ami), 0)
  DIRS += \
	AMI_Iterator \
	AMI_Observer
endif  # ami
