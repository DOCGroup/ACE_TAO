#----------------------------------------------------------------------------
#       $Id$
#
#       Top-level Makefile for the ACE toolkit libraries, tests, and
#       applications
#----------------------------------------------------------------------------

#### The "release" and "releaseall" targets can be used to create
#### the ACE and ACE+TAO kits.  By default, each creates a new beta
#### release.  To create a new minor or major release, add REL=minor
#### or REL=major, respectively, to the make invocation.
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
#### CHECK=-n to the invocation.
####
#### By default, make release* will regenerate the contents of the man
#### directory.  To suppress that, add GENERATE_MAN_PAGES= to your
#### make release or make releaseall invocation.

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
          TAO \
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

CONTROLLED_FILES = \
        ACE-INSTALL.html \
        ACE-install.sh \
        ASNMP \
        BIBLIOGRAPHY \
        COPYING \
        ChangeLog \
        ChangeLog-98a \
        ChangeLog-98b \
        ChangeLog-97b \
        ChangeLog-97a \
        ChangeLog-96b \
        ChangeLog-96a \
        ChangeLog-95 \
        ChangeLog-94 \
        ChangeLog-93 \
        FAQ \
        Makefile \
        PROBLEM-REPORT-FORM \
        README \
        VERSION \
        ace \
        apps \
        bin \
        docs \
        etc \
        examples \
        include \
        netsvcs \
        os-patches \
        performance-tests \
        tests \
        WindozeCE

RELEASE_FILES = \
        $(addprefix ACE_wrappers/,$(CONTROLLED_FILES)) \
        ACE_wrappers/ACE-INSTALL \
        ACE_wrappers/man

ALL_RELEASE_FILES = \
        $(RELEASE_FILES) \
        ACE_wrappers/TAO

RELEASE_LIB_FILES = \
        ACE_wrappers/VERSION \
        ACE_wrappers/ace \
        ACE_wrappers/bin \
        ACE_wrappers/etc \
        ACE_wrappers/include \
        ACE_wrappers/netsvcs \
        ACE_wrappers/tests

.PHONY: release releasetao releaseall

REL = beta
CHECK =
GENERATE_MAN_PAGES = $(ACE_ROOT)/bin/generate_man_pages

#### The release target creates the ACE (only) kit.
release: ACE-INSTALL
	@$(GENERATE_MAN_PAGES)
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
	@cd TAO  &&  $(MAKE) release REL=$(REL)

ACE-INSTALL: ACE-INSTALL.html
	@lynx -dump $< > $@
