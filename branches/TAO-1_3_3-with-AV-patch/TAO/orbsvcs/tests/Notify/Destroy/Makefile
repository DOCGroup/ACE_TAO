# $Id$
#----------------------------------------------------------------------------
#       GNU Makefile
#----------------------------------------------------------------------------
MAKEFILE        = Makefile
DEPENDENCY_FILE = .depend.Makefile
BIN_UNCHECKED   = Notify_Tests_Destroy

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif



FILES = \
  main

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------
include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

CURRENT_COMPONENTS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (Notify, $(findstring Notify, $(CURRENT_COMPONENTS)))
ifeq (Naming, $(findstring Naming, $(CURRENT_COMPONENTS)))
BIN    = $(BIN_UNCHECKED)
endif
endif

OBJS   = $(addsuffix .o, $(notdir $(FILES)))
SRC    = $(addsuffix .cpp, $(FILES))
ACELIB = -lACE

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk
include $(TAO_ROOT)/orbsvcs/tests/Notify/notify_tests.GNU

CPPFLAGS     += -I$(TAO_ROOT)/orbsvcs/tests/Notify/lib -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT) -I$(TAO_ROOT)/tao -I$(ACE_ROOT)
ifeq ($(static_libs),1)
  ifneq ($(LIB),)
    CPPFLAGS += -DTAO_AS_STATIC_LIBS -DACE_AS_STATIC_LIBS
  endif
endif


LDFLAGS      += -L$(TAO_ROOT)/orbsvcs/tests/Notify/lib -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao -L$(ACE_ROOT)/ace

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

$(BIN): $(addprefix $(VDIR), $(OBJS))
	$(LINK.cc) $(LDFLAGS) $(CC_OUTPUT_FLAG) $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
