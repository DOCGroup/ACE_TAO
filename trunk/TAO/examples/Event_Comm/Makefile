#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

LDLIBS = -lorbsvcs -lTAO
PROG_SRCS = Consumer_Handler.cpp \
            Consumer_Input_Handler.cpp \
            Event_Comm_i.cpp \
            Notifier_Handler.cpp \
            Supplier_Input_Handler.cpp \
            consumer.cpp \
            notifier.cpp \
            supplier.cpp \
            Notifier_Server.cpp

IDL_SRC = Event_CommS.cpp Event_CommC.cpp

LSRC = $(IDL_SRC) $(PROG_SRCS)

CONSUMER_OBJS = Event_CommC.o \
                Event_CommS.o \
                Consumer_Handler.o \
                Consumer_Input_Handler.o \
                Event_Comm_i.o \
                consumer.o

SUPPLIER_OBJS = Event_CommC.o \
                Event_CommS.o \
                Supplier_Input_Handler.o \
                Event_Comm_i.o \
                Notifier_Handler.o \
                supplier.o

NOTIFIER_OBJS = Event_CommC.o \
                Event_CommS.o \
                Notifier_Server.o \
                Event_Comm_i.o \
                notifier.o

VLDLIBS = $(LDLIBS:%=%$(VAR))

BIN2 = supplier \
       notifier \
       consumer

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (Naming,$(findstring Naming,$(TAO_ORBSVCS)))
  BIN = $(BIN2)
endif # Naming

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

ifeq ($(ACE_HAS_GNUG_PRE_2_8),1)

  # Since older g++ cannot handle nested classes properly, this sample
  # will not build on g++ prior to 2.8.0.
  all clean realclean:
	@echo Event_Comm is not built with older versions of g++

else # ! ACE_HAS_GNUG_PRE_2_8

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao -L$(TAO_ROOT)/orbsvcs/Naming_Service
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs

.PRECIOUS: Event_CommC.cpp Event_CommS.cpp Event_CommC.h Event_CommS.h

supplier: $(addprefix $(VDIR),$(SUPPLIER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

consumer: $(addprefix $(VDIR),$(CONSUMER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

notifier: $(addprefix $(VDIR),$(NOTIFIER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) Event_CommS.* Event_CommC.* Event_CommS_T.*

endif # ! ACE_HAS_GNUG_PRE_2_8

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
