#----------------------------------------------------------------------------
#       GNU Workspace
#----------------------------------------------------------------------------

# $Id$

include $(ACE_ROOT)/include/makeinclude/macros.GNU
TARGETS_NESTED := $(TARGETS_NESTED:.nested=)

$(TARGETS_NESTED):
ifneq (Windows,$(findstring Windows,$(OS)))
	@$(MAKE) -f Makefile.Bug_1393_Client -C . $(@);
else
	-@cmd /c "$(MAKE) -f Makefile.Bug_1393_Client -C . $(@)"
endif
