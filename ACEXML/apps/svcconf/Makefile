# $Id$
#----------------------------------------------------------------------------
#       GNU ACE Workspace
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/macros.GNU
TARGETS_NESTED := $(TARGETS_NESTED:.nested=)

$(TARGETS_NESTED):
ifneq (Windows,$(findstring Windows,$(OS)))
	@$(MAKE) -f Makefile.ACEXML_XML_Svc_Conf_Parser -C . $(@);
else
	-@cmd /c "$(MAKE) -f Makefile.ACEXML_XML_Svc_Conf_Parser -C . $(@)"
endif
