#----------------------------------------------------------------------------
#       GNU Workspace
#----------------------------------------------------------------------------

# $Id$

include $(ACE_ROOT)/include/makeinclude/macros.GNU
TARGETS_NESTED := $(TARGETS_NESTED:.nested=)

$(TARGETS_NESTED):
	@$(MAKE) -f Makefile.Bug_1393_Client -C . $(@);
