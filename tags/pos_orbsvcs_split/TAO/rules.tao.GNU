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

ifndef COMSPEC
  ifdef ComSpec
    #### ACE+TAO use COMSPEC, but ComSpec is defined.
    COMSPEC = $(ComSpec)
  endif # ComSpec
endif # ! COMPSPEC

ifdef COMSPEC
  #### Assume we're on a WIN32 host.
  ifndef TAO_IDL_PREPROCESSOR
    #### Assume we have Bourne shell and sed.  Otherwise, the user
    #### should find out here that the TAO_IDL_PREPROCESSOR environment
    #### variable must be set explicitly!
    TAO_IDL_PREPROCESSOR := $(shell type $(CXX) | sed 's/.* is //')
  endif # ! TAO_IDL_PREPROCESSOR
endif # COMSPEC


####
#### Macro customization.
####
ifeq ($(findstring g++,$(CXX)),g++)
  ifeq ($(ACE_HAS_GNUG_PRE_2_8),1)
    #### g++ < 2.8 complains about unused parameters in generated code.
    CCFLAGS += -Wno-unused
  endif # g++ < 2.8
endif # g++

ifeq ($(minimum_corba),1)
  ifndef rt_corba
    rt_corba = 0
  endif # ! rt_corba
  ifndef corba_messaging
    corba_messaging = 0
  endif # ! corba_messaging
  CPPFLAGS += -DTAO_HAS_MINIMUM_CORBA
else  # minimum_corba
  override minimum_corba = 0
endif # minimum_corba

ifeq ($(ami),1)
  ifndef ami_callback
    ami_callback = 1
  endif # ! ami_callback
  ifndef ami_poller
    ami_poller = 1
  endif # ! ami_poller
  CPPFLAGS += -DTAO_HAS_AMI=1
else  # ami
  override ami = 0
  CPPFLAGS += -DTAO_HAS_AMI=0
endif # ami

ifeq ($(ami_poller),1)
  CPPFLAGS += -DTAO_HAS_AMI_POLLER=1
else  # ami_poller
  override ami_poller = 0
  CPPFLAGS += -DTAO_HAS_AMI_POLLER=0
endif # ami_poller

ifeq ($(ami_callback),1)
  CPPFLAGS += -DTAO_HAS_AMI_CALLBACK=1
else  # ami_callback
  override ami_callback = 0
  CPPFLAGS += -DTAO_HAS_AMI_CALLBACK=0
endif # ami_callback

ifeq ($(rt_corba),0)
  CPPFLAGS += -DTAO_HAS_RT_CORBA=0
else  # rt_corba
  override rt_corba = 1
  CPPFLAGS += -DTAO_HAS_RT_CORBA=1
endif # rt_corba

ifeq ($(corba_messaging),0)
  CPPFLAGS += -DTAO_HAS_CORBA_MESSAGING=0
else  # corba_messaging
  override corba_messaging = 1
  CPPFLAGS += -DTAO_HAS_CORBA_MESSAGING=1
endif # corba_messaging

ifeq (1,$(CROSS-COMPILE))
else
ifeq (1,$(ACE_HAS_GNUG_PRE_2_8))
endif # ! ACE_HAS_GNUG_PRE_2_8
endif # ! CROSS-COMPILE

ifeq ($(interface_repo),1)
  CPPFLAGS += -DTAO_HAS_INTERFACE_REPOSITORY=1
endif # interface_repo

####
#### Event Channel customization.
####
# Uncomment out the following line (or invoke make with
# TAO_LACKS_EVENT_CHANNEL_ANY=1)
# if you don't want support for anys in the Event Channel.
# TAO_LACKS_EVENT_CHANNEL_ANY = 1
ifneq ($(TAO_LACKS_EVENT_CHANNEL_ANY),)
  override TAO_LACKS_EVENT_CHANNEL_ANY = -DTAO_LACKS_EVENT_CHANNEL_ANY
endif

# TAO_LACKS_EVENT_CHANNEL_OCTET_SEQUENCE = 1
ifneq ($(TAO_LACKS_EVENT_CHANNEL_OCTET_SEQUENCE),)
  override TAO_LACKS_EVENT_CHANNEL_OCTET_SEQUENCE := \
 -DTAO_LACKS_EVENT_CHANNEL_OCTET_SEQUENCE
endif

# TAO_LACKS_EVENT_CHANNEL_TIMESTAMPS = 1
ifneq ($(TAO_LACKS_EVENT_CHANNEL_TIMESTAMPS),)
  override TAO_LACKS_EVENT_CHANNEL_TIMESTAMPS := \
 -DTAO_LACKS_EVENT_CHANNEL_TIMESTAMPS
endif

TAO_IDLFLAGS += \
        $(TAO_LACKS_EVENT_CHANNEL_ANY) \
        $(TAO_LACKS_EVENT_CHANNEL_OCTET_SEQUENCE) \
        $(TAO_LACKS_EVENT_CHANNEL_TIMESTAMPS)

CPPFLAGS += \
        $(TAO_LACKS_EVENT_CHANNEL_ANY) \
        $(TAO_LACKS_EVENT_CHANNEL_OCTET_SEQUENCE) \
        $(TAO_LACKS_EVENT_CHANNEL_TIMESTAMPS)

####
#### Build rules.
####
IDL_EXT_MINUS=C.i C.cpp S.h S.i S.cpp S_T.h S_T.i S_T.cpp
IDL_EXT=C.h $(IDL_EXT_MINUS)
IDL_EXT2_MINUS=Cli.i Cli.cpp Ser.h Ser.i Ser.cpp Ser_T.h Ser_T.i Ser_T.cpp
IDL_EXT2=Cli.h $(IDL_EXT2_MINUS)

.SUFFIXES: $(IDL_EXT)

ifneq ($(use_flick),1)
$(foreach ext, $(IDL_EXT_MINUS), %$(ext)): %C.h
	@/bin/true $@

%C.h: %.idl $(TAO_IDL)
	$(TAO_IDL) $(TAO_IDLFLAGS) $<

$(foreach ext, $(IDL_EXT2_MINUS), %$(ext)): %Cli.h
	@/bin/true $@

%Cli.h: %.idl $(TAO_IDL)
	$(TAO_IDL) $(TAO_IDLFLAGS) $<

else
#### You can redefine the following in your platform_macros.GNU

ifndef FLICK_SRCDIR
  FLICK_SRCDIR  =       /project/doc/nanbor/mom
endif
ifndef FLICK_OBJDIR
  FLICK_OBJDIR  =       /project/doc/nanbor/dad
endif

CCFLAGS += -I$(FLICK_OBJDIR)/runtime/headers -I$(FLICK_SRCDIR)/runtime/headers

CORBA_FE=$(FLICK_OBJDIR)/bin/flick-fe-newcorba
CORBA_PG=$(FLICK_OBJDIR)/bin/flick-c-pfe-corbaxx
CORBA_BE=$(FLICK_OBJDIR)/bin/flick-c-pbe-iiopxx

$(foreach ext, $(IDL_EXT) $(IDL_EXT2), %$(ext)): %.idl
	$(CORBA_FE) $< && \
	$(CORBA_PG) -c -o $*-client.prc $*.aoi && \
	$(CORBA_BE) $*-client.prc -o $*C.cpp -h $*C.h -f $*S.h --presentation_implementation $(FLICK_SRCDIR)/runtime/headers/flick/pres/tao_cxx.scml && \
	echo > $*C.i
	$(CORBA_FE) $< && \
	$(CORBA_PG) -s -o $*-server.prc $*.aoi && \
	$(CORBA_BE) $*-server.prc --no_mu_stubs -o $*S.cpp -h $*S.h -F $*C.h --presentation_implementation $(FLICK_SRCDIR)/runtime/headers/flick/pres/tao_cxx.scml && \
	echo > $*S.i && \
        echo > $*S_T.h && \
        echo > $*S_T.i && \
        echo > $*S_T.cpp

endif
