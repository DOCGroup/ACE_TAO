##  Process this file with automake to create Makefile.in

ACE_ROOT = $(top_srcdir)/..
ACE_BUILDDIR = $(top_builddir)/..
TAO_ROOT = $(top_srcdir)
TAO_BUILDDIR = $(top_builddir)

## Makefile.Trading_Service.am 

noinst_PROGRAMS = Trading_Service

Trading_Service_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs/orbsvcs

Trading_Service_SOURCES = \
  Trading_Server.cpp \
  Trading_Service.cpp

Trading_Service_LDADD = \
 $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosTrading.la \
 $(TAO_BUILDDIR)/tao/libTAO_DynamicAny.la \
 $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming.la \
 $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_Svc_Utils.la \
 $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
 $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
 $(TAO_BUILDDIR)/tao/libTAO_IORInterceptor.la \
 $(TAO_BUILDDIR)/tao/libTAO_ObjRefTemplate.la \
 $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
 $(TAO_BUILDDIR)/tao/libTAO_IORTable.la \
 $(TAO_BUILDDIR)/tao/libTAO.la \
 $(ACE_BUILDDIR)/ace/libACE.la

noinst_HEADERS = \
  Trading_Service.h

## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -f gcctemp.c gcctemp so_locations
	-rm -rf ptrepository SunWS_cache Templates.DB
