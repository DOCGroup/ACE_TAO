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

bin_SCRIPTS = ace-config

man_MANS = ace-config.1

## Build the library directories first.
## The docs directory is currently handled by the "dist-hook" makefile target.
SUBDIRS = \
  ace \
  apps \
  tests
##  netsvcs \
##  man

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

## Additional flags to pass to `aclocal' when invoked by the generated
## Makefile.  In particular, add ACE's `m4' directory to the macro
## search path.
ACLOCAL_AMFLAGS = -I m4

## If any of these files are modified, `configure' will be
## regenerated.
CONFIGURE_DEPENDENCIES = \
  m4/ace.m4 \
  m4/acinclude.m4 \
  m4/compiler.m4 \
  m4/config_h.m4 \
  m4/features.m4 \
  m4/platform.m4 \
  m4/subsets.m4 \
  m4/threads.m4

## For some reason, automake doesn't include acconfig.h in the distribution
EXTRA_DIST = \
  ACE-INSTALL.html \
  ACE-install.sh \
  FAQ \
  PROBLEM-REPORT-FORM \
  VERSION \
  ace-config.in \
  ace-config.1.in \
  aceConf.sh.in

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
ACE_EXTRA_DIST = ChangeLogs bin docs m4

## Automake recognized target.  Appends these rules to the "all"
## target.
all-local:
	test -d bin || ($(LN_S) $(top_srcdir)/bin bin && touch ace-bin-stamp)

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

## Clean up files and directories created by this Makefile.
distclean-local:
	-test -f ace-bin-stamp && rm bin ace-bin-stamp
