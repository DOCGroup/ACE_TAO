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

DIRS    = ../IDL_Cubit

LDLIBS = ../IDL_Cubit/.obj/cubitC.o \
         -lTAO

PROG_SRCS = client.cpp

LSRC = $(PROG_SRCS)

CUBIT_CLT_OBJS = client.o

CFLAGS += -I../IDL_Cubit

BIN_UNCHECKED = client

TAO_IDLFLAGS += -Ge 1
#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

BUILD += ../IDL_Cubit/server

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

ifeq ($(minimum_corba),0)
  BIN=$(BIN_UNCHECKED)
endif # minimum_corba

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

LDFLAGS += -L$(TAO_ROOT)/tao

ifeq ($(CXX),cxx)
  ifeq (use_ld_input,$(findstring use_ld_input,$(SOFLAGS)))
    #### Digital Unix 4.0 or later
    CCFLAGS += -ptr ../IDL_Cubit/cxx_repository
  endif # DU 4.0 or later
else
ifeq (-xildoff,$(findstring -xildoff,$(LDFLAGS)))
  #### Sun CC
  CCFLAGS += -ptr. -ptr../IDL_Cubit
endif # Sun CC
endif # cxx

.PRECIOUS: cubitC.cpp cubitS.cpp cubitC.h cubitS.h

client: $(addprefix $(VDIR),$(CUBIT_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

../IDL_Cubit/server:
	cd ../IDL_Cubit; $(MAKE) BIN=server

realclean: clean
	-$(RM) cubitC.* cubitS.* cubitS_T.*

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
