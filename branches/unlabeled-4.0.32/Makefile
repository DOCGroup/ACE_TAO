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
          apps \
	  examples \
	  netsvcs \
	  performance-tests \
	  tests

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

include $(WRAPPER_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(WRAPPER_ROOT)/include/makeinclude/macros.GNU
include $(WRAPPER_ROOT)/include/makeinclude/rules.common.GNU
include $(WRAPPER_ROOT)/include/makeinclude/rules.nested.GNU
include $(WRAPPER_ROOT)/include/makeinclude/rules.nolocal.GNU

# For the following to work you need to compile the
# $(WRAPPER_ROOT)/bin/clone.cpp file and install it in your ~/bin
# directory (or some place similar).

clone:
	@for dir in $(CLONE) ;\
	do \
		(clone -s $(WRAPPER_ROOT)/$$dir $$dir) \
	done

RELEASE_FILES = ACE_wrappers/ACE-categories \
	        ACE_wrappers/ACE-install.sh \
	        ACE_wrappers/BIBLIOGRAPHY \
	        ACE_wrappers/ChangeLog \
	        ACE_wrappers/ChangeLog-96b \
	        ACE_wrappers/ChangeLog-96a \
	        ACE_wrappers/ChangeLog-95 \
	        ACE_wrappers/ChangeLog-94 \
	        ACE_wrappers/ChangeLog-93 \
		ACE_wrappers/FAQ \
		ACE_wrappers/INSTALL \
		ACE_wrappers/Makefile \
		ACE_wrappers/README \
		ACE_wrappers/VERSION \
		ACE_wrappers/ace \
		ACE_wrappers/apps \
		ACE_wrappers/bin \
		ACE_wrappers/examples \
		ACE_wrappers/include \
		ACE_wrappers/man \
		ACE_wrappers/netsvcs \
		ACE_wrappers/rpc++ \
		ACE_wrappers/performance-tests \
		ACE_wrappers/tests

cleanrelease:
	(make realclean; cd ..; /bin/rm -r ACE.tar.gz; tar cvf ACE.tar $(RELEASE_FILES); gzip -9 ACE.tar; chmod a+r ACE.tar.gz)

release:
	(cd ..; /bin/rm -r ACE.tar.gz; tar cvf ACE.tar $(RELEASE_FILES); gzip -9 ACE.tar; chmod a+r ACE.tar.gz)
