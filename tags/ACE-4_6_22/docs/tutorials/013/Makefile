
# $Id$

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN    = message_queue

FILES  = task block work mld

BUILD  = $(VBIN)

SRC  = $(addsuffix .cpp,$(BIN))
SRC += $(addsuffix .cpp,$(FILES))

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
		indent -npsl -l80 -fca -fc1 -cli0 -cdb -ts2 -bl -bli0 < $$i | \
			sed -e 's/: :/::/g'  \
			    -e 's/^.*\(public:\)/\1/' \
			    -e 's/^.*\(protected:\)/\1/' \
			    -e 's/^.*\(private:\)/\1/' \
			    -e 's/:\(public\)/ : \1/' \
			    -e 's/:\(protected\)/ : \1/' \
			    -e 's/:\(private\)/ : \1/' \
			    -e 's/	/  /g' \
			> $$i~ ;\
		mv $$i~ $$i ;\
	done

Depend : depend
	perl ../fix.Makefile

HTML : #
	[ -f hdr ] || $(MAKE) UNSHAR
	perl ../combine *.pre
	chmod +r *.html

SHAR : #
	[ ! -f combine.shar ] || exit 1
	shar -T hdr bodies *.pre *.pst > combine.shar && rm -f hdr bodies *.pre *.pst

UNSHAR : #
	sh combine.shar

CLEAN : realclean
	rm -f hdr bodies *.pre *.pst .depend

.depend : #
	touch .depend

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

include .depend
