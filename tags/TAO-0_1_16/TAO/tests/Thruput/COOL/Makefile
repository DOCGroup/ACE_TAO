#----------------------------------------------------------------------------
#	@(#)Makefile	1.1	10/18/96
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

include		$(WRAPPER_ROOT)/include/makeinclude/wrapper_macros.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/macros.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.common.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.nonested.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.lib.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.bin.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.local.GNU

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
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/ACE.h \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/OS.h \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/inc_user_config.h \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/config.h \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/streams.h \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/Basic_Types.h \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/Basic_Types.i \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/OS.i \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/Trace.h \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/Log_Msg.h \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/Log_Record.h \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/Log_Priority.h \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/Log_Record.i \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/Version.h \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/ACE.i ttcp_i.h ttcp.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/cool.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/shortSeq.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/longSeq.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/doubleSeq.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/charSeq.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/octetSeq.H \
 ttcp_decl.h \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/Profile_Timer.h \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/Time_Value.h \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/High_Res_Timer.h \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/High_Res_Timer.i \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/Profile_Timer.i
server.o: server.cpp \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/ACE.h \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/OS.h \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/inc_user_config.h \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/config.h \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/streams.h \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/Basic_Types.h \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/Basic_Types.i \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/OS.i \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/Trace.h \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/Log_Msg.h \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/Log_Record.h \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/Log_Priority.h \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/Log_Record.i \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/Version.h \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/ACE.i ttcp_i.h ttcp.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/cool.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/shortSeq.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/longSeq.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/doubleSeq.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/charSeq.H \
 /project/doc/miniCOOL/minicool.r4.3/solaris2.5-CC-mt/include/corba/octetSeq.H \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/Get_Opt.h \
 /project/doc/sergio/ACE_wrappers/build/sunt/ace/Get_Opt.i sk_ttcp.H \
 sk_ttcp.C

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
