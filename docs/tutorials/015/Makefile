
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
	[ -f hdr ] || $(MAKE) UNSHAR
	perl ../combine *.pre
	rm -f *.bdy

SHAR : #
	[ ! -f combine.shar ] || exit 1
	shar -T hdr links *.pre *.pst > combine.shar
	rm -f hdr links *.pre *.pst

UNSHAR : #
	sh combine.shar
