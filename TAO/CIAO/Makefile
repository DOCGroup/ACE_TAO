#----------------------------------------------------------------------------
#       $Id$
#
#       Top-level Makefile for the CIAO
#----------------------------------------------------------------------------

#### The "release" targets can be used to create the ACE+TAO+CIAO kit.  By
#### default, it creates a new beta release.  To create a new minor or
#### major release, add "REL=minor" or "REL=major", respectively, to
#### the make invocation.
####
#### To see what make release would do without actually doing it, add
#### "CHECK=-n" to the invocation.

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

INFO    = VERSION

DIRS    = CIDLC \
	  bin \
	  ciao \
	  docs \
	  examples \
	  tests \
	  tools 

CLONE   = Makefile \
	  CIDLC \
	  bin \
	  ciao \
	  docs \
	  examples \
	  tests \
	  tools 

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU



CONTROLLED_FILES = CIAO

RELEASE_FILES = $(CONTROLLED_FILES) \
                TAO/CIAO/INSTALL

.PHONY: INSTALL
#INSTALL: TAO-INSTALL.html
#	@lynx -dump $^ > $@; chmod a+r $@

#orbsvcs/README: docs/orbsvcs.html
#	@lynx -dump $^ > $@; chmod a+r $@

#.PHONY: release tag

REL = beta
CHECK =
APPLY_NEW_TAG = tag

#### The call to make_release below doesn't actually create the kit.
release: $(APPLY_NEW_TAG)
	@cd $(ACE_ROOT) &&  $(MAKE) -s releaseciao

tag:
	@$(ACE_ROOT)/bin/make_release $(CHECK) -k ciao -v $(REL) -u

.PHONY: show_controlled_files show_release_files show_release_lib_files

show_controlled_files:
	@echo $(CONTROLLED_FILES)

show_release_files:
	@echo $(RELEASE_FILES)

show_release_lib_files:
	@echo ""

show_release_tag_files:
	@echo ""


# For the following to work you need to compile the
# $(ACE_ROOT)/bin/clone.cpp file and install it in your ~/bin
# directory (or some place similar).

.PHONY: clone reverseclean

clone:
	@for dir in $(CLONE) ;\
	do \
		(clone -s $(ACE_ROOT)/$$dir $$dir) \
	done

reverseclean:
	@$(ACE_ROOT)/bin/reverse_clean $(DIRS)

