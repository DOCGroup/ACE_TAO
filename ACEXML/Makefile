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
	@for file in $(MFILES); do \
	$(MAKE) -f `basename $$file` -C `dirname $$file` $(@); \
	done
