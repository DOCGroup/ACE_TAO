#----------------------------------------------------------------------------
#       $Id$
#
#       Top-level Makefile for the TAO
#----------------------------------------------------------------------------

#### The "release" targets can be used to create the ACE+TAO kit.  By
#### default, it creates a new beta release.  To create a new minor or
#### major release, add "REL=minor" or "REL=major", respectively, to
#### the make invocation.
####
#### To see what make release would do without actually doing it, add
#### "CHECK=-n" to the invocation.

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

INFO    = README \
          VERSION

DIRS    = tao \
          TAO_IDL \
          tests \
          orbsvcs \
          examples \
          performance-tests \
          utils \
	  docs/tutorials/Quoter

CLONE   = Makefile \
          tao \
          TAO_IDL \
          tests \
          orbsvcs \
          examples \
          performance-tests \
          utils \
	  docs/tutorials/Quoter

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

.PHONY: clone reverseclean
clone:
	@for dir in $(CLONE) ;\
	do \
		(clone -s $(ACE_ROOT)/$$dir $$dir) \
	done

reverseclean:
	@$(ACE_ROOT)/bin/reverse_clean $(DIRS)

CONTROLLED_FILES = TAO/COPYING* \
	    	   TAO/ChangeLog* \
	  	   TAO/LICENSE.sun \
		   TAO/Makefile* \
		   TAO/PROBLEM-REPORT-FORM \
		   TAO/README* \
		   TAO/TAO-INSTALL.html \
		   TAO/TAOACE* \
		   TAO/TAO_IDL \
		   TAO/VERSION \
		   TAO/docs \
		   TAO/examples \
		   TAO/orbsvcs \
		   TAO/performance-tests \
		   TAO/release.pl \
		   TAO/rules.tao.GNU \
		   TAO/tao \
		   TAO/tao-config.1.in \
		   TAO/tao-config.in \
		   TAO/taoConf.sh.in \
	           TAO/tao_targets.GNU \
		   TAO/taoconfig.mk \
		   TAO/tests \
		   TAO/utils


RELEASE_FILES = $(addprefix ACE_wrappers/,$(CONTROLLED_FILES)) \
                ACE_wrappers/TAO/INSTALL \
                ACE_wrappers/TAO/orbsvcs/README

.PHONY: INSTALL
INSTALL: TAO-INSTALL.html
	@lynx -dump $^ > $@; chmod a+r $@

.PHONY: release tag

REL = beta
CHECK =
APPLY_NEW_TAG = tag

#### The call to make_release below doesn't actually create the kit.
#### Then, make releasetao is invoked to actually create the kit.
release: $(APPLY_NEW_TAG)
	@cd ..  &&  $(MAKE) -s releasetao

#### The call to releaseall below doesn't actually create the kit.
#### Then, make releasetao is invoked to actually create the kit.
releaseall: $(APPLY_NEW_TAG)
	@cd CIAO  &&  $(MAKE) -s release REL=$(REL)

tag:
	@$(ACE_ROOT)/bin/make_release $(CHECK) -k tao -v $(REL) -u

.PHONY: show_controlled_files show_release_files show_release_lib_files

show_controlled_files:
	@echo $(CONTROLLED_FILES)

show_release_files:
	@echo $(RELEASE_FILES)

show_release_lib_files:
	@echo ""

show_release_tag_files:
	@echo ""
