#----------------------------------------------------------------------------
#       $Id$
#
#       Top-level Makefile for the ACE toolkit libraries, tests, and
#       applications
#----------------------------------------------------------------------------

#### The "release" and "releaseall" targets can be used to create
#### the ACE and ACE+TAO kits.  By default, each creates a new beta
#### release.  To create a new minor or major release, add "REL=minor"
#### or "REL=major", respectively, to the make invocation.
####
#### NOTE: the REL modifier applies to _both_ ACE and TAO in
#### releaseall!
####
#### To create a new ACE minor release and a TAO beta release:
####   % make release REL=minor
####   % cd TAO
####   % make release
####
#### To see what make release* would do without actually doing it, add
#### "CHECK=-n" to the invocation.

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

INFO    = README \
          VERSION

DIRS    = ace \
          netsvcs \
          tests \
          apps \
          examples \
          performance-tests

CLONE   = Makefile \
          ace \
          apps \
          bin \
          etc \
          examples \
          include \
          netsvcs \
          performance-tests \
          tests

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU

# For the following to work you need to compile the
# $(ACE_ROOT)/bin/clone.cpp file and install it in your ~/bin
# directory (or some place similar).

.PHONY: clone
clone:
	@for dir in $(CLONE); \
        do \
                (clone -s $(ACE_ROOT)/$$dir $$dir) \
        done

CONTROLLED_FILES = ACE-INSTALL.html \
                ACE-categories \
                ACE-install.sh \
                ACE-lessons.html \
                ASNMP \
                BIBLIOGRAPHY \
                ChangeLog \
                ChangeLog-97a \
                ChangeLog-96b \
                ChangeLog-96a \
                ChangeLog-95 \
                ChangeLog-94 \
                ChangeLog-93 \
                FAQ \
                Makefile \
                README \
                STL \
                VERSION \
                ace \
                apps \
                bin \
                etc \
                examples \
                include \
                netsvcs \
                performance-tests \
                tests \
		WindozeCE

RELEASE_FILES = $(addprefix ACE_wrappers/,$(CONTROLLED_FILES)) \
                ACE_wrappers/ACE-INSTALL \
                ACE_wrappers/man

ALL_RELEASE_FILES = $(RELEASE_FILES) \
		    ACE_wrappers/TAO

RELEASE_LIB_FILES = \
                ACE_wrappers/STL \
                ACE_wrappers/VERSION \
                ACE_wrappers/ace \
                ACE_wrappers/include \
                ACE_wrappers/netsvcs \
                ACE_wrappers/tests

#### If creating the "official" ACE release:
#### 1) Check that the workspace is up-to-date, and bail out if not.
#### 2) Update the timestamp in the VERSION file.
#### 3) Update ace/Version.h.
#### 4) Add a ChangeLog entry.
#### 5) Tag the release.
#### Detect if we are creating the "official" release by looking at the PWD.
#### To disable this feature, add "TIMESTAMP=" to the make command line.
#### NOTE: if the version number in the VERSION file contains three components,
####       e.g., 4.1.5, then the third one will be incremented.  This assumes
####       that alpha/beta releases are numbered with three components, and
####       that final releases are not.  So, if the version number is, e.g.,
####       4.2, it will not be modified because it is assumed to be for a
####       final release.
ifeq ($(shell pwd),/project/adaptive/ACE_wrappers)
  TIMESTAMP = (DATE=`/usr/bin/date +"%a %b %d %T %Y"`; export DATE; \
              UPTODATE=`cvs -nq update $(CONTROLLED_FILES) | \
                egrep -v '/tests/log/' | perl -pi -e 's%/ACE_wrappers%%g; \
                s/$$/\\\n  /g'`; \
              if [ "$$UPTODATE" ]; then /pkg/gnu/bin/echo -e ERROR: workspace must be updated, and/or non-controlled files must be removed or added/committed: $$UPTODATE; exit 1; fi; \
              OLD_ACE_VERSION_TAG=ACE-`perl -ne  'if (/ACE version/) \
                { s/[^0-9]+(\d+)\.(\d+)\.(\d+).+/\1_\2_\3/; print }' VERSION`;\
              export OLD_ACE_VERSION_TAG; \
              ACE_VERSION=`perl -pi -e \
                'BEGIN { $$date=$$ENV{"DATE"} } \
                 s/(ACE version \d+\.\d+\.)(\d+)/sprintf("$$1%d",$$2+1)/e; \
                 if (s/(, released ).*/$$1$$date./) { \
                   ($$version = $$_) =~ s/^This is //; } \
                 END { print $$version  } ' VERSION;` export ACE_VERSION; \
              perl -i -e \
                'BEGIN {($$message = \
                           $$ENV{"DATE"} . "  " . \
                           $$ENV{"SIGNATURE"} . "  <" . \
                           $$ENV{"LOGNAME"} . \
                           "\@cs.wustl.edu>\n\n        * " . \
                           $$ENV{"ACE_VERSION"} . "\n"); \
                        $$message_printed = 0;} \
                 while (<>) { \
                   if ( ! $$message_printed++ ) { print "$$message\n"; } \
                   print; } ' ChangeLog; \
              echo $$ACE_VERSION | perl -ne ' \
               ($$version = $$_) =~ s/ACE version ([\d\.]+).*\n/$$1/; \
                 ($$major, $$minor, $$beta) = split /\./, $$version; \
                 $$beta = 0 if ! defined $$beta; \
                 print "// \$$Id\$$\n" . \
                       "// This is an automatically generated file.\n\n" . \
                       "\#define ACE_MAJOR_VERSION ($${major}u)\n" . \
                       "\#define ACE_MINOR_VERSION ($${minor}u)\n" . \
                       "\#define ACE_BETA_VERSION ($${beta}u)\n" . \
                       "\#define ACE_VERSION " . \
                         "\"$${major}.$${minor}.$${beta}\\0\"\n";' > \
                ace/Version.h; \
              cvs commit -m"$$ACE_VERSION" VERSION ChangeLog ace/Version.h; \
              chmod 644 VERSION ChangeLog ace/Version.h; \
              VERSION_TAG=ACE-`perl -ne  'if (/ACE version/) \
                { s/[^0-9]+(\d+)\.(\d+)\.(\d+).+/\1_\2_\3/; print }' VERSION`;\
              export VERSION_TAG; \
              cvs -q tag $$VERSION_TAG $(CONTROLLED_FILES) > /dev/null) &&
else
  TIMESTAMP =
endif

#### The following tar creation commands assume that cpio supports -H tar.
#### Old versions of cpio might not, but the version that's shipped with
#### Solaris 2.5.1, and gnu cpio 2.3, do support that option.

FILTER = -name CVS -prune -o ! -name '.\#*' ! -name '\#*' ! -name '*~' \
         ! -name '*.MAK' -print

cleanrelease-old: ACE-INSTALL
	@$(TIMESTAMP) (make realclean; cd ..; \
         find $(RELEASE_FILES) $(FILTER) | \
           cpio -o -H tar | gzip -9 > ACE.tar.gz; \
         chmod a+r ACE.tar.gz; mv ACE.tar.gz ./ACE_wrappers/)

release-old: ACE-INSTALL
	@$(TIMESTAMP) (cd ..; \
         find $(RELEASE_FILES) $(FILTER) | \
           cpio -o -H tar | gzip -9 > ACE.tar.gz; \
         find $(RELEASE_LIB_FILES) $(FILTER) | \
           cpio -o -H tar | gzip -9 > ACE-lib.tar.gz; \
         chmod a+r ACE.tar.gz ACE-lib.tar.gz; \
         mv ACE.tar.gz ACE-lib.tar.gz ./ACE_wrappers/)

releaseall-old: release-old
	@cd TAO; make release-old; chmod 644 VERSION ChangeLog
	@(cd ..; \
         find $(ALL_RELEASE_FILES) $(FILTER) | \
           cpio -o -H tar | gzip -9 > ACE+TAO.tar.gz; \
         chmod a+r ACE+TAO.tar.gz; \
         mv ACE+TAO.tar.gz ./ACE_wrappers/)

.PHONY: release releasetao releaseall ACE-INSTALL

REL = beta
CHECK =

#### The release target creates the ACE (only) kit.
release: ACE-INSTALL
	@$(ACE_ROOT)/bin/make_release $(CHECK) -k ace -t $(REL) \
	   -c "$(CONTROLLED_FILES)" -r "$(RELEASE_FILES)" \
	   -l "$(RELEASE_LIB_FILES)"

#### The following target is for use by the TAO Makefile.  It should not
#### be called directly from the command line.  The releasetao target
#### creates the combined ACE-TAO kit.
releasetao:
	@$(ACE_ROOT)/bin/make_release $(CHECK) -k ace+tao -t $(REL) \
	   -c "$(CONTROLLED_FILES)" -r "$(ALL_RELEASE_FILES)"

#### The releaseall target:
####   1) Creates the ACE kit.
####   2) Updates the TAO release (VERSION, ChangeLog, and tag) by
####      recursively invoking make release in the TAO directory.
####      The make then recursively invokes make releasetao in this
####      directory to create the combined ACE-TAO kit.
releaseall: release
	@cd TAO && make release REL=$(REL)

ACE-INSTALL: ACE-INSTALL.html
	@lynx -dump $< > $@
