# $Id$

#----------------------------------------------------------------------------
#       Makefile for the libACE_Connection
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME   = libACE_Connection
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)


LIB_FILES = \
	Acceptor\
	Asynch_Acceptor\
	Asynch_IO\
	Asynch_IO_Impl\
	Caching_Strategies_T\
	Connector\
	LOCK_SOCK_Acceptor\
	POSIX_Asynch_IO\
	Sock_Connect\
	Strategies_T\
	WIN32_Asynch_IO

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

