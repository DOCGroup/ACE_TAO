# $Id$

#----------------------------------------------------------------------------
#       Makefile for the libACE_Svcconf
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME   = libACE_Svcconf
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)


LIB_FILES = \
	DLL\
	Dynamic_Service\
	Dynamic_Service_Base\
	Parse_Node\
	Service_Config\
	Service_Manager\
	Service_Object\
	Service_Repository\
	Service_Templates\
	Service_Types\
	Shared_Object\
	Svc_Conf_Lexer_Guard\
	Svc_Conf_l\
	Svc_Conf_y\
	Svc_Handler

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

####
#### ACE_COMPONENTS support.
####
FILES += $(LIB_FILES)

LSRC  = $(addsuffix .cpp,$(FILES))

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

INSTALL =

clean:
	$(RM) -f $(LIBNAME).a $(LIBNAME).so

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

