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
AUTOMAKE_OPTIONS = 1.4 #check-news

bin_SCRIPTS = ace-config

man_MANS = ace-config.1

## Build the library directories first.
## The docs directory is currently handled by the "dist-hook" makefile target.
SUBDIRS = \
  ace \
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
  BIBLIOGRAPHY \
  ChangeLog-93 \
  ChangeLog-94 \
  ChangeLog-95 \
  ChangeLog-96a \
  ChangeLog-96b \
  ChangeLog-97a \
  ChangeLog-97b \
  ChangeLog-98a \
  ChangeLog-98b \
  ChangeLog-99a \
  ChangeLog-99b \
  FAQ \
  PROBLEM-REPORT-FORM \
  VERSION \
  acconfig.h \
  ace-config.in \
  ace-config.1.in \
  aceConf.sh.in \
  ltcf-c.sh \
  ltcf-cxx.sh

DISTCLEANFILES = ace-config aceConf.sh ace-config.1 

## release:
##         $(MAKE) dist distdir=$(PACKAGE)$(VERSION)
release:
	$(MAKE) dist

## Put `exec' in the name because this should be installed by
## `install-exec', not `install-data'.
confexecdir=$(libdir)
confexec_DATA = aceConf.sh

## Directories to include in the distribution but which I am too lazy to
## create Makefile.am files for.  A Makefile.am for these directories isn't
## needed anyway.
ACE_EXTRA_DIST = bin docs m4

## Clean up some additional files/directories possibly created during
## the configure script tests.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations

## Make sure config.h doesn't get into the distribution
## since it may later intefere with the newly generated
## config.h when doing VPATH builds.
## We don't want it in the distribution, anyway, since
## config.h is platform specific.
dist-hook:
	-rm -f $(distdir)/ace/config.h
	(cd $(top_srcdir); tar cf - $(ACE_EXTRA_DIST)) | (cd $(distdir); \
          tar xfBp -)
	list=`find $(distdir) -type d -name CVS -print`; for p in $$list; do \
          rm -rf $$p; \
        done
