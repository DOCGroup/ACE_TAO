#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for the Reactor Server Logging Daemon
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN   	= server

FILES 	= 

LSRC	= $(addsuffix .cpp,$(FILES))
LOBJ	= $(addsuffix .o,$(FILES))
SHOBJ	= $(addsuffix .so,$(FILES))

LDLIBS	=  $(addprefix .shobj/,$(SHOBJ)) 

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
	shar -T hdr bodies *.pre *.pst > combine.shar && rm -f hdr bodies *.pre *.pst

UNSHAR : #
	sh combine.shar

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

.obj/server.o .shobj/server.so: server.cpp acceptor.h logger.h

