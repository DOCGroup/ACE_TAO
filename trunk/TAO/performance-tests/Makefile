#----------------------------------------------------------------------------
#
#       $Id$
#
#       Makefile for the TAO performance-tests
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

DIRS  = Cubit \
        POA \
        Pluggable \
        RTCorba \
	AMI_Latency \
	Deferred_Latency \
	DII_Latency \
	DSI_Latency \
	Single_Threaded_Latency \
	AMH_Single_Threaded_Latency \
	Thread_Pool_Latency \
	Thread_Per_Connection_Latency \
	Callback \
	Throughput

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
