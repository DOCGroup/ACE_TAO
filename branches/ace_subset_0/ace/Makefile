#----------------------------------------------------------------------------
#       $Id$
#
#       Makefile for the entire ACE release
#----------------------------------------------------------------------------

MAKEFILE = Makefile
#LIBACE   = libACE
#LIB      = $(LIBACE).a
#SHLIB    = $(LIBACE).$(SOEXT)

SUBDIRS= \
  ATM \
  Connection \
  Demux \
  Event_Handler \
  IPC \
  Logging \
  Memory \
  OS \
  Other \
  QoS \
  RMCast \
  Sockets \
  SSL \
  Streams \
  Svcconf \
  Threads \
  Timer \
  Token \
  Utils 

all:
	@for subdir in $(SUBDIRS) ; do \
		if test -d $$subdir ; \
		then \
			echo "Making all in $$subdir"; \
			( cd $$subdir && $(MAKE) all ) || exit 1 ;\
		fi ; \
	done

clean:
	@for subdir in $(SUBDIRS) ; do \
		if test -d $$subdir ; \
		then \
			echo "Making clean in $$subdir"; \
			( cd $$subdir && $(MAKE) clean ) || exit 1 ;\
		fi ; \
	done
	$(RM) -rf *.so *.a

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
#### Disable installs in this ($(ACE_ROOT)/ace) directory, because this
#### is the (default) destination of installs anyways.  This line prevents
#### creation of a link from the ACE library to itself when the build of
#### the ACE library fails.
INSTALL =
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
