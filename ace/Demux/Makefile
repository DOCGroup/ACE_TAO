# $Id$

#----------------------------------------------------------------------------
#       Makefile for the libACE_Demux
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBOS   = libACE_Demux
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
	Event_Handler\
	Event_Handler_T\
	FlReactor\
	POSIX_Proactor\
	Priority_Reactor\
	Proactor\
	QtReactor\
	Reactor\
	Reactor_Notification_Strategy\
	Select_Reactor_Base\
	Select_Reactor\
	Select_Reactor_T\
	SUN_Proactor\
	TkReactor\
	WFMO_Reactor\
	WIN32_Proactor

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

