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
	        ACE_wrappers/ChangeLog-97a \
	        ACE_wrappers/ChangeLog-96b \
	        ACE_wrappers/ChangeLog-96a \
	        ACE_wrappers/ChangeLog-95 \
	        ACE_wrappers/ChangeLog-94 \
	        ACE_wrappers/ChangeLog-93 \
		ACE_wrappers/FAQ \
		ACE_wrappers/INSTALL \
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
		ACE_wrappers/rpc++ \
		ACE_wrappers/performance-tests \
		ACE_wrappers/tests

ifeq ($(PWD),)
  PWD := $(shell pwd)
endif

#### If creating the "official" ACE release, update the timestamp in VERSION.
#### Detect if we are doing that by looking at the PWD.
#### To disable this feature, add "TIMESTAMP=" to the make command line.
ifeq ($(PWD),/home/cs/faculty/schmidt/adaptive/ACE_wrappers)
  TIMESTAMP = perl -pi -e 'BEGIN {chop ($$date=`/usr/bin/date`);} \
                           s/(, released ).*/$$1$$date./' VERSION; \
	      chmod 644 VERSION; \
              cvs commit -m'make release: updated timestamp' VERSION; 
else
  TIMESTAMP =
endif

cleanrelease:
	($(TIMESTAMP)make realclean; cd ..; /bin/rm -f ACE.tar.gz; tar cvf ACE.tar $(RELEASE_FILES); gzip -9 ACE.tar; chmod a+r ACE.tar.gz)

release:
	($(TIMESTAMP)cd ..; /bin/rm -f ACE.tar.gz; tar cvf ACE.tar $(RELEASE_FILES); gzip -9 ACE.tar; chmod a+r ACE.tar.gz)
