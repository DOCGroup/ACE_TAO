##  Process this file with automake to create Makefile.in

ACE_ROOT = $(top_srcdir)/..
ACE_BUILDDIR = $(top_builddir)/..
TAO_ROOT = $(top_srcdir)
TAO_BUILDDIR = $(top_builddir)

TAO_IDL = ACE_ROOT=$(ACE_ROOT) TAO_ROOT=$(TAO_ROOT) $(top_builddir)/TAO_IDL/tao_idl
TAO_IDLFLAGS = -Ge 1 -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -I$(TAO_ROOT) -I$(srcdir) -g $(ACE_BUILDDIR)/apps/gperf/src/gperf

## Makefile.RTEC_Perf.am 

if BUILD_RT_CORBA
if !BUILD_MINIMUM_CORBA

BUILT_SOURCES = \
  Federated_TestS_T.cpp \
  Federated_TestC.inl \
  Federated_TestS.inl \
  Federated_TestS_T.inl \
  Federated_TestC.cpp \
  Federated_TestS.cpp \
  Federated_TestC.h \
  Federated_TestS.h \
  Federated_TestS_T.h

CLEANFILES = \
  Federated_TestS_T.cpp \
  Federated_TestC.inl \
  Federated_TestS.inl \
  Federated_TestS_T.inl \
  Federated_TestC.cpp \
  Federated_TestS.cpp \
  Federated_TestC.h \
  Federated_TestS.h \
  Federated_TestS_T.h \
  Federated_Test-stamp

Federated_TestS_T.cpp Federated_TestC.inl Federated_TestS.inl Federated_TestS_T.inl Federated_TestC.cpp Federated_TestS.cpp Federated_TestC.h Federated_TestS.h Federated_TestS_T.h: Federated_Test-stamp

Federated_Test-stamp: $(srcdir)/Federated_Test.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_RTEC_Perf_Export -Wb,export_include=rtec_perf_export.h $(srcdir)/Federated_Test.idl
	@touch $@

noinst_LTLIBRARIES = libTAO_RTEC_Perf.la

libTAO_RTEC_Perf_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_RTEC_PERF_BUILD_DLL

libTAO_RTEC_Perf_la_SOURCES = \
  Client_Group.cpp \
  Client_Options.cpp \
  Client_Pair.cpp \
  Consumer.cpp \
  Control.cpp \
  EC_Destroyer.cpp \
  Federated_TestC.cpp \
  Federated_TestS.cpp \
  Implicit_Deactivator.cpp \
  Loopback.cpp \
  Loopback_Consumer.cpp \
  Loopback_Pair.cpp \
  Loopback_Supplier.cpp \
  ORB_Holder.cpp \
  ORB_Shutdown.cpp \
  ORB_Task.cpp \
  ORB_Task_Activator.cpp \
  Peer_Base.cpp \
  PriorityBand_Setup.cpp \
  RTCORBA_Setup.cpp \
  RTClient_Setup.cpp \
  RTEC_Initializer.cpp \
  RTPOA_Setup.cpp \
  RTServer_Setup.cpp \
  RT_Class.cpp \
  Send_Task.cpp \
  Send_Task_Stopper.cpp \
  Supplier.cpp \
  SyncScope_Setup.cpp

libTAO_RTEC_Perf_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_RTEC_Perf_la_LIBADD = \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORTable.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_RTCORBAEvent.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_RTEvent.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_Svc_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_RTPortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_RTCORBA.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORInterceptor.la \
  $(TAO_BUILDDIR)/tao/libTAO_ObjRefTemplate.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/tao/libTAO_Strategies.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

noinst_HEADERS = \
  Auto_Disconnect.h \
  Client_Group.h \
  Client_Options.h \
  Client_Pair.h \
  Consumer.h \
  Control.h \
  EC_Destroyer.h \
  Federated_TestC.h \
  Federated_TestS.h \
  Federated_TestS_T.h \
  Implicit_Deactivator.h \
  Loopback.h \
  Loopback_Consumer.h \
  Loopback_Pair.h \
  Loopback_Supplier.h \
  Low_Priority_Setup.h \
  ORB_Holder.h \
  ORB_Shutdown.h \
  ORB_Task.h \
  ORB_Task_Activator.h \
  Peer_Base.h \
  PriorityBand_Setup.h \
  RIR_Narrow.h \
  RTCORBA_Setup.h \
  RTClient_Setup.h \
  RTEC_Initializer.h \
  RTPOA_Setup.h \
  RTServer_Setup.h \
  RT_Class.h \
  Send_Task.h \
  Send_Task_Stopper.h \
  Servant_var.h \
  Shutdown.h \
  Supplier.h \
  SyncScope_Setup.h \
  Task_Activator.h \
  rtec_perf_export.h \
  Auto_Disconnect.inl \
  Client_Group.inl \
  Client_Pair.inl \
  EC_Destroyer.inl \
  Federated_TestC.inl \
  Federated_TestS.inl \
  Federated_TestS_T.inl \
  Implicit_Deactivator.inl \
  Loopback_Pair.inl \
  Low_Priority_Setup.inl \
  ORB_Holder.inl \
  ORB_Shutdown.inl \
  ORB_Task.inl \
  ORB_Task_Activator.inl \
  PriorityBand_Setup.inl \
  RTCORBA_Setup.inl \
  RTClient_Setup.inl \
  RTPOA_Setup.inl \
  RTServer_Setup.inl \
  RT_Class.inl \
  Send_Task_Stopper.inl \
  Servant_var.inl \
  Shutdown.inl \
  SyncScope_Setup.inl \
  Task_Activator.inl \
  Auto_Disconnect.cpp \
  Federated_TestS_T.cpp \
  Low_Priority_Setup.cpp \
  RIR_Narrow.cpp \
  Servant_var.cpp \
  Shutdown.cpp \
  Task_Activator.cpp \
  Federated_Test.idl

endif !BUILD_MINIMUM_CORBA
endif BUILD_RT_CORBA

## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -f gcctemp.c gcctemp so_locations
	-rm -rf ptrepository SunWS_cache Templates.DB
