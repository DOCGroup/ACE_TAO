#----------------------------------------------------------------------------
#	@(#)Makefile	1.1	10/18/96
#
#	Top-level Makefile for the ACE toolkit libraries, tests, and applications
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INFO	= README \
	  VERSION

DIRS	= ace \
	  netsvcs \
	  tests \
          apps \
	  examples \
	  performance-tests

CLONE	= Makefile \
	  ace \
	  apps \
	  bin \
	  examples \
	  include \
	  netsvcs \
	  performance-tests \
	  tests

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU

# For the following to work you need to compile the
# $(ACE_ROOT)/bin/clone.cpp file and install it in your ~/bin
# directory (or some place similar).

clone:
	@for dir in $(CLONE) ;\
	do \
		(clone -s $(ACE_ROOT)/$$dir $$dir) \
	done

RELEASE_FILES = ACE_wrappers/ACE-INSTALL.html \
		ACE_wrappers/ACE-INSTALL \
		ACE_wrappers/ACE-categories \
	        ACE_wrappers/ACE-install.sh \
	        ACE_wrappers/ACE-lessons.html \
		ACE_wrappers/ASNMP \
	        ACE_wrappers/BIBLIOGRAPHY \
	        ACE_wrappers/ChangeLog-97b \
	        ACE_wrappers/ChangeLog-97a \
	        ACE_wrappers/ChangeLog-96b \
	        ACE_wrappers/ChangeLog-96a \
	        ACE_wrappers/ChangeLog-95 \
	        ACE_wrappers/ChangeLog-94 \
	        ACE_wrappers/ChangeLog-93 \
		ACE_wrappers/FAQ \
		ACE_wrappers/Makefile \
		ACE_wrappers/README \
		ACE_wrappers/STL \
		ACE_wrappers/VERSION \
		ACE_wrappers/ace \
		ACE_wrappers/apps \
		ACE_wrappers/bin \
		ACE_wrappers/examples \
		ACE_wrappers/include \
		ACE_wrappers/man \
		ACE_wrappers/netsvcs \
		ACE_wrappers/performance-tests \
		ACE_wrappers/tests

RELEASE_LIB_FILES = \
		ACE_wrappers/STL \
		ACE_wrappers/VERSION \
		ACE_wrappers/ace \
		ACE_wrappers/include \

#### If creating the "official" ACE release:
#### 1) Check that the workspace is up-to-date, and bail out if not.
#### 2) Update the timestamp in the VERSION file.
#### 3) Add a ChangeLog entry to the newest ChangeLog plain file.
#### Detect if we are creating the "official" release by looking at the PWD.
#### To disable this feature, add "TIMESTAMP=" to the make command line.
#### NOTE: if the version number in the VERSION file contains three components,
####       e.g., 4.1.5, then the third one will be incremented.  This assumes
####       that alpha/beta releases are numbered with three components, and
####       that final releases are not.  So, if the version number is, e.g.,
####       4.2, it will not be modified because it is assumed to be for a
####       final release.
ifeq ($(shell pwd),/project/adaptive/ACE_wrappers)
  TIMESTAMP = (CHANGELOG=`/pkg/gnu/bin/find -name 'ChangeLog*' -maxdepth 1 \
                -type f | xargs ls -1t | head -1`; export CHANGELOG; \
              if [ -z "$$CHANGELOG" ]; then echo unable to find latest ChangeLog file; exit 1; fi; \
              DATE=`/usr/bin/date +"%a %b %d %T %Y"`; export DATE; \
              cd ..; UPTODATE=`cvs -nq update $(RELEASE_FILES) | egrep -v '/tests/log/' | perl -pi -e 's%/ACE_wrappers%%g; s/$$/\\\n  /g'`; cd ACE_wrappers; \
              if [ "$$UPTODATE" ]; then /pkg/gnu/bin/echo -e ERROR: workspace must be updated, and/or non-controlled files must be removed or added/committed: $$UPTODATE; exit 1; fi; \
              ACE_VERSION=`perl -pi -e \
                'BEGIN { $$date=$$ENV{"DATE"} } \
                 s/(ACE version \d+\.\d+\.)(\d+)/sprintf("$$1%d",$$2+1)/e; \
                 if (s/(, released ).*/$$1$$date./) { \
                   ($$version = $$_) =~ s/^This is //; } \
                 END { print $$version  } ' VERSION;` export ACE_VERSION; \
	      lynx -dump ACE-INSTALL.html > ACE-INSTALL \
              perl -i -e \
                'BEGIN {($$message = \
                           $$ENV{"DATE"} . "  " . \
                           $$ENV{"SIGNATURE"} . "  <" . \
                           $$ENV{"LOGNAME"} . "\@cs.wustl.edu>\n\n\t* " . \
                           $$ENV{"ACE_VERSION"} . "\n"); \
                        $$message_printed = 0;} \
                 while (<>) { \
                   if ( ! $$message_printed++ ) { print "$$message\n"; } \
                   print; } ' $$CHANGELOG; \
              cvs commit -m"$$ACE_VERSION" VERSION $$CHANGELOG; \
              chmod 644 VERSION $$CHANGELOG) &&
else
  TIMESTAMP =
endif

#### The following tar creation commands assume that cpio supports -H tar.
#### Old versions of cpio might not, but the version that's shipped with
#### Solaris 2.5.1, and gnu cpio 2.3, do support that option.

FILTER = -name CVS -prune -o ! -name '.\#*' ! -name '\#*' ! -name '*~' -print

cleanrelease:
	@$(TIMESTAMP) (make realclean; cd ..; \
	 find $(RELEASE_FILES) $(FILTER) | cpio -o -H tar | gzip -9 > ACE.tar.gz; \
	 chmod a+r ACE.tar.gz; mv ACE.tar.gz ./ACE_wrappers/)

release:
	@$(TIMESTAMP) (cd ..; \
	 find $(RELEASE_FILES) $(FILTER) | cpio -o -H tar | gzip -9 > ACE.tar.gz; \
	 find $(RELEASE_LIB_FILES) $(FILTER) | cpio -o -H tar | gzip -9 > ACE-lib.tar.gz; \
	 chmod a+r ACE.tar.gz ACE-lib.tar.gz; \
	 mv ACE.tar.gz ACE-lib.tar.gz ./ACE_wrappers/)

