#----------------------------------------------------------------------------
#
#       $Id$
#
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

TAO_IDLFLAGS +=  -Ge 1 -I$(TAO_ROOT)/orbsvcs/orbsvcs -I$(TAO_ROOT)
IDL_FILES = NavWeap
IDL_SRC = NavWeapC.cpp NavWeapS.cpp
IDL_HDR = NavWeapC.h
EVENT_TEST_SRCS = Event_Sup.cpp Logging_Sup.cpp DualEC_Sup.cpp Event_Con.cpp DOVE_Supplier.cpp

LSRC = $(IDL_SRC) $(EVENT_TEST_SRCS)

EVENT_SUP_OBJS = NavWeapC.o NavWeapS.o Event_Sup.o DOVE_Supplier.o
EVENT_CON_OBJS = NavWeapC.o NavWeapS.o Event_Con.o
LOGGING_SUP_OBJS = NavWeapC.o NavWeapS.o Logging_Sup.o DOVE_Supplier.o
DUALEC_SUP_OBJS = NavWeapC.o NavWeapS.o DualEC_Sup.o DOVE_Supplier.o

LDLIBS = -lTAO_RTOLDEvent -lTAO_RTEvent -lTAO_RTSched -lTAO_CosNaming -lTAO_Svc_Utils -lTAO_IORTable -lTAO_PortableServer -lTAO

BIN2 = Event_Sup Logging_Sup DualEC_Sup Event_Con

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (RTOLDEvent,$(findstring RTOLDEvent,$(TAO_ORBSVCS)))
  BIN = $(BIN2)
endif # RTOLDEvent

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT) \
            -I$(TAO_ROOT)/orbsvcs/orbsvcs

NavWeapC.h: ../NavWeap.idl $(TAO_IDL)
	$(TAO_IDL) $(TAO_IDLFLAGS) $< -o .

Event_Sup: $(addprefix $(VDIR),$(EVENT_SUP_OBJS))

Logging_Sup: $(addprefix $(VDIR),$(LOGGING_SUP_OBJS))

DualEC_Sup: $(addprefix $(VDIR),$(DUALEC_SUP_OBJS))

Event_Con: $(addprefix $(VDIR),$(EVENT_CON_OBJS))

realclean: clean
	-$(RM) NavWeap.idl NavWeap[CS].* NavWeap[CS]_T.*

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
