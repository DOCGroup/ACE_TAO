# $Id$
#----------------------------------------------------------------------------
#       GNU ACE Workspace
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/macros.GNU
TARGETS_NESTED := $(TARGETS_NESTED:.nested=)
MFILES = \
         examples/SAXPrint/Makefile.SAXPrint \
         examples/svcconf/Makefile.Svcconf \
         common/Makefile.XML_Common \
         parser/parser/Makefile.Parser \
         parser/debug_validator/Makefile.Validator \
         tests/Makefile.HttpCharStream_Test \
         tests/Makefile.Transcoder_Test \
         tests/Makefile.NamespaceSupport_Test \
         compass/Makefile.Compass \
         compass/Makefile.Deployment \
         apps/svcconf/Makefile.XML_Svc_Conf_Parser

$(TARGETS_NESTED):
ifneq (Windows,$(findstring Windows,$(OS)))
	@for file in $(MFILES); do \
	$(MAKE) -f `basename $$file` -C `dirname $$file` $(@); \
	done
else
	-@cmd /c "$(MAKE) -f Makefile.SAXPrint -C examples/SAXPrint $(@)"
	-@cmd /c "$(MAKE) -f Makefile.Svcconf -C examples/svcconf $(@)"
	-@cmd /c "$(MAKE) -f Makefile.XML_Common -C common $(@)"
	-@cmd /c "$(MAKE) -f Makefile.Parser -C parser/parser $(@)"
	-@cmd /c "$(MAKE) -f Makefile.Validator -C parser/debug_validator $(@)"
	-@cmd /c "$(MAKE) -f Makefile.HttpCharStream_Test -C tests $(@)"
	-@cmd /c "$(MAKE) -f Makefile.Transcoder_Test -C tests $(@)"
	-@cmd /c "$(MAKE) -f Makefile.NamespaceSupport_Test -C tests $(@)"
	-@cmd /c "$(MAKE) -f Makefile.Compass -C compass $(@)"
	-@cmd /c "$(MAKE) -f Makefile.Deployment -C compass $(@)"
	-@cmd /c "$(MAKE) -f Makefile.XML_Svc_Conf_Parser -C apps/svcconf $(@)"
endif
