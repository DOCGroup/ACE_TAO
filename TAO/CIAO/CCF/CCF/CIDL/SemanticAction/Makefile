#----------------------------------------------------------------------------
#       $Id$
#
#       Makefile for the ACE library
#----------------------------------------------------------------------------

.DEFAULT:
	@$(MAKE) -f Makefile.object $@
	@$(MAKE) -C Impl $@

.PHONY: all

all:
	@$(MAKE) -f Makefile.object objects
	@$(MAKE) -C Impl objects
