#----------------------------------------------------------------------------
# $Id$
#
# Makefile for the TPLS library in the C++NPv2, Chapter 6 example.
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

SHLIB   = libTPLS.$(SOEXT)
FILES   = TP_Logging_Server \
          Logging_Acceptor \
          Logging_Event_Handler
LSRC    = $(addsuffix .cpp,$(FILES))

LIBS   += $(ACELIB)

BUILD   = $(VSHLIB)

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

# To remake the DLL_Test source with xlC on AIX, it works
# best to wipe out any previously-created tempinc directory.
# The compiler/linker isn't too smart about instantiating templates...
ifdef TEMPINCDIR
COMPILE.cc := $(RM) -rf tempinc; $(COMPILE.cc)
endif
