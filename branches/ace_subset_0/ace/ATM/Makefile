# $Id$

#----------------------------------------------------------------------------
#       Makefile for the libACE_ATM
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME   = libACE_ATM
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)


LIB_FILES = \
	ATM_Acceptor\
	ATM_Addr\
	ATM_Connector\
	ATM_Params\
	ATM_QoS\
	ATM_Stream

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
