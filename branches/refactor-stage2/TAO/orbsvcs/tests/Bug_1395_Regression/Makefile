#----------------------------------------------------------------------------
#       GNU Workspace
#----------------------------------------------------------------------------

# $Id$

include $(ACE_ROOT)/include/makeinclude/macros.GNU
TARGETS_NESTED := $(TARGETS_NESTED:.nested=)
MFILES = \
         Makefile.Bug_1395_Server \
         Makefile.Bug_1395_Client

$(TARGETS_NESTED):
ifneq (Windows,$(findstring Windows,$(OS)))
	@for file in $(MFILES); do \
	$(MAKE) -f `basename $$file` -C `dirname $$file` $(@); \
	done
else
	-@cmd /c "$(MAKE) -f Makefile.Bug_1395_Server -C . $(@)"
	-@cmd /c "$(MAKE) -f Makefile.Bug_1395_Client -C . $(@)"
endif
