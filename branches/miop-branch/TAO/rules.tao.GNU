#----------------------------------------------------------------------------
#
#       $Id$
#
#       Common Makefile rules for all of TAO
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

TAO_IDL_DEP = $(TAO_IDL)$(EXEEXT)

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
    ifneq ($(mingw32),1)
      TAO_IDL_PREPROCESSOR := $(shell type $(CXX) | sed 's/.* is //')
    else
      TAO_IDL_PREPROCESSOR := \
	$(shell cygpath -w $(shell type $(CXX) | sed 's/.* is //'))
    endif # !mingw32
  endif # ! TAO_IDL_PREPROCESSOR
endif # COMSPEC

####
#### TAO libraries
####
TAO_BASE_LIBS=-lTAO $(ACELIB) $(LIBS)
RT_TAO_BASE_LIBS=-lTAO_RTCORBA $(TAO_BASE_LIBS)
TAO_SRVR_LIBS=-lTAO_PortableServer $(TAO_BASE_LIBS)
RT_TAO_SRVR_LIBS=-lTAO_RTPortableServer -lTAO_PortableServer $(RT_TAO_BASE_LIBS)
TAO_CLNT_LIBS= $(TAO_BASE_LIBS)
RT_TAO_CLNT_LIBS= $(RT_TAO_BASE_LIBS)
TAO_DYNAMIC_LIBS=-lTAO_DynamicInterface

####
#### Macro customization.
####
ifeq ($(findstring g++,$(CXX)),g++)
  ifeq ($(ACE_HAS_GNUG_PRE_2_8),1)
    #### g++ < 2.8 complains about unused parameters in generated code.
    CCFLAGS += -Wno-unused
    ami=0
  endif # g++ < 2.8
endif # g++

ifeq ($(exceptions),1)
  ifeq ($(include_env),1)
    CPPFLAGS += -DTAO_ENV_BKWD_COMPAT
  endif
endif

ifeq ($(minimum_corba),1)
  ifndef rt_corba
    rt_corba = 0
  endif # ! rt_corba
  ifndef corba_messaging
    corba_messaging = 0
  endif # ! corba_messaging
  ifndef ami
    ami = 0
  endif # ! ami
  ifndef interceptors
    interceptors = 0
  endif  # ! interceptors
  CPPFLAGS += -DTAO_HAS_MINIMUM_CORBA=1
else  # minimum_corba
  override minimum_corba = 0
endif # minimum_corba

ifeq ($(ami),0)
  CPPFLAGS += -DTAO_HAS_AMI=0
else
  override ami = 1
  # The following line is not needed,
  # the $TAO_ROOT/tao/orbconf.h file defines it by default
  # CPPFLAGS += -DTAO_HAS_AMI=1
  ifndef ami_callback
    ami_callback = 1
  endif # ! ami_callback
  ifndef ami_poller
    ami_poller = 1
  endif # ! ami_poller
endif # ami

ifeq ($(ami_poller),0)
  CPPFLAGS += -DTAO_HAS_AMI_POLLER=0
else  # ami_poller
  override ami_poller = 1
  # The following line is not needed,
  # the $TAO_ROOT/tao/orbconf.h file defines it by default
  # CPPFLAGS += -DTAO_HAS_AMI_POLLER=1
endif # ami_poller

ifeq ($(ami_callback),0)
  CPPFLAGS += -DTAO_HAS_AMI_CALLBACK=0
else  # ami_callback
  override ami_callback = 1
  # The following line is not needed,
  # the $TAO_ROOT/tao/orbconf.h file defines it by default
  # CPPFLAGS += -DTAO_HAS_AMI_CALLBACK=1
endif # ami_callback

ifeq ($(rt_corba),0)
  CPPFLAGS += -DTAO_HAS_RT_CORBA=0
else  # rt_corba
  override rt_corba = 1
  # The following line is not needed,
  # the $TAO_ROOT/tao/orbconf.h file defines it by default
  # CPPFLAGS += -DTAO_HAS_RT_CORBA=1
endif # rt_corba

ifeq ($(corba_messaging),0)
  CPPFLAGS += -DTAO_HAS_CORBA_MESSAGING=0
else  # corba_messaging
  override corba_messaging = 1
  # The following line is not needed,
  # the $TAO_ROOT/tao/orbconf.h file defines it by default
  # CPPFLAGS += -DTAO_HAS_CORBA_MESSAGING=1
endif # corba_messaging

ifeq ($(interceptors),0)
  CPPFLAGS += -DTAO_HAS_INTERCEPTORS=0
else  # interceptors
  override interceptors = 1
  # The following line is not needed,
  # the $TAO_ROOT/tao/orbconf.h file defines it by default
  # CPPFLAGS += -DTAO_HAS_INTERCEPTORS=1
endif # interceptors

ifeq (1,$(CROSS-COMPILE))
else
ifeq (1,$(ACE_HAS_GNUG_PRE_2_8))
endif # ! ACE_HAS_GNUG_PRE_2_8
endif # ! CROSS-COMPILE

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
IDL_EXT_MINUS=$(IDL_CLIENT_INL_EXT) $(IDL_CLIENT_SRC_EXT) \
	$(IDL_SERVER_HDR_EXT) $(IDL_SERVER_INL_EXT) \
	$(IDL_SERVER_SRC_EXT) $(IDL_SERVER_THDR_EXT) \
	$(IDL_SERVER_TINL_EXT) $(IDL_SERVER_TSRC_EXT)
IDL_EXT=$(IDL_CLIENT_HDR_EXT) $(IDL_EXT_MINUS)
IDL_EXT2_MINUS=Cli.i Cli.cpp Ser.h Ser.i Ser.cpp Ser_T.h Ser_T.i Ser_T.cpp
IDL_EXT2=Cli.h $(IDL_EXT2_MINUS)

.SUFFIXES: $(IDL_EXT)

ifneq ($(use_flick),1)
$(foreach ext, $(IDL_EXT_MINUS), %$(ext)): %$(IDL_CLIENT_HDR_EXT)
	@

ifneq ($(tao_dont_use_idl_make_rule),1)
%C.h: %.idl $(TAO_IDL_DEP)
	$(TAO_IDL) $(TAO_IDLFLAGS) $<
endif

$(foreach ext, $(IDL_EXT2_MINUS), %$(ext)): %Cli.h
	@

ifneq ($(tao_dont_use_idl_make_rule),1)
%Cli.h: %.idl $(TAO_IDL_DEP)
	$(TAO_IDL) $(TAO_IDLFLAGS) $<
endif

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

ifneq ($(tao_dont_use_idl_make_rule),1)
$(foreach ext, $(IDL_EXT) $(IDL_EXT2), %$(ext)): %.idl
	$(CORBA_FE) $< && \
	$(CORBA_PG) -c -o $*-client.prc $*.aoi && \
	$(CORBA_BE) $*-client.prc -o $*$(IDL_CLIENT_SRC_EXT) -h $*$(IDL_CLIENT_HDR_EXT) -f $*$(IDL_SERVER_HDR_EXT) --presentation_implementation $(FLICK_SRCDIR)/runtime/headers/flick/pres/tao_cxx.scml && \
	echo > $*$(IDL_CLIENT_INL_EXT)
	$(CORBA_FE) $< && \
	$(CORBA_PG) -s -o $*-server.prc $*.aoi && \
	$(CORBA_BE) $*-server.prc --no_mu_stubs -o $*$(IDL_SERVER_SRC_EXT) -h $*$(IDL_SERVER_HDR_EXT) -F $*$(IDL_CLIENT_HDR_EXT) --presentation_implementation $(FLICK_SRCDIR)/runtime/headers/flick/pres/tao_cxx.scml && \
	echo > $*$(IDL_SERVER_INL_EXT) && \
	echo > $*$(IDL_SERVER_THDR_EXT) && \
	echo > $*$(IDL_SERVER_TINL_EXT) && \
	echo > $*$(IDL_SERVER_TSRC_EXT)
endif # tao_dont_use_idl_make_rule

endif # use_flick = 1
