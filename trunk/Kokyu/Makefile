# $Id$

#----------------------------------------------------------------------------
#       GNU Workspace
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/macros.GNU
TARGETS_NESTED := $(TARGETS_NESTED:.nested=)

$(TARGETS_NESTED):
	@$(MAKE) -f Makefile.Kokyu -C . $(@);
