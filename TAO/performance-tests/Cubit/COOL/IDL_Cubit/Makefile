#----------------------------------------------------------------------------
# $Id$
#
#	Makefile for the CHORUS COOL Cubit tests
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

IDL_SRC = cubit.cpp cubit.H sk_cubit.C sk_cubit.H

SRC = $(IDL_SRC) cubit_i.cpp server.cpp client.cpp tmplinst.cpp

SVR_OBJS = cubit_i.o server.o cubit.o tmplinst.o

CLT_OBJS = client.o cubit.o tmplinst.o

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
IDLFLAGS = --stub-header --stub-impl --skeleton-header --skeleton-impl --no-local --binding Cubit Cubit_i --binding Cubit_Factory Cubit_Factory_i

ifdef CHORUS
  LDLIBS += -lOrb
else
  LDLIBS += -lOrb-mt $(MATHLIB)
endif

VLDLIBS := $(LDLIBS:%=%$(VAR)) $(ACELIB) $(LIBS)

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

all: $(IDL_SRC) server client

$(IDL_SRC): cubit.idl
	$(CHORUS_BINDIR)/chic $(IDLFLAGS) cubit.idl
	-/bin/mv cubit.C cubit.cpp

server: $(addprefix $(VDIR),$(SVR_OBJS))
	$(LINK.cc) -o server $(addprefix $(VDIR),$(SVR_OBJS)) $(LDFLAGS) $(ITIIOP) $(ITSRV) $(VLDLIBS) $(POSTLINK)

client: $(addprefix $(VDIR),$(CLT_OBJS))
	$(LINK.cc) -o client $(addprefix $(VDIR),$(CLT_OBJS)) $(LDFLAGS) $(ITIIOP) $(ITCLT) $(VLDLIBS) $(POSTLINK)

realclean: clean
	-/bin/rm -rf cubit.cpp cubit.H sk_cubit.C sk_cubit.H server client

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/cubit.o .obj/cubit.so .shobj/cubit.o .shobj/cubit.so: cubit.cpp cubit.H
.obj/sk_cubit.o .obj/sk_cubit.so .shobj/sk_cubit.o .shobj/sk_cubit.so: sk_cubit.C
.obj/cubit_i.o .obj/cubit_i.so .shobj/cubit_i.o .shobj/cubit_i.so: cubit_i.cpp cubit_i.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i cubit.H sk_cubit.H
.obj/server.o .obj/server.so .shobj/server.o .shobj/server.so: server.cpp \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Get_Opt.i cubit_i.h cubit.H \
 sk_cubit.H sk_cubit.C
.obj/client.o .obj/client.so .shobj/client.o .shobj/client.so: client.cpp \
 $(ACE_ROOT)/ace/Profile_Timer.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/High_Res_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.i \
 $(ACE_ROOT)/ace/Profile_Timer.i \
 $(ACE_ROOT)/ace/Env_Value_T.h \
 $(ACE_ROOT)/ace/Env_Value_T.i client.h \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i cubit.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/cool.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/api/api.H \
 sk_cubit.H
.obj/tmplinst.o .obj/tmplinst.so .shobj/tmplinst.o .shobj/tmplinst.so: tmplinst.cpp cubit.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/cool.H

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
