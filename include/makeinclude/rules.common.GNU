#----------------------------------------------------------------------------
#	@(#)rules.common.GNU	1.1	10/18/96
#
#	Common targets
#----------------------------------------------------------------------------

all: 		all.nested all.local
debug:		debug.nested debug.local
profile:	profile.nested profile.local
install:	install.nested install.local
deinstall:	deinstall.nested deinstall.local
clean:		clean.nested clean.local
realclean:	realclean.nested realclean.local
clobber:	clobber.nested clobber.local
depend:		depend.nested depend.local
rcs_info:	rcs_info.nested rcs_info.local

