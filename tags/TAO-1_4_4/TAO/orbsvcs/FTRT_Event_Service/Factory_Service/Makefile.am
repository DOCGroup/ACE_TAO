##  Process this file with automake to create Makefile.in

ACE_ROOT = $(top_srcdir)/..
ACE_BUILDDIR = $(top_builddir)/..
TAO_ROOT = $(top_srcdir)
TAO_BUILDDIR = $(top_builddir)

## Makefile.FTRTEC_Factory_Service.am 

if BUILD_ACE_UUID
if !BUILD_MINIMUM_CORBA

noinst_PROGRAMS = ftrtec_factory_service

ftrtec_factory_service_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_HAS_TYPED_EVENT_CHANNEL

ftrtec_factory_service_SOURCES = \
  EventChannelFactory_i.cpp \
  FTRTEC_Factory_Service.cpp \
  EventChannelFactory_i.h

ftrtec_factory_service_LDADD = \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosEvent_Skel.la \
  $(TAO_BUILDDIR)/tao/libTAO_IFR_Client.la \
  $(TAO_BUILDDIR)/tao/libTAO_DynamicInterface.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_FaultTolerance.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_FT_ServerORB.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_FT_ClientORB.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_FTORB_Utils.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_PortableGroup.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORManip.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNotification.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosEvent.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

endif !BUILD_MINIMUM_CORBA
endif BUILD_ACE_UUID

## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -f gcctemp.c gcctemp so_locations
	-rm -rf ptrepository SunWS_cache Templates.DB
