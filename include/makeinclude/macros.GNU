#----------------------------------------------------------------------------
#	@(#)macros.GNU	1.1	10/18/96
#
#	Local and nested target definitions
#----------------------------------------------------------------------------

TARGETS_LOCAL  = \
	all.local \
	debug.local \
	profile.local \
	optimize.local \
	install.local \
	deinstall.local \
	clean.local \
	realclean.local \
	clobber.local \
	depend.local \
	rcs_info.local

TARGETS_NESTED   = \
	$(TARGETS_LOCAL:.local=.nested)

