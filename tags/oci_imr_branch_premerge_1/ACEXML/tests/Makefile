# $Id$
#----------------------------------------------------------------------------
#       GNU ACE Workspace
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/macros.GNU
TARGETS_NESTED := $(TARGETS_NESTED:.nested=)
MFILES = \
         Makefile.Transcoder_Test \
         Makefile.NamespaceSupport_Test \
         Makefile.HttpCharStream_Test \
         Makefile.ContentHandler_Test

$(TARGETS_NESTED):
	@for file in $(MFILES); do \
	$(MAKE) -f `basename $$file` -C `dirname $$file` $(@); \
	done
