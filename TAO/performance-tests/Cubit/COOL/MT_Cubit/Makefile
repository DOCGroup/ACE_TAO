#----------------------------------------------------------------------------
# $Id$
#
#	Makefile for the CHORUS COOL Cubit tests
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

IDL_SRC = cubit.cpp cubit.H sk_cubit.C sk_cubit.H

SRC = $(IDL_SRC) \
	cubit_impl.cpp \
	server.cpp \
	client.cpp \
	Task_Client.cpp \
	Util_Thread.cpp

SVR_OBJS = cubit_impl.o \
	server.o \
	cubit.o

CLT_OBJS = Task_Client.o \
	client.o \
	cubit.o \
	Util_Thread.o

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	CHORUS related macros and target settings.
#----------------------------------------------------------------------------

CHORUS_BINDIR = $(CHORUS_ROOT)/bin
CHORUS_LIBDIR = $(CHORUS_ROOT)/lib
CHORUS_INCDIR = $(CHORUS_ROOT)/include

CPPFLAGS += -I$(CHORUS_INCDIR) -I$(CHORUS_ROOT)
LDFLAGS  += -L$(CHORUS_LIBDIR) -R$(CHORUS_LIBDIR) -L$(ACE)/ace
IDLFLAGS = --stub-header --stub-impl --skeleton-header --skeleton-impl --no-local --no-any --binding Cubit Cubit_Impl

# pccTimer will eventually get integrated into ACE.  It is specific for
# ClassiX on VME
ifdef CHORUS
  SRC += pccTimer.cpp
  CLT_OBJS += pccTimer.o
  LDLIBS += -lOrb
else
  LDLIBS += -lOrb-mt
endif

VLDLIBS := $(LDLIBS:%=%$(VAR)) $(ACELIB) $(LIBS)

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

all: $(IDL_SRC) server client

$(IDL_SRC): cubit.idl
	$(CHORUS_BINDIR)/chic $(IDLFLAGS) cubit.idl
	-mv cubit.C cubit.cpp

server: $(addprefix $(VDIR),$(SVR_OBJS))
	$(LINK.cc) -o server $(addprefix $(VDIR),$(SVR_OBJS)) $(LDFLAGS) $(ITIIOP) $(ITSRV) $(VLDLIBS) $(POSTLINK)

client: $(addprefix $(VDIR),$(CLT_OBJS))
	$(LINK.cc) -o client $(addprefix $(VDIR),$(CLT_OBJS)) $(LDFLAGS) $(ITIIOP) $(ITCLT) $(VLDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) cubit.cpp cubit.H sk_cubit.C sk_cubit.H server client

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
