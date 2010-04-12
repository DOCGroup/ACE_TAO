# -*- Makefile -*-

#----------------------------------------------------------------------------
#	$Id$
#
#	Common targets
#----------------------------------------------------------------------------

.PHONY: __prebuild__

all:              __prebuild__ all.nested all.local
debug:	          debug.nested debug.local
profile:          profile.nested profile.local
install:          install.nested install.local
deinstall:        deinstall.nested deinstall.local
clean_binaries:   clean_binaries.nested clean_binaries.local
clean_idl_stubs:  clean_idl_stubs.nested clean_idl_stubs.local
clean:            clean.nested clean.local
realclean:        realclean.nested realclean.local
clobber:          clobber.nested clobber.local
depend:	          depend.nested depend.local
rcs_info:         rcs_info.nested rcs_info.local
idl_stubs:        idl_stubs.nested idl_stubs.local
svnignore:	  svnignore.nested svnignore.local
