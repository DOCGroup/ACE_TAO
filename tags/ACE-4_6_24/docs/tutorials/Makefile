
# $Id$

all clean realclean UNSHAR SHAR HTML CLEAN : #
	for i in * ; do \
		[ -f $$i/Makefile ] || continue ; \
		( cd $$i ; $(MAKE) $@ ) ; \
	done

