##---------------------------------------------------------------------------
##       $Id$
##
##       Top-level Makefile for TAO
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

AUTOMAKE_OPTIONS = foreign

ACLOCAL = @ACLOCAL@
ACLOCAL_AMFLAGS = -I m4 -I ../m4

SUBDIRS = \
        TAO_IDL \
        tao \
	orbsvcs \
        docs

if BUILD_EXAMPLES
SUBDIRS += \
	examples
endif

if BUILD_TESTS
SUBDIRS += \
	performance-tests \
	interop-tests \
	tests
endif

SUBDIRS += \
	utils

## For some reason, automake doesn't include acconfig.h in the distribution
EXTRA_DIST =  \
  PROBLEM-REPORT-FORM \
  VERSION \
  COPYING.sun \
  LICENSE.sun \
  README.sun

## release:
##         $(MAKE) dist distdir=$(PACKAGE)$(VERSION)
release:
	$(MAKE) dist

## Directories to include in the distribution but which I am too lazy to
## create Makefile.am files for.  A Makefile.am for these directories isn't
## needed anyway.
TAO_EXTRA_DIST = docs

dist-hook:
	(cd $(top_srcdir); tar cf - $(TAO_EXTRA_DIST)) | (cd $(distdir); \
          tar xfBp -)
	list=`find $(distdir) -type d -name CVS -print`; for p in $$list; do \
          rm -rf $$p; \
        done

## Clean up some additional files/directories possibly created during
## the configure script tests.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations
