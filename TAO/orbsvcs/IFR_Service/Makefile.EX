#----------------------------------------------------------------------------
#       $Id$
#
#       Makefile for the tao_idl executable
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

LDLIBS = -lTAO_IFR_BE_DLL -lTAO_IDL_FE_DLL 

BIN = tao_ifr

VPATH = .

MAKEFILE=Makefile.EX

FILES = tao_ifr \
	drv_init_ifr \
        drv_private_ifr \
        drv_args_ifr \
        drv_preproc_ifr

SRC = $(addsuffix .cpp,$(FILES))

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

CPPFLAGS += -I../TAO_IDL/include -I../TAO_IDL/fe

ifeq ($(TAO_IDL_PREPROCESSOR),)
	CPPFLAGS += -DTAO_IDL_PREPROCESSOR=\"$(strip $(CXX))\"
else
	CPPFLAGS += -DTAO_IDL_PREPROCESSOR=\"$(TAO_IDL_PREPROCESSOR)\"
endif # !def TAO_IDL_PREPROCESSOR

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

