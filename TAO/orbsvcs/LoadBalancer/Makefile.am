##  Process this file with automake to create Makefile.in

ACE_ROOT = $(top_srcdir)/..
ACE_BUILDDIR = $(top_builddir)/..
TAO_ROOT = $(top_srcdir)
TAO_BUILDDIR = $(top_builddir)

## Makefile.LoadManager.am 

if BUILD_AMI
if BUILD_INTERCEPTORS
if !BUILD_MINIMUM_CORBA

bin_PROGRAMS = LoadManager

LoadManager_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs

LoadManager_SOURCES = \
  LoadManager.cpp \
  Signal_Handler.cpp \
  Signal_Handler.h

LoadManager_LDADD = \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosLoadBalancing.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORInterceptor.la \
  $(TAO_BUILDDIR)/tao/libTAO_ObjRefTemplate.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_PortableGroup.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORManip.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORTable.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

endif !BUILD_MINIMUM_CORBA
endif BUILD_INTERCEPTORS
endif BUILD_AMI

## Makefile.LoadMonitor.am 

if BUILD_AMI
if BUILD_INTERCEPTORS
if !BUILD_MINIMUM_CORBA

bin_PROGRAMS += LoadMonitor

LoadMonitor_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs

LoadMonitor_SOURCES = \
  LoadMonitor.cpp \
  Monitor_Signal_Handler.cpp \
  Push_Handler.cpp \
  Signal_Handler.cpp \
  Monitor_Signal_Handler.h \
  Push_Handler.h \
  Signal_Handler.h

LoadMonitor_LDADD = \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosLoadBalancing.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORInterceptor.la \
  $(TAO_BUILDDIR)/tao/libTAO_ObjRefTemplate.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_PortableGroup.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORManip.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

endif !BUILD_MINIMUM_CORBA
endif BUILD_INTERCEPTORS
endif BUILD_AMI

## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -f gcctemp.c gcctemp so_locations
	-rm -rf ptrepository SunWS_cache Templates.DB
