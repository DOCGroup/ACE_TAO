
# $Id$

all clean realclean : #
	for i in * ; do \
		[ -f $$i/Makefile ] || continue ; \
		( cd $$i ; $(MAKE) $@ ) ; \
	done

