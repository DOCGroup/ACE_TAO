#----------------------------------------------------------------------------
#	$Id$
#
#	Top-level Makefile for the TAO
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INFO	= README \
	  VERSION

DIRS	= tao \
	  TAO_IDL \
	  orbsvcs \
	  tests


CLONE	= Makefile \
	  tao \
	  TAO_IDL \
	  orbsvcs \
	  tests \

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

RELEASE_FILES = TAO/ChangeLog \
		TAO/INSTALL \
		TAO/TAO-INSTALL.html \
		TAO/COPYING \
		TAO/COPYING.sun \
		TAO/docs \
		TAO/LICENSE.sun \
		TAO/Makefile \
		TAO/README \
		TAO/README.sun \
		TAO/tao \
		TAO/TAO_IDL \
		TAO/orbsvcs \
		TAO/tests \
		TAO/VERSION

INSTALL:	TAO-INSTALL.html
	lynx -dump $^ > $@

#### If creating the "official" TAO release:
#### 1) Check that the workspace is up-to-date, and bail out if not.
#### 2) Update the timestamp in the VERSION file.
#### 3) Add a ChangeLog entry to the ChangeLog plain file.
#### Detect if we are creating the "official" release by looking at the PWD.
#### To disable this feature, add "TIMESTAMP=" to the make command line.
#### NOTE: if the version number in the VERSION file contains three components,
####       e.g., 4.1.5, then the third one will be incremented.  This assumes
####       that alpha/beta releases are numbered with three components, and
####       that final releases are not.  So, if the version number is, e.g.,
####       4.2, it will not be modified because it is assumed to be for a
####       final release.
ifeq ($(shell pwd),/project/adaptive/ACE_wrappers/TAO)
  TIMESTAMP = (CHANGELOG='ChangeLog'; export CHANGELOG; \
              if [ -z "$$CHANGELOG" ]; then echo unable to find latest ChangeLog file; exit 1; fi; \
              cd ..; UPTODATE=`cvs -nq update $(RELEASE_FILES) | egrep -v '/tests/log/' | perl -pi -e 's%/TAO%%g; s/$$/\\\n  /g'`; cd TAO; \
              if [ "$$UPTODATE" ]; then /pkg/gnu/bin/echo -e ERROR: workspace must be updated, and/or non-controlled files must be removed or added/committed: $$UPTODATE; exit 1; fi; \
	      perl release.pl) &&
else
  TIMESTAMP =
endif

#### The following tar creation commands assume that cpio supports -H tar.
#### Old versions of cpio might not, but the version that's shipped with
#### Solaris 2.5.1, and gnu cpio 2.3, do support that option.

FILTER = -name CVS -prune -o ! -name '.\#*' ! -name '\#*' ! -name '*~' -print

cleanrelease:	INSTALL
	@$(TIMESTAMP) (make realclean; cd ..; \
	 find $(RELEASE_FILES) $(FILTER) | cpio -o -H tar | gzip -9 > TAO.tar.gz; \
	 chmod a+r TAO.tar.gz; )

release:	INSTALL
	@$(TIMESTAMP) (cd ..; \
	 find $(RELEASE_FILES) $(FILTER) | cpio -o -H tar | gzip -9 > TAO.tar.gz; \
	 chmod a+r TAO.tar.gz; )
