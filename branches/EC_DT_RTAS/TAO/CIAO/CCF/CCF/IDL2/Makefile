#----------------------------------------------------------------------------
#       $Id$
#
#       Makefile for the ACE library
#----------------------------------------------------------------------------

.DEFAULT:
	@$(MAKE) -C SemanticGraph $@
	@$(MAKE) -C SemanticAction $@
	@$(MAKE) -C Traversal $@
	@$(MAKE) -f Makefile.archive $@


.PHONY: all

all:
	@$(MAKE) -C SemanticGraph objects
	@$(MAKE) -C SemanticAction objects
	@$(MAKE) -C Traversal objects
	@$(MAKE) -f Makefile.archive
