#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for the CHORUS COOL Cubit tests
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

IDL_SRC = ttcp.cpp ttcp.H sk_ttcp.C sk_ttcp.H

SRC = $(IDL_SRC) ttcp_i.cpp server.cpp client.cpp utils.cpp

SVR_OBJS = ttcp_i.o server.o ttcp.o utils.o

CLT_OBJS = client.o ttcp.o utils.o

LDLIBS	 = -lm -lOrb-mt -lposix4

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
#	CHORUS related macros and target settings.
#----------------------------------------------------------------------------

CHORUS_BINDIR    = $(CHORUS_ROOT)/bin
CHORUS_LIBDIR    = $(CHORUS_ROOT)/lib
CHORUS_INCDIR    = $(CHORUS_ROOT)/include

CPPFLAGS += -I$(CHORUS_INCDIR) -I$(CHORUS_ROOT)
LDFLAGS  += -L$(CHORUS_LIBDIR) -R $(CHORUS_LIBDIR) -L$(ACE)/ace
IDLFLAGS = --stub-header --stub-impl --skeleton-header --skeleton-impl --no-local --no-any --binding ttcp_sequence ttcp_sequence_i

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

all:	$(IDL_SRC) server client

$(IDL_SRC): ttcp.idl
	$(CHORUS_BINDIR)/chic $(IDLFLAGS) ttcp.idl
	-/bin/mv ttcp.C ttcp.cpp

server: $(addprefix $(VDIR),$(SVR_OBJS))
	$(LINK.cc) -o server $(addprefix $(VDIR),$(SVR_OBJS)) $(LDFLAGS) $(ITIIOP) $(ITSRV) $(VLDLIBS) $(POSTLINK)

client: $(addprefix $(VDIR),$(CLT_OBJS))
	$(LINK.cc) -o client $(addprefix $(VDIR),$(CLT_OBJS)) $(LDFLAGS) $(ITIIOP) $(ITCLT) $(VLDLIBS) $(POSTLINK)

realclean: clean
	-/bin/rm -rf ttcp.cpp ttcp.H sk_ttcp.C sk_ttcp.H server client

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

ttcp.o: ttcp.cpp ttcp.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/shortSeq.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/longSeq.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/doubleSeq.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/charSeq.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/octetSeq.H
sk_ttcp.o: sk_ttcp.C
ttcp_i.o: ttcp_i.cpp \
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
 $(ACE_ROOT)/ace/ACE.i ttcp_i.h ttcp.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/cool.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/shortSeq.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/longSeq.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/doubleSeq.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/charSeq.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/octetSeq.H \
 ttcp_decl.h \
 $(ACE_ROOT)/ace/Profile_Timer.h \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/High_Res_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.i \
 $(ACE_ROOT)/ace/Profile_Timer.i
server.o: server.cpp \
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
 $(ACE_ROOT)/ace/ACE.i ttcp_i.h ttcp.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/cool.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/shortSeq.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/longSeq.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/doubleSeq.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/charSeq.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/octetSeq.H \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i sk_ttcp.H \
 sk_ttcp.C

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
