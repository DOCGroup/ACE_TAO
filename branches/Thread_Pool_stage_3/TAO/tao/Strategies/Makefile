#----------------------------------------------------------------------------
#
#      $Id$
#
#       Makefile for TAO_Strategies
#----------------------------------------------------------------------------

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

MAKEFILE = Makefile
LIBNAME  = libTAO_Strategies
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

ACE_SHLIBS = -lTAO -lACE

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

CPP_SRCS += \
  DIOP_Factory \
  DIOP_Profile \
  DIOP_Transport \
  DIOP_Connector \
  DIOP_Acceptor \
  DIOP_Connection_Handler \
  DIOP_Endpoint \
  UIOP_Factory \
  UIOP_Lite_Factory \
  UIOP_Profile \
  UIOP_Transport \
  UIOP_Connector \
  UIOP_Acceptor \
  UIOP_Connection_Handler \
  UIOP_Endpoint \
  SHMIOP_Factory \
  SHMIOP_Profile \
  SHMIOP_Transport \
  SHMIOP_Connector \
  SHMIOP_Acceptor \
  SHMIOP_Connection_Handler \
  SHMIOP_Endpoint \
  TAO_Strategies_Internal \
  uiop_endpoints \
  advanced_resource \
  LFU_Connection_Purging_Strategy \
  FIFO_Connection_Purging_Strategy \
  NULL_Connection_Purging_Strategy \
  LF_Strategy_Null

IDL_SRC = \
        $(addsuffix S.cpp, $(IDL_FILES)) \
        $(addsuffix C.cpp, $(IDL_FILES))
FILES = $(CPP_SRCS)
DEFS  = $(addsuffix .h,$(FILES))
LSRC  = $(addsuffix .cpp,$(FILES))

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets (and local hacks)
#----------------------------------------------------------------------------

LDFLAGS += -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
