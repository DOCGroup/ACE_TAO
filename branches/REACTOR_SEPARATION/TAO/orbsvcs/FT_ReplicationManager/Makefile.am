##  Process this file with automake to create Makefile.in

ACE_ROOT = $(top_srcdir)/..
ACE_BUILDDIR = $(top_builddir)/..
TAO_ROOT = $(top_srcdir)
TAO_BUILDDIR = $(top_builddir)

## Makefile.FT_ReplicationManager_Lib.am 

if BUILD_ACE_UUID
if !BUILD_MINIMUM_CORBA

lib_LTLIBRARIES = libTAO_ReplicationManagerLib.la

libTAO_ReplicationManagerLib_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_HAS_TYPED_EVENT_CHANNEL \
  -DTAO_REPLICATIONMANAGERLIB_BUILD_DLL

libTAO_ReplicationManagerLib_la_SOURCES = \
  FT_DefaultFaultAnalyzer.cpp \
  FT_FaultAnalyzer.cpp \
  FT_FaultConsumer.cpp \
  FT_FaultEventDescriptor.cpp

libTAO_ReplicationManagerLib_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_ReplicationManagerLib_la_LIBADD = \
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

noinst_HEADERS = \
  FT_DefaultFaultAnalyzer.h \
  FT_FaultAnalyzer.h \
  FT_FaultConsumer.h \
  FT_FaultEventDescriptor.h

endif !BUILD_MINIMUM_CORBA
endif BUILD_ACE_UUID

## Makefile.FT_ReplicationManager.am 

if BUILD_ACE_UUID
if !BUILD_MINIMUM_CORBA

bin_PROGRAMS = FT_ReplicationManager

FT_ReplicationManager_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_HAS_TYPED_EVENT_CHANNEL

FT_ReplicationManager_SOURCES = \
  FT_Property_Validator.cpp \
  FT_ReplicationManager.cpp \
  FT_ReplicationManagerFaultAnalyzer.cpp \
  FT_ReplicationManager_Main.cpp \
  FT_Property_Validator.h \
  FT_ReplicationManager.h \
  FT_ReplicationManagerFaultAnalyzer.h

FT_ReplicationManager_LDADD = \
  libTAO_ReplicationManagerLib.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORTable.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNotification_Skel.la \
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
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNotification.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosEvent.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

endif !BUILD_MINIMUM_CORBA
endif BUILD_ACE_UUID

## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -f gcctemp.c gcctemp so_locations
	-rm -rf ptrepository SunWS_cache Templates.DB
