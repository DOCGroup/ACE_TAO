#----------------------------------------------------------------------------
# $Id$
#
# Makefile for the TPCLS library in the C++NPv2, Chapter 7 example.
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

# This example uses SSL.
ssl=1

SHLIB   = libTPCLS.$(SOEXT)
FILES   = TPC_Logging_Server \
          Logging_Handler
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
