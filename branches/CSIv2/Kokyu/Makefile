# $Id$

#----------------------------------------------------------------------------
#       GNU Workspace
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/macros.GNU
TARGETS_NESTED := $(TARGETS_NESTED:.nested=)

$(TARGETS_NESTED):
ifneq (Windows,$(findstring Windows,$(OS)))
	@$(MAKE) -f Makefile.Kokyu -C . $(@);
else
	-@cmd /c "$(MAKE) -f Makefile.Kokyu -C . $(@)"
endif
