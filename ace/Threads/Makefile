# $Id$

#----------------------------------------------------------------------------
#       Makefile for the libACE_Threads
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBOS   = libACE_Threads
LIB      = $(LIBOS).a
SHLIB    = $(LIBOS).$(SOEXT)


#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

####
#### ACE_COMPONENTS support.
####
FILES += \
	Activation_Queue\
	Atomic_Op\
	File_Lock\
	Process\
	Process_Manager\
	Process_Mutex\
	Process_Semaphore\
	RW_Process_Mutex\
	Synch\
	Synch_Options\
	Synch_T\
	Thread_Adapter\
	Thread_Control\
	Thread\
	Thread_Exit\
	Thread_Manager\
	Token

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

