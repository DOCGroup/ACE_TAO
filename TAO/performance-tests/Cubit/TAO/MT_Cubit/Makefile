#----------------------------------------------------------------------------
#
#       $Id$
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

LDLIBS = -lTAO_PortableServer -lTAO $(MATHLIB)

IDL_FILES = cubit
IDL_SRC = cubitC.cpp cubitS.cpp
PROG_SRCS = server.cpp \
        cubit_i.cpp \
        Task_Client.cpp \
        client.cpp \
        Util_Thread.cpp \
        tmplinst.cpp \
        Cubit_Task.cpp \
        Globals.cpp \
        Timer.cpp

LSRC = $(IDL_SRC) $(PROG_SRCS)

CUBIT_SVR_OBJS = \
        cubitC.o \
        cubitS.o \
        server.o \
        cubit_i.o \
        Globals.o \
        Cubit_Task.o \
        tmplinst.o \
        Timer.o

CUBIT_CLIENT_OBJS = \
        cubitC.o \
        cubitS.o \
        Task_Client.o \
        client.o \
        Util_Thread.o \
        tmplinst.o \
        Cubit_Task.o \
        cubit_i.o \
        Globals.o \
        Timer.o

BIN = server client

TAO_IDLFLAGS += -Ge 1
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

# Local modifications to variables imported by includes above.
CPPFLAGS += -DDEBUG

LDFLAGS += -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)

# pccTimer will eventually get integrated into ACE.  It is specific for ClassiX on VME
ifdef CHORUS
        PCCTIMER_DIR = /project/doc/mvme
        LSRC += $(PCCTIMER_DIR)/pccTimer.cpp
        CPPFLAGS += -I$(PCCTIMER_DIR)
        CUBIT_CLIENT_OBJS += pccTimer.o
        CUBIT_SVR_OBJS += pccTimer.o
        #### For miniCOOL, I think.  Should it be -lOrb-mt?  LDLIBS += -lOrb
endif

.PRECIOUS: cubitC.h cubitC.i cubitC.cpp cubitS.h cubitS.i cubitS.cpp

server: $(addprefix $(VDIR),$(CUBIT_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client: $(addprefix $(VDIR),$(CUBIT_CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)  $(POSTLINK)

.obj/pccTimer.o .obj/pccTimer.so .shobj/pccTimer.o .shobj/pccTimer.so: /project/doc/mvme/pccTimer.cpp
	@$(COMPILE.cc) -o $@ $<

realclean: clean
	-$(RM) cubitC.* cubitS.* cubitS_T.*

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
