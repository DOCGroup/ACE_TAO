#----------------------------------------------------------------------------
#	$Id$
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN    = server directed_client broadcast_client

FILES  = 

BUILD	= $(VBIN)

HDR = *.h

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

Indent : #
	for i in $(SRC) $(HDR) ; do \
		indent -npsl -bli0 -l80 -fca -fc1 -cli0 -cdb < $$i | \
			sed -e 's/: :/::/g'  \
			    -e 's/^.*\(public:\)/\1/' \
			    -e 's/^.*\(protected:\)/\1/' \
			    -e 's/^.*\(private:\)/\1/' \
			    -e 's/:\(public\)/ : \1/' \
			    -e 's/:\(protected\)/ : \1/' \
			    -e 's/:\(private\)/ : \1/' \
			> $$i~ ;\
		mv $$i~ $$i ;\
	done

Depend : depend
	perl ../fix.Makefile

.depend : #
	touch .depend

HTML : #
	[ -f hdr ] || $(MAKE) UNSHAR
	perl ../combine *.pre ; chmod +r *.html

SHAR : #
	[ ! -f combine.shar ] || exit 1
	shar -T hdr bodies *.pre *.pst > combine.shar && $(RM) hdr bodies *.pre *.pst

UNSHAR : #
	sh combine.shar

CLEAN : realclean
	$(RM) hdr bodies *.pre *.pst .depend

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

	# Don't put anything below here.  Between the "depend" target and fix.Makefile
	# it's guaranteed to be lost!

	# This is inserted by the fix.Makefile script
include .depend
