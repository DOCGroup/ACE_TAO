##  Process this file with automake to create Makefile.in

ACE_ROOT = $(top_srcdir)/..
ACE_BUILDDIR = $(top_builddir)/..
TAO_ROOT = $(top_srcdir)
TAO_BUILDDIR = $(top_builddir)

## Makefile.RTEvent_Logging_Service.am 

noinst_PROGRAMS = RTEvent_Logging_Service

RTEvent_Logging_Service_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs

RTEvent_Logging_Service_SOURCES = \
  RTEvent_Logging_Service.cpp \
  RTEvent_Logging_Service.h

RTEvent_Logging_Service_LDADD = \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_RTEventLogAdmin.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_RTOLDEvent.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_RTSched.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_RTEvent.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_DsLogAdmin.la \
  $(TAO_BUILDDIR)/tao/libTAO_DynamicAny.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_ETCL.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_Svc_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -f gcctemp.c gcctemp so_locations
	-rm -rf ptrepository SunWS_cache Templates.DB
