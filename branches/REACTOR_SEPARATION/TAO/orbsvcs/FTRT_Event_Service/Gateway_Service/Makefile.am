##  Process this file with automake to create Makefile.in

ACE_ROOT = $(top_srcdir)/..
ACE_BUILDDIR = $(top_builddir)/..
TAO_ROOT = $(top_srcdir)
TAO_BUILDDIR = $(top_builddir)

## Makefile.FTRTEC_Gateway_Service.am 

if BUILD_ACE_UUID
if !BUILD_MINIMUM_CORBA

noinst_PROGRAMS = ftrtec_gateway_service

ftrtec_gateway_service_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -I$(TAO_ROOT)/orbsvcs/FtRtEvent/Utils

ftrtec_gateway_service_SOURCES = \
  FTRTEC_Gateway_Service.cpp

ftrtec_gateway_service_LDADD = \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_FTRT_ClientORB.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_FtRtEvent.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_RTEvent.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_Svc_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_FT_ClientORB.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_FTORB_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORManip.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

endif !BUILD_MINIMUM_CORBA
endif BUILD_ACE_UUID

## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -f gcctemp.c gcctemp so_locations
	-rm -rf ptrepository SunWS_cache Templates.DB
