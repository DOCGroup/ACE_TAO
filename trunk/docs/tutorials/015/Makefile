
# $Id$

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN    = client server

all clean realclean : #
	$(MAKE) -f Makefile.client $@
	$(MAKE) -f Makefile.server $@

client server : #
	$(MAKE) -f Makefile.$@ all

Depend : #
	$(MAKE) -f Makefile.client $@

HTML : #
	perl ../combine *.pre

include .depend
