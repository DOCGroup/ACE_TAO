#----------------------------------------------------------------------------
#       $Id$
#
#       Makefile for the ACE library
#----------------------------------------------------------------------------

# This default rule is here so invoking make realclean will do
# work when recursing through subdirectories
.DEFAULT:
	@$(MAKE) -f Makefile.tao $@
	@$(MAKE) -f Makefile.dirs $@

# Invoke Makefile.ace first to build libACE, and then
# invoke Makefile.dirs to recurse through subdirectories
all:
	@$(MAKE) -f Makefile.tao
	@$(MAKE) -f Makefile.dirs
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
