# $Id$

#----------------------------------------------------------------------------
#       Makefile for the libACE_Other
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIB_NAME   = libACE_Other
LIB      = $(LIB_NAME).a
SHLIB    = $(LIB_NAME).$(SOEXT)


FILES = \
	Local_Name_Space\
	Local_Name_Space_T\
	Msg_WFMO_Reactor\
	Name_Proxy\
	Name_Request_Reply\
	Name_Space\
	Naming_Context\
	Registry_Name_Space\
	Remote_Name_Space\
	TP_Reactor\
	XtReactor
	
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




