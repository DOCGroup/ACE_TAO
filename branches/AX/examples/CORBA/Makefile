#----------------------------------------------------------------------------
#	$Id: Makefile	1.1	10/18/96
#
#	Makefile for the ACE_MT_CORBA_Handler tests
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

SRC = Test_i.cpp client.cpp server.cpp

SVR_OBJS = TestS.o Test_i.o server.o
CLT_OBJS = TestC.o client.o

LDLIBS	 = 

VLDLIBS	 = $(LDLIBS:%=%$(VAR))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include		$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include		$(ACE_ROOT)/include/makeinclude/macros.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Orbix related macros and target settings.
#----------------------------------------------------------------------------

ORBIX_BINDIR    = $(ORBIX_ROOT)/bin
ORBIX_LIBDIR    = $(ORBIX_ROOT)/lib
ORBIX_INCDIR    = $(ORBIX_ROOT)/include

CPPFLAGS += -DEXCEPTIONS -I$(ORBIX_INCDIR) -DWANT_ORBIX_FDS
LDFLAGS  += -L$(ORBIX_LIBDIR) -R $(ORBIX_LIBDIR) 

IDLFLAGS = -s S.cpp -c C.cpp -B

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

all: client server

client: $(addprefix $(VDIR),$(CLT_OBJS))
	$(LINK.cc) -o client $(addprefix $(VDIR),$(CLT_OBJS)) $(LDFLAGS) -lITsrvmt $(VLDLIBS)

server: $(addprefix $(VDIR),$(SVR_OBJS))
	$(LINK.cc) -o server $(addprefix $(VDIR),$(SVR_OBJS)) $(LDFLAGS) -lITsrvmt $(VLDLIBS)

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
