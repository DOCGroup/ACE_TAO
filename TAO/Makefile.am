##---------------------------------------------------------------------------
##       $Id$
##
##       Top-level Makefile for TAO
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4 check-news

bin_SCRIPTS = tao-config

man_MANS = tao-config.1

SUBDIRS = \
        tao \
        TAO_IDL \
        orbsvcs

## For some reason, automake doesn't include acconfig.h in the distribution
EXTRA_DIST = BUG-REPORT-FORM VERSION \
             COPYING.sun LICENSE.sun README.sun README.taoconf \
             tao-config.in taoConf.sh.in tao-config.1.in

DISTCLEANFILES = tao-config taoConf.sh tao-config.1

## release:
##         $(MAKE) dist distdir=$(PACKAGE)$(VERSION)
release:
	$(MAKE) dist

## Put `exec' in the name because this should be installed by
## `install-exec', not `install-data'.
confexecdir=$(libdir)
confexec_DATA = taoConf.sh

## We create taoConf.sh here and not from configure because we want
## to get the paths expanded correctly.  Macros like srcdir are given
## the value NONE in configure if the user doesn't specify them (this
## is an autoconf feature, not a bug).
taoConf.sh: taoConf.sh.in Makefile
## Use sed and then mv to avoid problems if the user interrupts.
	sed -e 's,\@TAO_LIBDIR\@,$(TAO_LIBDIR),g' \
            -e 's,\@TAO_INCLUDEDIR\@,$(TAO_INCLUDEDIR),g' \
            -e 's,\@TAO_LIBS\@,$(TAO_LIBS),g' \
            -e 's,\@TAO_CXXFLAGS\@,$(TAO_CXXFLAGS),g' \
              < $(srcdir)/taoConf.sh.in > taoConf.tmp \
          && mv taoConf.tmp taoConf.sh

tao-config.1: tao-config.1.in Makefile
## Use sed and then mv to avoid problems if the user interrupts.
	sed -e 's,\@libdir\@,$(libdir),g' \
              < $(srcdir)/tao-config.1.in > tao-config.1.tmp \
          && mv tao-config.1.tmp tao-config.1

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

