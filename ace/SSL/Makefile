#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

ifndef ACE_ROOT
default:
	@echo 'You must set ACE_ROOT'
endif # ! ACE_ROOT

MAKEFILE = Makefile
LIBNAME  = libACE_SSL
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

FILES = \
  SSL_Context \
  SSL_SOCK \
  SSL_SOCK_Acceptor \
  SSL_SOCK_Connector \
  SSL_SOCK_Stream

LSRC    = $(addsuffix .cpp,$(FILES))

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Local targets (and local hacks)
#----------------------------------------------------------------------------

ifeq ($(HPUX_VERS),)
ACE_SHLIBS = -lACE -lssl -lcrypto
endif # HPUX_VERS

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
