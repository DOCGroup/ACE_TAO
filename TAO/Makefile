#----------------------------------------------------------------------------
#	$Id$
#
#	Top-level Makefile for the TAO
#----------------------------------------------------------------------------

#### The "release" targets can be used to create the ACE+TAO kit.  By
#### default, it creates a new beta release.  To create a new minor or
#### major release, add "REL=minor" or "REL=major", respectively, to
#### the make invocation.
####
#### To see what make release would do without actually doing it, add
#### "CHECK=-n" to the invocation.

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INFO	= README \
	  VERSION

DIRS	= tao \
	  TAO_IDL \
	  orbsvcs \
	  examples \
	  performance-tests \
	  tests \
	  utils

CLONE	= Makefile \
	  tao \
	  TAO_IDL \
	  orbsvcs \
	  examples \
	  performance-tests \
	  tests \
	  utils

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

.PHONY: clone
clone:
	@for dir in $(CLONE) ;\
	do \
		(clone -s $(ACE_ROOT)/$$dir $$dir) \
	done

CONTROLLED_FILES = ChangeLog \
		   TAO-INSTALL.html \
		   TAO-inheritance.pdf.gz \
		   TAO-inheritance.ps.gz \
                   TAOACE.dsw \
		   COPYING \
		   COPYING.sun \
		   release.chk \
		   rules.tao.GNU \
		   taoconfig.mk \
		   docs \
		   LICENSE.sun \
		   Makefile \
                   PROBLEM-REPORT-FORM \
		   README \
		   README.sun \
		   tao \
		   TAO_IDL \
		   orbsvcs \
		   examples \
		   performance-tests \
		   tests \
		   utils \
		   VERSION

RELEASE_FILES = $(addprefix TAO/,$(CONTROLLED_FILES)) \
                TAO/INSTALL \
		TAO/orbsvcs/README

.PHONY: INSTALL
INSTALL: TAO-INSTALL.html
	@lynx -dump $^ > $@; chmod a+r $@

orbsvcs/README: docs/orbsvcs.html
	@lynx -dump $^ > $@; chmod a+r $@

.PHONY: release

REL = beta
CHECK =

#### The call to make_release below doesn't actually create the kit.
#### If creating a release in /project/adaptive/ACE_wrappers/TAO, it
#### just updates the VERSION and ChangeLog files, and tags the release.
#### Then, make releasetao is invoked to actually create the kit.
release: INSTALL
	@$(ACE_ROOT)/bin/make_release $(CHECK) -k tao -t $(REL) \
	   -c "$(CONTROLLED_FILES)" -r "$(RELEASE_FILES)"  &&  \
	 cd ..  &&  $(MAKE) releasetao
