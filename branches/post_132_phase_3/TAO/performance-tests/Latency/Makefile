#----------------------------------------------------------------------------
#
#       $Id$
#
#       Makefile for the TAO "Latency" performance-tests
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

DIRS = \
  AMI \
  AMH \
  DII \
  Deferred \
  DSI \
  Single_Threaded \
  Thread_Pool \
  Thread_Per_Connection

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
