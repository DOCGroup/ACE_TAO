#----------------------------------------------------------------------------
#
#       $Id$
#
#       Common rules for all of TAO
#
#----------------------------------------------------------------------------

####
#### Required macros for TAO.
####
ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

ifndef TAO_IDL
  TAO_IDL = $(TAO_ROOT)/TAO_IDL/tao_idl
endif


####
#### Macro customization.
####
ifeq ($(findstring g++,$(CXX)),g++)
  ifeq ($(ACE_HAS_GNUG_PRE_2_8),1)
    #### g++ < 2.8 complains about unused parameters in generated code.
    CCFLAGS += -Wno-unused
  endif # g++ < 2.8

  ifneq ($(findstring egcs-2.91,$(shell $(CXX) --version)),)
    #### egcs 2.91.57 seems to go overboard:
    ####   DynAny_i.cpp:565: warning: `class TAO_DynAny_i * this' might be
    ####   used uninitialized in this function
    CCFLAGS += -Wno-uninitialized
  endif # egcs 2.91.x
endif # g++


####
#### Build rules.
####
.SUFFIXES:      S.h C.h S.i C.i S.cpp C.cpp

ifneq ($(use_flick),1)
%S.h %Ser.h %C.h %Cli.h %S.i %Ser.i  %C.i %Cli.i %S.cpp %Ser.cpp %C.cpp %Cli.cpp:       %.idl $(TAO_IDL)
	$(TAO_IDL) $(TAO_IDLFLAGS) $<
else
#### You can redefine the following in your platform_macros.GNU

ifndef SRCDIR
  SRCDIR  =       /project/doc/nanbor/mom
endif
ifndef OBJDIR
  OBJDIR  =       /project/doc/nanbor/dad
endif

CCFLAGS += -I$(OBJDIR)/runtime/headers -I$(SRCDIR)/runtime/headers # -g

CORBA_FE=$(OBJDIR)/bin/flick-fe-newcorba
CORBA_PG=$(OBJDIR)/bin/flick-c-pfe-corbaxx
CORBA_BE=$(OBJDIR)/bin/flick-c-pbe-iiopxx

%C.h %C.i %C.cpp: %.idl
	$(CORBA_FE) $< && \
	$(CORBA_PG) -c -o $*-client.prc $*.aoi && \
	$(CORBA_BE) $*-client.prc -o $*C.cpp -h $*C.h -f $*S.h && \
	echo > $*C.i

%S.h %S.i %S.cpp: %.idl
	$(CORBA_FE) $< && \
	$(CORBA_PG) -s -o $*-server.prc $*.aoi && \
	$(CORBA_BE) $*-server.prc --no_mu_stubs -o $*S.cpp -h $*S.h -F $*C.h && \
	echo > $*S.i && \
        echo > $*S_T.h && \
        echo > $*S_T.i && \
        echo > $*S_T.cpp

endif
