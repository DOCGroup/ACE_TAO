#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for the Reactor version of the Server Logging Daemon
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN   	= server

VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD	= $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

HTML : #
	[ -f hdr ] || $(MAKE) UNSHAR
	perl ../combine *.pre ; chmod +r *.html

SHAR : #
	[ ! -f combine.shar ] || exit 1
	shar -T hdr bodies *.pre > combine.shar && rm -f hdr bodies *.pre *.pst

UNSHAR : #
	sh combine.shar

CLEAN : realclean
	rm -f hdr bodies *.pre *.pst .depend

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
