#----------------------------------------------------------------------------
#
#       $Id$
#
#       Makefile for the Simple Quoter tutorials
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU

DIRS = \
  Client \
  Server  \
  Persistent \
  Impl-Repo 

  # ImprovedServer directory requires the
  # PortableServer::POA::ImplicitActivationPolicy, which is not in
  # available in the minimum CORBA configuration.
  ifeq ($(minimum_corba),0)
    DIRS += \
      ImprovedServer
  endif  # minimum_corba


ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU
