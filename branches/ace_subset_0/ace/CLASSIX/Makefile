# $Id$

#----------------------------------------------------------------------------
#       Makefile for the libACE_ATM
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME   = libACE_CLASSIX
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)


FILES = \
	CLASSIX_Addr\
	CLASSIX_CLD_Connector\
	CLASSIX_CO_Acceptor\
	CLASSIX_CO_Connector\
	CLASSIX_Dgram_Mcast\
	CLASSIX_Group\
	CLASSIX_Group_Dynamic\
	CLASSIX_Group_Stamp\
	CLASSIX_OS\
	CLASSIX_Peer_SAP\
	CLASSIX_Port\
	CLASSIX_Port_Core\
	CLASSIX_Port_Default\
	CLASSIX_SAP\
	CLASSIX_Select_Reactor

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
	$(RM) -f $(LIBNAME).a $(LIBNAME).so

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

