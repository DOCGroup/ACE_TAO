# $Id$

#----------------------------------------------------------------------------
#       Makefile for the libACE_Timer
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIB_NAME   = libACE_Timer
LIB      = $(LIB_NAME).a
SHLIB    = $(LIB_NAME).$(SOEXT)


FILES = \
	Basic_Stats\
	High_Res_Timer\
	Profile_Timer\
	System_Time\
	Timeprobe\
	Timeprobe_T\
	Time_Request_Reply\
	Timer_Hash\
	Timer_Hash_T\
	Timer_Heap\
	Timer_Heap_T\
	Timer_List\
	Timer_List_T\
	Timer_Queue_Adapters\
	Timer_Queue\
	Timer_Queue_T\
	Timer_Wheel\
	Timer_Wheel_T

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

####
#### ACE_COMPONENTS support.
####

LSRC  = $(addsuffix .cpp,$(FILES))

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

INSTALL =

clean:
	$(RM) -f $(LIB_NAME).a $(LIB_NAME).so

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------



#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



