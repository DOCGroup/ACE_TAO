
# $Id$

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN    = client server

FILES  = mpool

BUILD  = $(VBIN)

BSRC = $(addsuffix .cpp,$(BIN))

SRC += $(addsuffix .cpp,$(FILES))

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

rename : #
	for i in *.cxx ; do	\
		n=`expr "$$i" : "\(.*\).cxx"`	;\
		mv $$i $$n.cpp	;\
	done

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

Depend : #
	$(MAKE) SRC="$(SRC) $(BSRC)" depend
	perl ../fix.Makefile

.depend : #
	touch .depend

HTML : #
	[ -f hdr ] || $(MAKE) UNSHAR
	perl ../combine *.pre

SHAR : #
	[ ! -f combine.shar ] || exit 1
	shar -T hdr bodies *.pre *.pst > combine.shar && rm -f hdr bodies *.pre *.pst

UNSHAR : #
	sh combine.shar

CLEAN : realclean
	rm -f hdr bodies *.pre *.pst .depend

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

include .depend
