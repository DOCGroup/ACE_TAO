#----------------------------------------------------------------------------
#
# $Id$
#
# Makefile for C++NPv2 TP_Reactor_Logging_Server example program,
# chapter 4.
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN     = TP_Reactor_Logging_Server

FILES	= Logging_Acceptor \
          Logging_Event_Handler \
          Logging_Event_Handler_Ex \
          Logging_Handler

SRC	= $(addsuffix .cpp,$(FILES))
OBJ	= $(addsuffix .o,$(FILES))

BUILD   = $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU
