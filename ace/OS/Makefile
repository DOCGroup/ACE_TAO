# $Id$

#----------------------------------------------------------------------------
#       Makefile for the libACE_OS
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBOS   = libACE_OS
LIB      = $(LIBOS).a
SHLIB    = $(LIBOS).$(SOEXT)


OS_FILES = \
	Base_Thread_Adapter\
	Basic_Types\
	Date_Time\
	gethrtime\
	Handle_Ops\
	Handle_Set\
	OS\
	OS_Dirent\
	OS_Errno\
	OS_Log_Msg_Attributes\
	OS_Memory\
	OS_QoS\
	OS_String\
	OS_Thread_Adapter\
	OS_TLI\
	Sched_Params\
	Thread_Hook

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

####
#### ACE_COMPONENTS support.
####
FILES += $(OS_FILES)

LSRC  = $(addsuffix .cpp,$(FILES))

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

INSTALL =

clean:
	$(RM) -f $(LIBOS).a $(LIBOS).so

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------



#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


