#----------------------------------------------------------------------------
#       $Id$
#
#       Top-level Makefile for the ACE toolkit libraries, tests, and
#       applications
#----------------------------------------------------------------------------

#### Please see ACE-INSTALL.html for build information.

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

INFO    = README \
          THANKS \
          VERSION

DIRS    = ace \
	  ACEXML \
          netsvcs \
          tests \
          apps \
          examples \
          performance-tests \
          websvcs

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
          tests \
          websvcs \
	  ACEXML

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU

# Optional TAO targets.
-include $(ACE_ROOT)/TAO/tao_targets.GNU

# For the following to work you need to compile the
# $(ACE_ROOT)/bin/clone.cpp file and install it in your ~/bin
# directory (or some place similar).

.PHONY: clone
clone:
	@for dir in $(CLONE); \
        do \
                (clone -s $(ACE_ROOT)/$$dir $$dir) \
        done

CORE_DIRS= \
	ace \
	apps/gperf/src \
	TAO/tao \
	TAO/TAO_IDL \
	TAO/orbsvcs/orbsvcs \
	TAO/orbsvcs/Naming_Service

.PHONY: Core reverseclean
Core:
	@for dir in $(CORE_DIRS); \
	do \
		$(MAKE) -C $$dir; \
	done

reverseclean:
	@$(ACE_ROOT)/bin/reverse_clean $(DIRS)

#### NOTE:  The following comments describe how to create kits.
####        It's intended for use by ACE+TAO developers and
####        maintainers only.  ACE+TAO users need not be concerned
####        with creating kits, just unpacking and using them.

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
#### There are some other release/releaseall options [default value is in
#### brackets]:
####   ACE_TAG [ACE version in your workspace]: prepend tag with -ta
####   TAO_TAG [TAO version in your workspace]: prepend tag with -tt
####
####   NOTE: the ACE and TAO versions will be updated automatically
####   by release/releaseall.  ACE_TAG and TAO_TAG can be overridden
####   to kit a particular version.
####
####   APPLY_NEW_TAG [enabled]: set to null to disable
####   CHECK [disabled]: set to -n to see what make_release will do, but not
####     do it
####   INSTALL_KIT [enabled]: set to null to not install in public
####     ftp/http directory on host ace
####   REL [beta]: set to minor or major, optionally, when applying a new tag
####   ZIP_FILES [enabled]: set to -z to disable creation of .zip files
####
#### Example creation of ACE-only kit, version ACE-5_0_1 from current
#### workspace:
#### make release ACE_TAG='-ta ACE-5_0_1' APPLY_NEW_TAG= \
####   INSTALL_KIT= ZIP_FILES=-z

CONTROLLED_FILES = \
        ACE-INSTALL.html \
        ACE-install.sh \
        ASNMP \
        AUTHORS \
        COPYING \
        ChangeLog* \
        FAQ \
        Makefile \
        Makefile.am \
        PROBLEM-REPORT-FORM \
        README \
        THANKS \
        TODO \
        VERSION \
	ACEXML \
        acconfig.h \
        ace \
        ace-config.1.in \
        ace-config.in \
        aceConf.sh.in \
        apps \
        bin \
        docs \
        etc \
        examples \
        include \
        m4 \
        netsvcs \
        os-patches \
        performance-tests \
        tests \
        websvcs

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
        ACE_wrappers/m4 \
        ACE_wrappers/netsvcs \
        ACE_wrappers/tests \
        ACE_wrappers/websvcs

# Files that should get tagged at release time but shouldn't be in any
# of the release file lists since they may already be in a directory
# found in those lists.
RELEASE_TAG_FILES = \
        html \
        man/Makefile.am \
        man/man3/Makefile.am

.PHONY: release releasetao releaseall tag

ACE_TAG_VALUE = $(shell head -1 VERSION | perl -ne \
                's/.* ([\d\.]+),.*\n/$$1/; tr/./_/; print "ACE-$$_";')
ACE_TAG       = -ta $(ACE_TAG_VALUE)
TAO_TAG_VALUE = $(shell head -1 TAO/VERSION | perl -ne \
                's/.* ([\d\.]+),.*\n/$$1/; tr/./_/; print "TAO-$$_";')
TAO_TAG       = -tt $(TAO_TAG_VALUE)
CIAO_TAG_VALUE = $(shell head -1 TAO/CIAO/VERSION | perl -ne \
                's/.* ([\d\.]+),.*\n/$$1/; tr/./_/; print "CIAO-$$_";')
CIAO_TAG       = -tc $(CIAO_TAG_VALUE)
APPLY_NEW_TAG      = tag
CHECK              =
GENERATE_MAN_PAGES = -g
INSTALL_KIT        = -i
REL                = beta
ZIP_FILES          =

#### The release target creates the ACE (only) kit.
release: $(APPLY_NEW_TAG)
	@$(ACE_ROOT)/bin/make_release -k ace $(ACE_TAG) \
         $(INSTALL_KIT) $(ZIP_FILES) $(CHECK)

tag:
	@$(ACE_ROOT)/bin/make_release $(CHECK) -k ace -v $(REL) -u

#### The following target is for use by the TAO Makefile.  It should not
#### be called directly from the command line.  The releasetao target
#### creates the combined ACE-TAO kit.
releasetao:
	@$(ACE_ROOT)/bin/make_release -k ace+tao $(ACE_TAG) $(TAO_TAG) \
         $(INSTALL_KIT) $(ZIP_FILES) $(CHECK)

#### The following target is for use by the CIAO Makefile.  It should not
#### be called directly from the command line.  The releaseciao target
#### creates the combined ACE+TAO+CIAO kit.
releaseciao:
	@$(ACE_ROOT)/bin/make_release -k ace+tao+ciao $(ACE_TAG) $(TAO_TAG) $(CIAO_TAG) \
         $(INSTALL_KIT) $(ZIP_FILES) $(CHECK)

#### The releaseall target:
releaseall: $(APPLY_NEW_TAG)
	@cd TAO  &&  $(MAKE) -s releaseall REL=$(REL)

### Generate man pages
genmanpages:
	@$(ACE_ROOT)/bin/generate_rel_manpages

.PHONY: show_controlled_files show_release_files show_release_lib_files
.PHONY: show_release_tag_files

show_controlled_files:
	@echo $(CONTROLLED_FILES)

show_release_files:
	@echo $(RELEASE_FILES)

show_release_lib_files:
	@echo $(RELEASE_LIB_FILES)

show_release_tag_files:
	@echo $(RELEASE_TAG_FILES)

ACE-INSTALL: ACE-INSTALL.html
	@lynx -dump $< > $@
