#----------------------------------------------------------------------------
#       $Id$
#
#       Makefile for NT_Service
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

BIN     = ntsvc

SRC    = main.cpp ntsvc.cpp

LDLIBS  =

VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD   = $(VBIN)

INSTALL =

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include         $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include         $(ACE_ROOT)/include/makeinclude/macros.GNU
include         $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include         $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include         $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include         $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
