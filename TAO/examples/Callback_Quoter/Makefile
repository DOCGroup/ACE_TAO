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

LDLIBS = -lTAO -lorbsvcs

IDL_SRC = NotifierC.cpp NotifierS.cpp ConsumerC.cpp ConsumerS.cpp

PROG_SRCS = \
        consumer.cpp \
        supplier.cpp \
        Supplier_i.cpp \
        Supplier_Timer_Handler.cpp\
        notifier.cpp \
        Notifier_Input_Handler.cpp \
        Notifier_i.cpp \
        Consumer_i.cpp \
        Consumer_Input_Handler.cpp \
        Consumer_Signal_Handler.cpp \
        Consumer_Handler.cpp


SRC = $(IDL_SRC) $(PROG_SRCS)

CONSUMER_CLT_OBJS = \
                NotifierC.o \
                NotifierS.o \
                Consumer_Input_Handler.o \
                Consumer_Signal_Handler.o \
                Consumer_Handler.o \
                consumer.o\
                ConsumerC.o\
                ConsumerS.o \
                Consumer_i.o
MARKET_CLT_OBJS = \
                NotifierC.o \
                NotifierS.o \
                Supplier_i.o \
                Supplier_Timer_Handler.o \
                supplier.o \
                ConsumerC.o\
                ConsumerS.o \
                Consumer_i.o

SIMPLE_SVR_OBJS = \
                NotifierC.o \
                NotifierS.o \
                Notifier_i.o \
                Notifier_Input_Handler.o \
                ConsumerC.o\
                ConsumerS.o\
                notifier.o

VLDLIBS = $(LDLIBS:%=%$(VAR))

BIN2 = notifier \
       consumer\
       supplier

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
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
#include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

LDFLAGS +=  -L$(TAO_ROOT)/tao -L$(ACE_ROOT)/ace
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs

.PRECIOUS: NotifierC.cpp NotifierC.i NotifierC.h
.PRECIOUS: NotifierS.cpp NotifierS.i NotifierS.h
.PRECIOUS: NotifierS_T.cpp NotifierS_T.i NotifierS_T.h
.PRECIOUS: ConsumerC.cpp ConsumerC.i ConsumerC.h
.PRECIOUS: ConsumerS.cpp ConsumerS.i ConsumerS.h
.PRECIOUS: ConsumerS_T.cpp ConsumerS_T.i ConsumerS_T.h

notifier:$(addprefix $(VDIR),$(SIMPLE_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

supplier:$(addprefix $(VDIR),$(MARKET_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

consumer:$(addprefix $(VDIR),$(CONSUMER_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)
realclean: clean
	-$(RM) NotifierC.* NotifierS.* NotifierS_T.* \
               ConsumerC.* ConsumerS.* ConsumerS_T.*

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
