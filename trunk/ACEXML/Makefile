# $Id$
#----------------------------------------------------------------------------
#       GNU ACE Workspace
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/macros.GNU
TARGETS_NESTED := $(TARGETS_NESTED:.nested=)
MFILES = \
         common/Makefile.ACEXML \
         parser/parser/Makefile.ACEXML_Parser \
         apps/svcconf/Makefile.ACEXML_XML_Svc_Conf_Parser \
         examples/SAXPrint/Makefile.SAXPrint \
         tests/Makefile.Transcoder_Test \
         tests/Makefile.NamespaceSupport_Test \
         tests/Makefile.HttpCharStream_Test \
         tests/Makefile.ContentHandler_Test

$(TARGETS_NESTED):
ifneq (Windows,$(findstring Windows,$(OS)))
	@for file in $(MFILES); do \
	$(MAKE) -f `basename $$file` -C `dirname $$file` $(@); \
	done
else
	-@cmd /c "$(MAKE) -f Makefile.ACEXML -C common $(@)"
	-@cmd /c "$(MAKE) -f Makefile.ACEXML_Parser -C parser/parser $(@)"
	-@cmd /c "$(MAKE) -f Makefile.ACEXML_XML_Svc_Conf_Parser -C apps/svcconf $(@)"
	-@cmd /c "$(MAKE) -f Makefile.SAXPrint -C examples/SAXPrint $(@)"
	-@cmd /c "$(MAKE) -f Makefile.Transcoder_Test -C tests $(@)"
	-@cmd /c "$(MAKE) -f Makefile.NamespaceSupport_Test -C tests $(@)"
	-@cmd /c "$(MAKE) -f Makefile.HttpCharStream_Test -C tests $(@)"
	-@cmd /c "$(MAKE) -f Makefile.ContentHandler_Test -C tests $(@)"
endif
