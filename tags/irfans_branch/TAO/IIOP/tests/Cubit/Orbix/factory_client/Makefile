#----------------------------------------------------------------------------
#	@(#)Makefile	1.1	10/18/96
#
#	Makefile for the ACE_MT_CORBA_Handler tests
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

SRC = cubitC.cpp client.cpp

CLT_OBJS = cubitC.o client.o

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
ORBIX_LIBDIR    = $(ORBIX_ROOT)/corba2/lib
ORBIX_INCDIR    = $(ORBIX_ROOT)/corba2/include

CPPFLAGS += -DEXCEPTIONS -I$(ORBIX_INCDIR) -DWANT_ORBIX_FDS
LDFLAGS  += -L$(ORBIX_LIBDIR) -R $(ORBIX_LIBDIR) -L$(ACE)/ace

IDLFLAGS = -s S.cpp -c C.cpp -B

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

all: client 

client: $(addprefix $(VDIR),$(CLT_OBJS))
	$(LINK.cc) -o client $(addprefix $(VDIR),$(CLT_OBJS)) $(LDFLAGS) $(ITIIOP) $(ITCLT) $(VLDLIBS)


#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/cubitC.o .shobj/cubitC.so: cubitC.cpp cubit.h
.obj/client.o .shobj/client.so: client.cpp $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5-sunc++-4.x.h \
 $(ACE_ROOT)/ace/stdcpp.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i cubit.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
