##---------------------------------------------------------------------------
##       $Id$
##
##       Top-level Makefile for the ACE toolkit libraries, tests, and
##       applications
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4 check-news

bin_SCRIPTS = ace-config

man_MANS = ace-config.1

## Build the library directories first.
## The docs directory is currently handled by the "dist-hook" makefile target.
SUBDIRS = ace \
          netsvcs \
          tests \
          apps \
          man

##SUBDIRS = \
##          docs \
##          apps \
##          examples \
##          performance-tests \
##          bin \
##          ASNMP \
##          os-patches \
##          etc \
##          includes

## For some reason, automake doesn't include acconfig.h in the distribution
EXTRA_DIST = \
        ACE-INSTALL.html \
        ACE-install.sh \
        ACE-lessons.html \
        ACE.DSW \
        BIBLIOGRAPHY \
        BUG-REPORT-FORM \
        ChangeLog-93 \
        ChangeLog-94 \
        ChangeLog-95 \
        ChangeLog-96a \
        ChangeLog-96b \
        ChangeLog-97a \
        ChangeLog-97b \
        ChangeLog-98a \
        ChangeLog-98b \
        FAQ \
        README.aceconf \
        VERSION \
        acconfig.h \
        ace-config.in \
        ace-config.1.in \
        aceConf.sh.in \
        ACE.ifnames \
        ace-diff-config.in

DISTCLEANFILES = ace-config aceConf.sh ace-config.1 ace-diff-config \
                 ace-diff-config.results ace-diff-config.results.bak

MAINTAINERCLEANFILES = ACE.ifnames

## release:
##         $(MAKE) dist distdir=$(PACKAGE)$(VERSION)
release:
	$(MAKE) dist

## Put `exec' in the name because this should be installed by
## `install-exec', not `install-data'.
confexecdir=$(libdir)
confexec_DATA = aceConf.sh

## We create aceConf.sh here and not from configure because we want
## to get the paths expanded correctly.  Macros like srcdir are given
## the value NONE in configure if the user doesn't specify them (this
## is an autoconf feature, not a bug).
aceConf.sh: aceConf.sh.in Makefile
## Use sed and then mv to avoid problems if the user interrupts.
	sed -e 's,\@ACE_LIBDIR\@,$(ACE_LIBDIR),g' \
            -e 's,\@ACE_INCLUDEDIR\@,$(ACE_INCLUDEDIR),g' \
            -e 's,\@ACE_LIBS\@,$(ACE_LIBS),g' \
            -e 's,\@CXXFLAGS\@,$(CXXFLAGS),g' \
              < $(srcdir)/aceConf.sh.in > aceConf.tmp \
          && mv aceConf.tmp aceConf.sh

ace-config.1: ace-config.1.in Makefile
## Use sed and then mv to avoid problems if the user interrupts.
	sed -e 's,\@libdir\@,$(libdir),g' \
              < $(srcdir)/ace-config.1.in > ace-config.1.tmp \
          && mv ace-config.1.tmp ace-config.1

## The file containing the list of preprocessor conditional macros used by ACE
noinst_DATA = ACE.ifnames

ACE.ifnames: ChangeLog acconfig.h
	ifnames ace/*.{h,i,cpp} | sed -e 's/ .*//g' > ACE.ifnames


## Directories to include in the distribution but which I am too lazy to
## create Makefile.am files for.  A Makefile.am for these directories isn't
## needed anyway.
ACE_EXTRA_DIST = ./bin ./docs ./m4

## Make sure config.h doesn't get into the distribution
## since it may later intefere with the newly generated
## config.h when doing VPATH builds.
## We don't want it in the distribution, anyway, since
## config.h is platform specific.
dist-hook:
	-rm -f $(distdir)/ace/config.h
	tar cf - $(ACE_EXTRA_DIST) | (cd $(distdir); tar xfBp -)
	list=`find $(distdir) -type d -name CVS -print`; for p in $$list; do \
          rm -rf $$p; \
        done
