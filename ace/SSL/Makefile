# $Id$

#----------------------------------------------------------------------------
#       Makefile for the libACE_OS
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBOS   = libACE_SSL
LIB      = $(LIBOS).a
SHLIB    = $(LIBOS).$(SOEXT)


FILES = \
	SSL_Asynch_BIO\
	SSL_Asynch_Stream\
	SSL_Context\
	SSL_SOCK\
	SSL_SOCK_Acceptor\
	SSL_SOCK_Connector\
	SSL_SOCK_Stream

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
	$(RM) -f $(LIBOS).a $(LIBOS).so

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------



#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


