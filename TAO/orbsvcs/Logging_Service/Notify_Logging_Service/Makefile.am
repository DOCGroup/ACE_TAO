##  Process this file with automake to create Makefile.in

ACE_ROOT = $(top_srcdir)/..
ACE_BUILDDIR = $(top_builddir)/..
TAO_ROOT = $(top_srcdir)
TAO_BUILDDIR = $(top_builddir)

## Makefile.Notify_Logging_Service.am 

if !BUILD_MINIMUM_CORBA

noinst_PROGRAMS = Notify_Logging_Service

Notify_Logging_Service_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_HAS_TYPED_EVENT_CHANNEL

Notify_Logging_Service_SOURCES = \
  Notify_Logging_Server.cpp \
  Notify_Logging_Service.cpp \
  Notify_Logging_Service.h

Notify_Logging_Service_LDADD = \
  $(TAO_BUILDDIR)/tao/libTAO_IORTable.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_DsNotifyLogAdmin_Serv.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_DsLogAdmin_Serv.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_ETCL.la \
  $(TAO_BUILDDIR)/tao/libTAO_DynamicAny.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_DsNotifyLogAdmin_Skel.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNotification_Skel.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_DsEventLogAdmin_Skel.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosEvent_Skel.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_DsLogAdmin_Skel.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_DsNotifyLogAdmin.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNotification.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_DsEventLogAdmin.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosEvent.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_DsLogAdmin.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

endif !BUILD_MINIMUM_CORBA

## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -f gcctemp.c gcctemp so_locations
	-rm -rf ptrepository SunWS_cache Templates.DB
