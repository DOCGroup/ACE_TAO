#----------------------------------------------------------------------------
#       $Id$
#
#       Makefile for the TAO library
#----------------------------------------------------------------------------

# This default rule is here so invoking make realclean will
# work when recursing through subdirectories
.DEFAULT:
	@$(MAKE) -f Makefile.tao $@
	@$(MAKE) -f Makefile.dirs SUBDIR_MAKEFILE=Makefile $@

# We need this rule, so that if parallel makes (-j) are invoked,
# the tao target will be built before the dirs target (otherwise the
# dirs targets may try to link libTAO before libTAO has been built).
# Recursively invoked makes from this file will still be run in parallel.
.NOTPARALLEL:

# Invoke Makefile.tao first to build libTAO, and then
# invoke Makefile.dirs to recurse through subdirectories
all: tao dirs

tao:
	@$(MAKE) -f Makefile.tao

dirs:
	@$(MAKE) -f Makefile.dirs SUBDIR_MAKEFILE=Makefile

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
