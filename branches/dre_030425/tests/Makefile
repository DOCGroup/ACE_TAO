#----------------------------------------------------------------------------
#
# $Id$
#
#       Makefile for the ACE "one-button" tests directory
#----------------------------------------------------------------------------

# This default rule is here so invoking make realclean will do
# work when recursing through subdirectories
.DEFAULT:
	@$(MAKE) -f Makefile.libs $@
	@$(MAKE) -f Makefile.tests $@
	@$(MAKE) -f Makefile.dirs SUBDIR_MAKEFILE=Makefile $@

# Invoke Makefile.libs first to build the libTest_Output and the other
# libraries, then invoke Makefile.tests and Makefile.dirs to recurse
# through subdirectories
all:
	@$(MAKE) -f Makefile.libs $@
	@$(MAKE) -f Makefile.tests $@
	@$(MAKE) -f Makefile.dirs SUBDIR_MAKEFILE=Makefile $@
