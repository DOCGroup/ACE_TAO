##  Process this file with automake to create Makefile.in

includedir = @includedir@/orbsvcs

ACE_BUILDDIR = $(top_builddir)/..
ACE_ROOT = $(top_srcdir)/..
TAO_BUILDDIR = $(top_builddir)
TAO_ROOT = $(top_srcdir)

TAO_IDL = ACE_ROOT=$(ACE_ROOT) TAO_ROOT=$(TAO_ROOT) $(TAO_BUILDDIR)/TAO_IDL/tao_idl
TAO_IDLFLAGS = -Ge 1 -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -I$(TAO_ROOT) -I$(srcdir) -g $(ACE_BUILDDIR)/apps/gperf/src/gperf

noinst_HEADERS =
noinst_LTLIBRARIES =
nobase_include_HEADERS =
lib_LTLIBRARIES =
CLEANFILES =
BUILT_SOURCES =

## Makefile.CosNaming.am 

BUILT_SOURCES += \
  CosNamingS_T.cpp \
  CosNamingC.inl \
  CosNamingC.cpp \
  CosNamingC.h

CLEANFILES += \
  CosNamingS_T.cpp \
  CosNamingC.inl \
  CosNamingC.cpp \
  CosNamingC.h \
  CosNaming-stamp

CosNamingS_T.cpp CosNamingC.inl CosNamingC.cpp CosNamingC.h: CosNaming-stamp

CosNaming-stamp: $(srcdir)/CosNaming.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,stub_export_macro=TAO_Naming_Export -Wb,stub_export_include=orbsvcs/Naming/naming_export.h -Wb,skel_export_macro=TAO_Naming_Skel_Export -Wb,skel_export_include=orbsvcs/Naming/naming_skel_export.h $(srcdir)/CosNaming.idl
	@touch $@

lib_LTLIBRARIES += libTAO_CosNaming.la

libTAO_CosNaming_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_NAMING_BUILD_DLL

libTAO_CosNaming_la_SOURCES = \
  CosNamingC.cpp \
  Naming/Naming_Client.cpp

libTAO_CosNaming_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_CosNaming_la_LIBADD = \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  CosNamingC.h \
  Naming/Naming_Client.h \
  Naming/naming_export.h \
  CosNamingC.inl \
  CosNaming.idl

EXTRA_DIST = \
  CosNaming.rc

## Makefile.CosProperty.am 

BUILT_SOURCES += \
  CosPropertyServiceS_T.cpp \
  CosPropertyServiceC.inl \
  CosPropertyServiceS.inl \
  CosPropertyServiceS_T.inl \
  CosPropertyServiceC.cpp \
  CosPropertyServiceS.cpp \
  CosPropertyServiceC.h \
  CosPropertyServiceS.h \
  CosPropertyServiceS_T.h

CLEANFILES += \
  CosPropertyServiceS_T.cpp \
  CosPropertyServiceC.inl \
  CosPropertyServiceS.inl \
  CosPropertyServiceS_T.inl \
  CosPropertyServiceC.cpp \
  CosPropertyServiceS.cpp \
  CosPropertyServiceC.h \
  CosPropertyServiceS.h \
  CosPropertyServiceS_T.h \
  CosPropertyService-stamp

CosPropertyServiceS_T.cpp CosPropertyServiceC.inl CosPropertyServiceS.inl CosPropertyServiceS_T.inl CosPropertyServiceC.cpp CosPropertyServiceS.cpp CosPropertyServiceC.h CosPropertyServiceS.h CosPropertyServiceS_T.h: CosPropertyService-stamp

CosPropertyService-stamp: $(srcdir)/CosPropertyService.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_Property_Export -Wb,export_include=orbsvcs/Property/property_export.h $(srcdir)/CosPropertyService.idl
	@touch $@

Property = \
  CosPropertyServiceC.cpp \
  CosPropertyServiceS.cpp \
  Property/CosPropertyService_i.cpp

lib_LTLIBRARIES += libTAO_CosProperty.la

libTAO_CosProperty_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_PROPERTY_BUILD_DLL

libTAO_CosProperty_la_SOURCES = \
  $(Property)

libTAO_CosProperty_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_CosProperty_la_LIBADD = \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  CosPropertyServiceC.h \
  CosPropertyServiceS.h \
  CosPropertyServiceS_T.h \
  Property/CosPropertyService_i.h \
  CosPropertyServiceC.inl \
  CosPropertyServiceS.inl \
  CosPropertyServiceS_T.inl \
  CosPropertyServiceS_T.cpp \
  CosPropertyService.idl

EXTRA_DIST += \
  CosProperty.rc

## Makefile.AV.am 

BUILT_SOURCES += \
  AVStreamsS_T.cpp \
  AVStreamsC.inl \
  AVStreamsS.inl \
  AVStreamsS_T.inl \
  AVStreamsC.cpp \
  AVStreamsS.cpp \
  AVStreamsC.h \
  AVStreamsS.h \
  AVStreamsS_T.h

CLEANFILES += \
  AVStreamsS_T.cpp \
  AVStreamsC.inl \
  AVStreamsS.inl \
  AVStreamsS_T.inl \
  AVStreamsC.cpp \
  AVStreamsS.cpp \
  AVStreamsC.h \
  AVStreamsS.h \
  AVStreamsS_T.h \
  AVStreams-stamp

AVStreamsS_T.cpp AVStreamsC.inl AVStreamsS.inl AVStreamsS_T.inl AVStreamsC.cpp AVStreamsS.cpp AVStreamsC.h AVStreamsS.h AVStreamsS_T.h: AVStreams-stamp

AVStreams-stamp: $(srcdir)/AVStreams.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_AV_Export -Wb,export_include=orbsvcs/AV/AV_export.h $(srcdir)/AVStreams.idl
	@touch $@

BUILT_SOURCES += \
  Null_MediaCtrlS_T.cpp \
  Null_MediaCtrlC.inl \
  Null_MediaCtrlS.inl \
  Null_MediaCtrlS_T.inl \
  Null_MediaCtrlC.cpp \
  Null_MediaCtrlS.cpp \
  Null_MediaCtrlC.h \
  Null_MediaCtrlS.h \
  Null_MediaCtrlS_T.h

CLEANFILES += \
  Null_MediaCtrlS_T.cpp \
  Null_MediaCtrlC.inl \
  Null_MediaCtrlS.inl \
  Null_MediaCtrlS_T.inl \
  Null_MediaCtrlC.cpp \
  Null_MediaCtrlS.cpp \
  Null_MediaCtrlC.h \
  Null_MediaCtrlS.h \
  Null_MediaCtrlS_T.h \
  Null_MediaCtrl-stamp

Null_MediaCtrlS_T.cpp Null_MediaCtrlC.inl Null_MediaCtrlS.inl Null_MediaCtrlS_T.inl Null_MediaCtrlC.cpp Null_MediaCtrlS.cpp Null_MediaCtrlC.h Null_MediaCtrlS.h Null_MediaCtrlS_T.h: Null_MediaCtrl-stamp

Null_MediaCtrl-stamp: $(srcdir)/Null_MediaCtrl.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_AV_Export -Wb,export_include=orbsvcs/AV/AV_export.h $(srcdir)/Null_MediaCtrl.idl
	@touch $@

BUILT_SOURCES += \
  sfpS_T.cpp \
  sfpC.inl \
  sfpS.inl \
  sfpS_T.inl \
  sfpC.cpp \
  sfpS.cpp \
  sfpC.h \
  sfpS.h \
  sfpS_T.h

CLEANFILES += \
  sfpS_T.cpp \
  sfpC.inl \
  sfpS.inl \
  sfpS_T.inl \
  sfpC.cpp \
  sfpS.cpp \
  sfpC.h \
  sfpS.h \
  sfpS_T.h \
  sfp-stamp

sfpS_T.cpp sfpC.inl sfpS.inl sfpS_T.inl sfpC.cpp sfpS.cpp sfpC.h sfpS.h sfpS_T.h: sfp-stamp

sfp-stamp: $(srcdir)/sfp.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_AV_Export -Wb,export_include=orbsvcs/AV/AV_export.h $(srcdir)/sfp.idl
	@touch $@

AV = \
  AV/AVStreams_i.cpp \
  AV/AV_Core.cpp \
  AV/Endpoint_Strategy.cpp \
  AV/Fill_ACE_QoS.cpp \
  AV/FlowSpec_Entry.cpp \
  AV/MCast.cpp \
  AV/Policy.cpp \
  AV/Protocol_Factory.cpp \
  AV/QoS_UDP.cpp \
  AV/RTCP.cpp \
  AV/RTCP_Channel.cpp \
  AV/RTCP_Packet.cpp \
  AV/RTP.cpp \
  AV/SCTP_SEQ.cpp \
  AV/TCP.cpp \
  AV/Transport.cpp \
  AV/UDP.cpp \
  AV/default_resource.cpp \
  AV/md5c.cpp \
  AV/media_timer.cpp \
  AV/sfp.cpp \
  AVStreamsC.cpp \
  AVStreamsS.cpp \
  Null_MediaCtrlC.cpp \
  Null_MediaCtrlS.cpp \
  sfpC.cpp \
  sfpS.cpp

lib_LTLIBRARIES += libTAO_AV.la

libTAO_AV_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_AV_BUILD_DLL

libTAO_AV_la_SOURCES = \
  $(AV)

libTAO_AV_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_AV_la_LIBADD = \
  libTAO_CosNaming.la \
  libTAO_CosProperty.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  AV/AVStreams_i.h \
  AV/AV_Core.h \
  AV/AV_export.h \
  AV/Endpoint_Strategy.h \
  AV/Endpoint_Strategy_T.h \
  AV/Fill_ACE_QoS.h \
  AV/FlowSpec_Entry.h \
  AV/Flows_T.h \
  AV/MCast.h \
  AV/Policy.h \
  AV/Protocol_Factory.h \
  AV/QoS_UDP.h \
  AV/RTCP.h \
  AV/RTCP_Channel.h \
  AV/RTCP_Packet.h \
  AV/RTP.h \
  AV/Resource_Factory.h \
  AV/SCTP_SEQ.h \
  AV/TCP.h \
  AV/Transport.h \
  AV/UDP.h \
  AV/default_resource.h \
  AV/global.h \
  AV/md5.h \
  AV/media_timer.h \
  AV/ntp-time.h \
  AV/sfp.h \
  AVStreamsC.h \
  AVStreamsS.h \
  AVStreamsS_T.h \
  Channel_Clients_T.h \
  Null_MediaCtrlC.h \
  Null_MediaCtrlS.h \
  Null_MediaCtrlS_T.h \
  sfpC.h \
  sfpS.h \
  sfpS_T.h \
  AV/AVStreams_i.i \
  AV/FlowSpec_Entry.i \
  AV/MCast.i \
  AV/Policy.i \
  AV/QoS_UDP.i \
  AV/Transport.i \
  AV/UDP.i \
  AVStreamsC.inl \
  AVStreamsS.inl \
  AVStreamsS_T.inl \
  Channel_Clients_T.i \
  Null_MediaCtrlC.inl \
  Null_MediaCtrlS.inl \
  Null_MediaCtrlS_T.inl \
  sfpC.inl \
  sfpS.inl \
  sfpS_T.inl \
  AV/Endpoint_Strategy_T.cpp \
  AV/Flows_T.cpp \
  AVStreamsS_T.cpp \
  Channel_Clients_T.cpp \
  Null_MediaCtrlS_T.cpp \
  sfpS_T.cpp \
  AVStreams.idl \
  Null_MediaCtrl.idl \
  sfp.idl

EXTRA_DIST += \
  AV.rc

## Makefile.CosConcurrency.am 

BUILT_SOURCES += \
  CosConcurrencyControlS_T.cpp \
  CosConcurrencyControlC.inl \
  CosConcurrencyControlS.inl \
  CosConcurrencyControlS_T.inl \
  CosConcurrencyControlC.cpp \
  CosConcurrencyControlS.cpp \
  CosConcurrencyControlC.h \
  CosConcurrencyControlS.h \
  CosConcurrencyControlS_T.h

CLEANFILES += \
  CosConcurrencyControlS_T.cpp \
  CosConcurrencyControlC.inl \
  CosConcurrencyControlS.inl \
  CosConcurrencyControlS_T.inl \
  CosConcurrencyControlC.cpp \
  CosConcurrencyControlS.cpp \
  CosConcurrencyControlC.h \
  CosConcurrencyControlS.h \
  CosConcurrencyControlS_T.h \
  CosConcurrencyControl-stamp

CosConcurrencyControlS_T.cpp CosConcurrencyControlC.inl CosConcurrencyControlS.inl CosConcurrencyControlS_T.inl CosConcurrencyControlC.cpp CosConcurrencyControlS.cpp CosConcurrencyControlC.h CosConcurrencyControlS.h CosConcurrencyControlS_T.h: CosConcurrencyControl-stamp

CosConcurrencyControl-stamp: $(srcdir)/CosConcurrencyControl.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_Concurrency_Export -Wb,export_include=orbsvcs/Concurrency/concurrency_export.h $(srcdir)/CosConcurrencyControl.idl
	@touch $@

Concurrency = \
  Concurrency/CC_Lock.cpp \
  Concurrency/CC_LockSet.cpp \
  Concurrency/CC_LockSetFactory.cpp \
  Concurrency/Concurrency_Loader.cpp \
  Concurrency/Concurrency_Utils.cpp \
  CosConcurrencyControlC.cpp \
  CosConcurrencyControlS.cpp

lib_LTLIBRARIES += libTAO_CosConcurrency.la

libTAO_CosConcurrency_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_CONCURRENCY_BUILD_DLL

libTAO_CosConcurrency_la_SOURCES = \
  $(Concurrency)

libTAO_CosConcurrency_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_CosConcurrency_la_LIBADD = \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  Concurrency/CC_Lock.h \
  Concurrency/CC_LockSet.h \
  Concurrency/CC_LockSetFactory.h \
  Concurrency/Concurrency_Loader.h \
  Concurrency/Concurrency_Utils.h \
  CosConcurrencyControlC.h \
  CosConcurrencyControlS.h \
  CosConcurrencyControlS_T.h \
  CosConcurrencyControlC.inl \
  CosConcurrencyControlS.inl \
  CosConcurrencyControlS_T.inl \
  CosConcurrencyControlS_T.cpp \
  CosConcurrencyControl.idl

EXTRA_DIST += \
  CosConcurrency.rc

## Makefile.CosEvent.am 

if !BUILD_MINIMUM_CORBA

BUILT_SOURCES += \
  CosEventChannelAdminS_T.cpp \
  CosEventChannelAdminC.inl \
  CosEventChannelAdminC.cpp \
  CosEventChannelAdminC.h

CLEANFILES += \
  CosEventChannelAdminS_T.cpp \
  CosEventChannelAdminC.inl \
  CosEventChannelAdminC.cpp \
  CosEventChannelAdminC.h \
  CosEventChannelAdmin-stamp

CosEventChannelAdminS_T.cpp CosEventChannelAdminC.inl CosEventChannelAdminC.cpp CosEventChannelAdminC.h: CosEventChannelAdmin-stamp

CosEventChannelAdmin-stamp: $(srcdir)/CosEventChannelAdmin.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,stub_export_macro=TAO_Event_Export -Wb,stub_export_include=orbsvcs/CosEvent/event_export.h -Wb,skel_export_macro=TAO_Event_Skel_Export -Wb,skel_export_include=orbsvcs/CosEvent/event_skel_export.h -Wb,stub_export_macro=TAO_Event_Export -Wb,stub_export_include=orbsvcs/CosEvent/event_export.h -Wb,skel_export_macro=TAO_Event_Skel_Export -Wb,skel_export_include=orbsvcs/CosEvent/event_skel_export.h $(srcdir)/CosEventChannelAdmin.idl
	@touch $@

BUILT_SOURCES += \
  CosEventCommS_T.cpp \
  CosEventCommC.inl \
  CosEventCommC.cpp \
  CosEventCommC.h

CLEANFILES += \
  CosEventCommS_T.cpp \
  CosEventCommC.inl \
  CosEventCommC.cpp \
  CosEventCommC.h \
  CosEventComm-stamp

CosEventCommS_T.cpp CosEventCommC.inl CosEventCommC.cpp CosEventCommC.h: CosEventComm-stamp

CosEventComm-stamp: $(srcdir)/CosEventComm.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,stub_export_macro=TAO_Event_Export -Wb,stub_export_include=orbsvcs/CosEvent/event_export.h -Wb,skel_export_macro=TAO_Event_Skel_Export -Wb,skel_export_include=orbsvcs/CosEvent/event_skel_export.h -Wb,stub_export_macro=TAO_Event_Export -Wb,stub_export_include=orbsvcs/CosEvent/event_export.h -Wb,skel_export_macro=TAO_Event_Skel_Export -Wb,skel_export_include=orbsvcs/CosEvent/event_skel_export.h $(srcdir)/CosEventComm.idl
	@touch $@

BUILT_SOURCES += \
  CosTypedEventChannelAdminS_T.cpp \
  CosTypedEventChannelAdminC.inl \
  CosTypedEventChannelAdminC.cpp \
  CosTypedEventChannelAdminC.h

CLEANFILES += \
  CosTypedEventChannelAdminS_T.cpp \
  CosTypedEventChannelAdminC.inl \
  CosTypedEventChannelAdminC.cpp \
  CosTypedEventChannelAdminC.h \
  CosTypedEventChannelAdmin-stamp

CosTypedEventChannelAdminS_T.cpp CosTypedEventChannelAdminC.inl CosTypedEventChannelAdminC.cpp CosTypedEventChannelAdminC.h: CosTypedEventChannelAdmin-stamp

CosTypedEventChannelAdmin-stamp: $(srcdir)/CosTypedEventChannelAdmin.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,stub_export_macro=TAO_Event_Export -Wb,stub_export_include=orbsvcs/CosEvent/event_export.h -Wb,skel_export_macro=TAO_Event_Skel_Export -Wb,skel_export_include=orbsvcs/CosEvent/event_skel_export.h $(srcdir)/CosTypedEventChannelAdmin.idl
	@touch $@

BUILT_SOURCES += \
  CosTypedEventCommS_T.cpp \
  CosTypedEventCommC.inl \
  CosTypedEventCommC.cpp \
  CosTypedEventCommC.h

CLEANFILES += \
  CosTypedEventCommS_T.cpp \
  CosTypedEventCommC.inl \
  CosTypedEventCommC.cpp \
  CosTypedEventCommC.h \
  CosTypedEventComm-stamp

CosTypedEventCommS_T.cpp CosTypedEventCommC.inl CosTypedEventCommC.cpp CosTypedEventCommC.h: CosTypedEventComm-stamp

CosTypedEventComm-stamp: $(srcdir)/CosTypedEventComm.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,stub_export_macro=TAO_Event_Export -Wb,stub_export_include=orbsvcs/CosEvent/event_export.h -Wb,skel_export_macro=TAO_Event_Skel_Export -Wb,skel_export_include=orbsvcs/CosEvent/event_skel_export.h $(srcdir)/CosTypedEventComm.idl
	@touch $@

lib_LTLIBRARIES += libTAO_CosEvent.la

libTAO_CosEvent_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_HAS_TYPED_EVENT_CHANNEL \
  -DTAO_EVENT_BUILD_DLL

libTAO_CosEvent_la_SOURCES = \
  CosEventChannelAdminC.cpp \
  CosEventCommC.cpp \
  CosTypedEventChannelAdminC.cpp \
  CosTypedEventCommC.cpp

libTAO_CosEvent_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_CosEvent_la_LIBADD = \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  CosEvent/event_export.h \
  CosEventChannelAdminC.h \
  CosEventCommC.h \
  CosTypedEventChannelAdminC.h \
  CosTypedEventCommC.h \
  CosEventChannelAdminC.inl \
  CosEventCommC.inl \
  CosTypedEventChannelAdminC.inl \
  CosTypedEventCommC.inl \
  CosEventChannelAdmin.idl \
  CosEventComm.idl \
  CosTypedEventChannelAdmin.idl \
  CosTypedEventComm.idl

endif !BUILD_MINIMUM_CORBA

EXTRA_DIST += \
  CosEvent.rc

## Makefile.Svc_Utils.am 

BUILT_SOURCES += \
  RtecBaseS_T.cpp \
  RtecBaseC.inl \
  RtecBaseS.inl \
  RtecBaseS_T.inl \
  RtecBaseC.cpp \
  RtecBaseS.cpp \
  RtecBaseC.h \
  RtecBaseS.h \
  RtecBaseS_T.h

CLEANFILES += \
  RtecBaseS_T.cpp \
  RtecBaseC.inl \
  RtecBaseS.inl \
  RtecBaseS_T.inl \
  RtecBaseC.cpp \
  RtecBaseS.cpp \
  RtecBaseC.h \
  RtecBaseS.h \
  RtecBaseS_T.h \
  RtecBase-stamp

RtecBaseS_T.cpp RtecBaseC.inl RtecBaseS.inl RtecBaseS_T.inl RtecBaseC.cpp RtecBaseS.cpp RtecBaseC.h RtecBaseS.h RtecBaseS_T.h: RtecBase-stamp

RtecBase-stamp: $(srcdir)/RtecBase.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_Svc_Utils_Export -Wb,export_include=orbsvcs/svc_utils_export.h $(srcdir)/RtecBase.idl
	@touch $@

BUILT_SOURCES += \
  TimeBaseS_T.cpp \
  TimeBaseC.inl \
  TimeBaseS.inl \
  TimeBaseS_T.inl \
  TimeBaseC.cpp \
  TimeBaseS.cpp \
  TimeBaseC.h \
  TimeBaseS.h \
  TimeBaseS_T.h

CLEANFILES += \
  TimeBaseS_T.cpp \
  TimeBaseC.inl \
  TimeBaseS.inl \
  TimeBaseS_T.inl \
  TimeBaseC.cpp \
  TimeBaseS.cpp \
  TimeBaseC.h \
  TimeBaseS.h \
  TimeBaseS_T.h \
  TimeBase-stamp

TimeBaseS_T.cpp TimeBaseC.inl TimeBaseS.inl TimeBaseS_T.inl TimeBaseC.cpp TimeBaseS.cpp TimeBaseC.h TimeBaseS.h TimeBaseS_T.h: TimeBase-stamp

TimeBase-stamp: $(srcdir)/TimeBase.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_Svc_Utils_Export -Wb,export_include=orbsvcs/svc_utils_export.h $(srcdir)/TimeBase.idl
	@touch $@

lib_LTLIBRARIES += libTAO_Svc_Utils.la

libTAO_Svc_Utils_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_SVC_UTILS_BUILD_DLL

libTAO_Svc_Utils_la_SOURCES = \
  IOR_Multicast.cpp \
  RtecBaseC.cpp \
  RtecBaseS.cpp \
  Shutdown_Utilities.cpp \
  TimeBaseC.cpp \
  TimeBaseS.cpp \
  Time_Utilities.cpp

libTAO_Svc_Utils_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_Svc_Utils_la_LIBADD = \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  IOR_Multicast.h \
  RtecBaseC.h \
  RtecBaseS.h \
  RtecBaseS_T.h \
  Shutdown_Utilities.h \
  TimeBaseC.h \
  TimeBaseS.h \
  TimeBaseS_T.h \
  Time_Utilities.h \
  RtecBaseC.inl \
  RtecBaseS.inl \
  RtecBaseS_T.inl \
  TimeBaseC.inl \
  TimeBaseS.inl \
  TimeBaseS_T.inl \
  Time_Utilities.i \
  RtecBaseS_T.cpp \
  TimeBaseS_T.cpp \
  RtecBase.idl \
  TimeBase.idl

EXTRA_DIST += \
  Svc_Utils.rc

## Makefile.CosEvent_Skel.am 

if !BUILD_MINIMUM_CORBA

lib_LTLIBRARIES += libTAO_CosEvent_Skel.la

libTAO_CosEvent_Skel_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_HAS_TYPED_EVENT_CHANNEL \
  -DTAO_HAS_TYPED_EVENT_CHANNEL \
  -DTAO_EVENT_SKEL_BUILD_DLL

libTAO_CosEvent_Skel_la_SOURCES = \
  CosEventChannelAdminS.cpp \
  CosEventCommS.cpp \
  CosTypedEventChannelAdminS.cpp \
  CosTypedEventCommS.cpp

libTAO_CosEvent_Skel_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_CosEvent_Skel_la_LIBADD = \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  libTAO_CosEvent.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  CosEvent/event_skel_export.h \
  CosEventChannelAdminS.h \
  CosEventChannelAdminS_T.h \
  CosEventCommS.h \
  CosEventCommS_T.h \
  CosTypedEventChannelAdminS.h \
  CosTypedEventChannelAdminS_T.h \
  CosTypedEventCommS.h \
  CosTypedEventCommS_T.h \
  CosEventChannelAdminS.inl \
  CosEventChannelAdminS_T.inl \
  CosEventCommS.inl \
  CosEventCommS_T.inl \
  CosTypedEventChannelAdminS.inl \
  CosTypedEventChannelAdminS_T.inl \
  CosTypedEventCommS.inl \
  CosTypedEventCommS_T.inl \
  CosEventChannelAdminS_T.cpp \
  CosEventCommS_T.cpp \
  CosTypedEventChannelAdminS_T.cpp \
  CosTypedEventCommS_T.cpp

endif !BUILD_MINIMUM_CORBA

EXTRA_DIST += \
  CosEvent_Skel.rc

## Makefile.CosEvent_Serv.am 

if BUILD_CORBA_MESSAGING
if !BUILD_MINIMUM_CORBA

CosEvent = \
  CosEvent/CEC_ConsumerAdmin.cpp \
  CosEvent/CEC_ConsumerControl.cpp \
  CosEvent/CEC_Default_Factory.cpp \
  CosEvent/CEC_Dispatching.cpp \
  CosEvent/CEC_Dispatching_Task.cpp \
  CosEvent/CEC_DynamicImplementation.cpp \
  CosEvent/CEC_EventChannel.cpp \
  CosEvent/CEC_Event_Loader.cpp \
  CosEvent/CEC_Factory.cpp \
  CosEvent/CEC_MT_Dispatching.cpp \
  CosEvent/CEC_ProxyPullConsumer.cpp \
  CosEvent/CEC_ProxyPullSupplier.cpp \
  CosEvent/CEC_ProxyPushConsumer.cpp \
  CosEvent/CEC_ProxyPushSupplier.cpp \
  CosEvent/CEC_Pulling_Strategy.cpp \
  CosEvent/CEC_Reactive_ConsumerControl.cpp \
  CosEvent/CEC_Reactive_Pulling_Strategy.cpp \
  CosEvent/CEC_Reactive_SupplierControl.cpp \
  CosEvent/CEC_SupplierAdmin.cpp \
  CosEvent/CEC_SupplierControl.cpp \
  CosEvent/CEC_TypedConsumerAdmin.cpp \
  CosEvent/CEC_TypedEvent.cpp \
  CosEvent/CEC_TypedEventChannel.cpp \
  CosEvent/CEC_TypedProxyPushConsumer.cpp \
  CosEvent/CEC_TypedSupplierAdmin.cpp

lib_LTLIBRARIES += libTAO_CosEvent_Serv.la

libTAO_CosEvent_Serv_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_HAS_TYPED_EVENT_CHANNEL \
  -DTAO_HAS_TYPED_EVENT_CHANNEL \
  -DTAO_EVENT_SERV_BUILD_DLL

libTAO_CosEvent_Serv_la_SOURCES = \
  $(CosEvent)

libTAO_CosEvent_Serv_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_CosEvent_Serv_la_LIBADD = \
  libTAO_Svc_Utils.la \
  libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO_IFR_Client.la \
  $(TAO_BUILDDIR)/tao/libTAO_DynamicInterface.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  libTAO_CosEvent_Skel.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  libTAO_CosEvent.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  CosEvent/CEC_ConsumerAdmin.h \
  CosEvent/CEC_ConsumerControl.h \
  CosEvent/CEC_Default_Factory.h \
  CosEvent/CEC_Dispatching.h \
  CosEvent/CEC_Dispatching_Task.h \
  CosEvent/CEC_DynamicImplementation.h \
  CosEvent/CEC_EventChannel.h \
  CosEvent/CEC_Event_Loader.h \
  CosEvent/CEC_Factory.h \
  CosEvent/CEC_MT_Dispatching.h \
  CosEvent/CEC_ProxyPullConsumer.h \
  CosEvent/CEC_ProxyPullSupplier.h \
  CosEvent/CEC_ProxyPushConsumer.h \
  CosEvent/CEC_ProxyPushSupplier.h \
  CosEvent/CEC_Pulling_Strategy.h \
  CosEvent/CEC_Reactive_ConsumerControl.h \
  CosEvent/CEC_Reactive_Pulling_Strategy.h \
  CosEvent/CEC_Reactive_SupplierControl.h \
  CosEvent/CEC_SupplierAdmin.h \
  CosEvent/CEC_SupplierControl.h \
  CosEvent/CEC_TypedConsumerAdmin.h \
  CosEvent/CEC_TypedEvent.h \
  CosEvent/CEC_TypedEventChannel.h \
  CosEvent/CEC_TypedProxyPushConsumer.h \
  CosEvent/CEC_TypedSupplierAdmin.h \
  CosEvent/CEC_ConsumerAdmin.i \
  CosEvent/CEC_ConsumerControl.i \
  CosEvent/CEC_Default_Factory.i \
  CosEvent/CEC_Dispatching.i \
  CosEvent/CEC_Dispatching_Task.i \
  CosEvent/CEC_DynamicImplementation.i \
  CosEvent/CEC_EventChannel.i \
  CosEvent/CEC_Factory.i \
  CosEvent/CEC_MT_Dispatching.i \
  CosEvent/CEC_ProxyPullConsumer.i \
  CosEvent/CEC_ProxyPullSupplier.i \
  CosEvent/CEC_ProxyPushConsumer.i \
  CosEvent/CEC_ProxyPushSupplier.i \
  CosEvent/CEC_Pulling_Strategy.i \
  CosEvent/CEC_Reactive_ConsumerControl.i \
  CosEvent/CEC_Reactive_Pulling_Strategy.i \
  CosEvent/CEC_Reactive_SupplierControl.i \
  CosEvent/CEC_SupplierAdmin.i \
  CosEvent/CEC_SupplierControl.i \
  CosEvent/CEC_TypedConsumerAdmin.i \
  CosEvent/CEC_TypedEvent.i \
  CosEvent/CEC_TypedEventChannel.i \
  CosEvent/CEC_TypedProxyPushConsumer.i \
  CosEvent/CEC_TypedSupplierAdmin.i

endif !BUILD_MINIMUM_CORBA
endif BUILD_CORBA_MESSAGING

EXTRA_DIST += \
  CosEvent_Serv.rc

## Makefile.CosLifeCycle.am 

BUILT_SOURCES += \
  CosLifeCycleS_T.cpp \
  CosLifeCycleC.inl \
  CosLifeCycleS.inl \
  CosLifeCycleS_T.inl \
  CosLifeCycleC.cpp \
  CosLifeCycleS.cpp \
  CosLifeCycleC.h \
  CosLifeCycleS.h \
  CosLifeCycleS_T.h

CLEANFILES += \
  CosLifeCycleS_T.cpp \
  CosLifeCycleC.inl \
  CosLifeCycleS.inl \
  CosLifeCycleS_T.inl \
  CosLifeCycleC.cpp \
  CosLifeCycleS.cpp \
  CosLifeCycleC.h \
  CosLifeCycleS.h \
  CosLifeCycleS_T.h \
  CosLifeCycle-stamp

CosLifeCycleS_T.cpp CosLifeCycleC.inl CosLifeCycleS.inl CosLifeCycleS_T.inl CosLifeCycleC.cpp CosLifeCycleS.cpp CosLifeCycleC.h CosLifeCycleS.h CosLifeCycleS_T.h: CosLifeCycle-stamp

CosLifeCycle-stamp: $(srcdir)/CosLifeCycle.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_LifeCycle_Export -Wb,export_include=orbsvcs/LifeCycle/lifecycle_export.h $(srcdir)/CosLifeCycle.idl
	@touch $@

BUILT_SOURCES += \
  LifeCycleServiceS_T.cpp \
  LifeCycleServiceC.inl \
  LifeCycleServiceS.inl \
  LifeCycleServiceS_T.inl \
  LifeCycleServiceC.cpp \
  LifeCycleServiceS.cpp \
  LifeCycleServiceC.h \
  LifeCycleServiceS.h \
  LifeCycleServiceS_T.h

CLEANFILES += \
  LifeCycleServiceS_T.cpp \
  LifeCycleServiceC.inl \
  LifeCycleServiceS.inl \
  LifeCycleServiceS_T.inl \
  LifeCycleServiceC.cpp \
  LifeCycleServiceS.cpp \
  LifeCycleServiceC.h \
  LifeCycleServiceS.h \
  LifeCycleServiceS_T.h \
  LifeCycleService-stamp

LifeCycleServiceS_T.cpp LifeCycleServiceC.inl LifeCycleServiceS.inl LifeCycleServiceS_T.inl LifeCycleServiceC.cpp LifeCycleServiceS.cpp LifeCycleServiceC.h LifeCycleServiceS.h LifeCycleServiceS_T.h: LifeCycleService-stamp

LifeCycleService-stamp: $(srcdir)/LifeCycleService.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_LifeCycle_Export -Wb,export_include=orbsvcs/LifeCycle/lifecycle_export.h $(srcdir)/LifeCycleService.idl
	@touch $@

LifeCycle = \
  CosLifeCycleC.cpp \
  CosLifeCycleS.cpp \
  LifeCycleServiceC.cpp \
  LifeCycleServiceS.cpp

lib_LTLIBRARIES += libTAO_CosLifeCycle.la

libTAO_CosLifeCycle_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_LIFECYCLE_BUILD_DLL

libTAO_CosLifeCycle_la_SOURCES = \
  $(LifeCycle)

libTAO_CosLifeCycle_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_CosLifeCycle_la_LIBADD = \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  CosLifeCycleC.h \
  CosLifeCycleS.h \
  CosLifeCycleS_T.h \
  LifeCycleServiceC.h \
  LifeCycleServiceS.h \
  LifeCycleServiceS_T.h \
  CosLifeCycleC.inl \
  CosLifeCycleS.inl \
  CosLifeCycleS_T.inl \
  LifeCycleServiceC.inl \
  LifeCycleServiceS.inl \
  LifeCycleServiceS_T.inl \
  CosLifeCycleS_T.cpp \
  LifeCycleServiceS_T.cpp \
  CosLifeCycle.idl \
  LifeCycleService.idl

EXTRA_DIST += \
  CosLifeCycle.rc

## Makefile.FTORB_Utils.am 

if !BUILD_MINIMUM_CORBA

BUILT_SOURCES += \
  FT_CORBA_ORBS_T.cpp \
  FT_CORBA_ORBC.inl \
  FT_CORBA_ORBS.inl \
  FT_CORBA_ORBS_T.inl \
  FT_CORBA_ORBC.cpp \
  FT_CORBA_ORBS.cpp \
  FT_CORBA_ORBC.h \
  FT_CORBA_ORBS.h \
  FT_CORBA_ORBS_T.h

CLEANFILES += \
  FT_CORBA_ORBS_T.cpp \
  FT_CORBA_ORBC.inl \
  FT_CORBA_ORBS.inl \
  FT_CORBA_ORBS_T.inl \
  FT_CORBA_ORBC.cpp \
  FT_CORBA_ORBS.cpp \
  FT_CORBA_ORBC.h \
  FT_CORBA_ORBS.h \
  FT_CORBA_ORBS_T.h \
  FT_CORBA_ORB-stamp

FT_CORBA_ORBS_T.cpp FT_CORBA_ORBC.inl FT_CORBA_ORBS.inl FT_CORBA_ORBS_T.inl FT_CORBA_ORBC.cpp FT_CORBA_ORBS.cpp FT_CORBA_ORBC.h FT_CORBA_ORBS.h FT_CORBA_ORBS_T.h: FT_CORBA_ORB-stamp

FT_CORBA_ORB-stamp: $(srcdir)/FT_CORBA_ORB.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,skel_export_include=tao/PortableServer/PolicyS.h -Wb,export_macro=TAO_FT_ORB_Utils_Export -Wb,export_include=orbsvcs/FaultTolerance/FT_ORB_Utils_export.h $(srcdir)/FT_CORBA_ORB.idl
	@touch $@

BUILT_SOURCES += \
  PortableGroup_Simple_DSC.inl \
  PortableGroup_Simple_DSS.inl \
  PortableGroup_Simple_DSS_T.inl \
  PortableGroup_Simple_DSC.cpp \
  PortableGroup_Simple_DSC.h \
  PortableGroup_Simple_DSS.h

CLEANFILES += \
  PortableGroup_Simple_DSC.inl \
  PortableGroup_Simple_DSS.inl \
  PortableGroup_Simple_DSS_T.inl \
  PortableGroup_Simple_DSC.cpp \
  PortableGroup_Simple_DSC.h \
  PortableGroup_Simple_DSS.h \
  PortableGroup_Simple_DS-stamp

PortableGroup_Simple_DSC.inl PortableGroup_Simple_DSS.inl PortableGroup_Simple_DSS_T.inl PortableGroup_Simple_DSC.cpp PortableGroup_Simple_DSC.h PortableGroup_Simple_DSS.h: PortableGroup_Simple_DS-stamp

PortableGroup_Simple_DS-stamp: $(srcdir)/PortableGroup_Simple_DS.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,skel_export_include=tao/PortableServer/PolicyS.h -Wb,export_macro=TAO_PortableGroup_Simple_Export -Wb,export_include=orbsvcs/PortableGroup_Simple_export.h $(srcdir)/PortableGroup_Simple_DS.idl
	@touch $@

FTORB_Utils = \
  FT_CORBA_ORBC.cpp \
  FT_CORBA_ORBS.cpp \
  FaultTolerance/FT_IOGR_Property.cpp \
  PortableGroup_Simple_DSC.cpp

lib_LTLIBRARIES += libTAO_FTORB_Utils.la

libTAO_FTORB_Utils_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_PORTABLEGROUP_SIMPLE_HAS_DLL=0 \
  -DTAO_FT_ORB_UTILS_BUILD_DLL

libTAO_FTORB_Utils_la_SOURCES = \
  $(FTORB_Utils)

libTAO_FTORB_Utils_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_FTORB_Utils_la_LIBADD = \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORManip.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  FT_CORBA_ORBC.h \
  FT_CORBA_ORBS.h \
  FT_CORBA_ORBS_T.h \
  FaultTolerance/FT_IOGR_Property.h \
  FaultTolerance/FT_ORB_Utils_export.h \
  PortableGroup_Simple_DSC.h \
  PortableGroup_Simple_DSS.h \
  FT_CORBA_ORBC.inl \
  FT_CORBA_ORBS.inl \
  FT_CORBA_ORBS_T.inl \
  FaultTolerance/FT_IOGR_Property.i \
  PortableGroup_Simple_DSC.inl \
  PortableGroup_Simple_DSS.inl \
  PortableGroup_Simple_DSS_T.inl \
  FT_CORBA_ORBS_T.cpp \
  FT_CORBA_ORB.idl \
  PortableGroup_Simple_DS.idl

endif !BUILD_MINIMUM_CORBA

## Makefile.PortableGroup.am 

if BUILD_CORBA_MESSAGING
if !BUILD_MINIMUM_CORBA

BUILT_SOURCES += \
  PortableGroupS_T.cpp \
  PortableGroupC.inl \
  PortableGroupS.inl \
  PortableGroupS_T.inl \
  PortableGroupC.cpp \
  PortableGroupS.cpp \
  PortableGroupC.h \
  PortableGroupS.h \
  PortableGroupS_T.h

CLEANFILES += \
  PortableGroupS_T.cpp \
  PortableGroupC.inl \
  PortableGroupS.inl \
  PortableGroupS_T.inl \
  PortableGroupC.cpp \
  PortableGroupS.cpp \
  PortableGroupC.h \
  PortableGroupS.h \
  PortableGroupS_T.h \
  PortableGroup-stamp

PortableGroupS_T.cpp PortableGroupC.inl PortableGroupS.inl PortableGroupS_T.inl PortableGroupC.cpp PortableGroupS.cpp PortableGroupC.h PortableGroupS.h PortableGroupS_T.h: PortableGroup-stamp

PortableGroup-stamp: $(srcdir)/PortableGroup.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_PortableGroup_Export -Wb,export_include=orbsvcs/PortableGroup/portablegroup_export.h -GC $(srcdir)/PortableGroup.idl
	@touch $@

BUILT_SOURCES += \
  miopS_T.cpp \
  miopC.inl \
  miopS.inl \
  miopS_T.inl \
  miopC.cpp \
  miopS.cpp \
  miopC.h \
  miopS.h \
  miopS_T.h

CLEANFILES += \
  miopS_T.cpp \
  miopC.inl \
  miopS.inl \
  miopS_T.inl \
  miopC.cpp \
  miopS.cpp \
  miopC.h \
  miopS.h \
  miopS_T.h \
  miop-stamp

miopS_T.cpp miopC.inl miopS.inl miopS_T.inl miopC.cpp miopS.cpp miopC.h miopS.h miopS_T.h: miop-stamp

miop-stamp: $(srcdir)/miop.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_PortableGroup_Export -Wb,export_include=orbsvcs/PortableGroup/portablegroup_export.h $(srcdir)/miop.idl
	@touch $@

PortableGroup = \
  PortableGroup/MIOP.cpp \
  PortableGroup/PG_Default_Property_Validator.cpp \
  PortableGroup/PG_FactoryRegistry.cpp \
  PortableGroup/PG_Factory_Map.cpp \
  PortableGroup/PG_Factory_Set.cpp \
  PortableGroup/PG_GenericFactory.cpp \
  PortableGroup/PG_Group_Factory.cpp \
  PortableGroup/PG_Group_Guard.cpp \
  PortableGroup/PG_Location_Equal_To.cpp \
  PortableGroup/PG_Location_Hash.cpp \
  PortableGroup/PG_Location_Map.cpp \
  PortableGroup/PG_MemberInfo.cpp \
  PortableGroup/PG_Null_Property_Validator.cpp \
  PortableGroup/PG_ObjectGroupManager.cpp \
  PortableGroup/PG_ObjectGroup_Map.cpp \
  PortableGroup/PG_Object_Group.cpp \
  PortableGroup/PG_Object_Group_Manipulator.cpp \
  PortableGroup/PG_Operators.cpp \
  PortableGroup/PG_Properties_Encoder.cpp \
  PortableGroup/PG_Properties_Support.cpp \
  PortableGroup/PG_PropertyManager.cpp \
  PortableGroup/PG_Property_Set.cpp \
  PortableGroup/PG_Property_Utils.cpp \
  PortableGroup/PG_Utils.cpp \
  PortableGroup/POA_Hooks.cpp \
  PortableGroup/PortableGroup_Acceptor_Registry.cpp \
  PortableGroup/PortableGroup_Loader.cpp \
  PortableGroup/PortableGroup_ORBInitializer.cpp \
  PortableGroup/PortableGroup_Request_Dispatcher.cpp \
  PortableGroup/Portable_Group_Map.cpp \
  PortableGroup/UIPMC_Acceptor.cpp \
  PortableGroup/UIPMC_Connection_Handler.cpp \
  PortableGroup/UIPMC_Connector.cpp \
  PortableGroup/UIPMC_Endpoint.cpp \
  PortableGroup/UIPMC_Factory.cpp \
  PortableGroup/UIPMC_Message_Block_Data_Iterator.cpp \
  PortableGroup/UIPMC_Profile.cpp \
  PortableGroup/UIPMC_Transport.cpp \
  PortableGroup/UIPMC_Wait_Never.cpp \
  PortableGroupC.cpp \
  PortableGroupS.cpp \
  PortableGroup_Simple_DSC.cpp \
  miopC.cpp \
  miopS.cpp

lib_LTLIBRARIES += libTAO_PortableGroup.la

libTAO_PortableGroup_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_PORTABLEGROUP_BUILD_DLL \
  -DTAO_PORTABLEGROUP_SIMPLE_BUILD_DLL

libTAO_PortableGroup_la_SOURCES = \
  $(PortableGroup)

libTAO_PortableGroup_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_PortableGroup_la_LIBADD = \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORManip.la \
  libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  PortableGroup/MIOP.h \
  PortableGroup/PG_Default_Property_Validator.h \
  PortableGroup/PG_FactoryRegistry.h \
  PortableGroup/PG_Factory_Map.h \
  PortableGroup/PG_Factory_Set.h \
  PortableGroup/PG_GenericFactory.h \
  PortableGroup/PG_Group_Factory.h \
  PortableGroup/PG_Group_Guard.h \
  PortableGroup/PG_Location_Equal_To.h \
  PortableGroup/PG_Location_Hash.h \
  PortableGroup/PG_Location_Map.h \
  PortableGroup/PG_MemberInfo.h \
  PortableGroup/PG_Null_Property_Validator.h \
  PortableGroup/PG_ObjectGroupManager.h \
  PortableGroup/PG_ObjectGroup_Map.h \
  PortableGroup/PG_Object_Group.h \
  PortableGroup/PG_Object_Group_Manipulator.h \
  PortableGroup/PG_Operators.h \
  PortableGroup/PG_Properties_Encoder.h \
  PortableGroup/PG_Properties_Support.h \
  PortableGroup/PG_PropertyManager.h \
  PortableGroup/PG_Property_Set.h \
  PortableGroup/PG_Property_Utils.h \
  PortableGroup/PG_Utils.h \
  PortableGroup/POA_Hooks.h \
  PortableGroup/PortableGroup_Acceptor_Registry.h \
  PortableGroup/PortableGroup_Loader.h \
  PortableGroup/PortableGroup_ORBInitializer.h \
  PortableGroup/PortableGroup_Request_Dispatcher.h \
  PortableGroup/Portable_Group_Map.h \
  PortableGroup/UIPMC_Acceptor.h \
  PortableGroup/UIPMC_Connection_Handler.h \
  PortableGroup/UIPMC_Connector.h \
  PortableGroup/UIPMC_Endpoint.h \
  PortableGroup/UIPMC_Factory.h \
  PortableGroup/UIPMC_Message_Block_Data_Iterator.h \
  PortableGroup/UIPMC_Profile.h \
  PortableGroup/UIPMC_Transport.h \
  PortableGroup/UIPMC_Wait_Never.h \
  PortableGroup/portablegroup_export.h \
  PortableGroupC.h \
  PortableGroupS.h \
  PortableGroupS_T.h \
  miopC.h \
  miopS.h \
  miopS_T.h \
  PortableGroup/PG_Location_Equal_To.inl \
  PortableGroup/PG_Location_Hash.inl \
  PortableGroup/PG_Properties_Encoder.inl \
  PortableGroup/UIPMC_Acceptor.i \
  PortableGroup/UIPMC_Connection_Handler.i \
  PortableGroup/UIPMC_Endpoint.i \
  PortableGroup/UIPMC_Profile.i \
  PortableGroup/UIPMC_Transport.i \
  PortableGroupC.inl \
  PortableGroupS.inl \
  PortableGroupS_T.inl \
  miopC.inl \
  miopS.inl \
  miopS_T.inl \
  PortableGroupS_T.cpp \
  miopS_T.cpp \
  PortableGroup.idl \
  miop.idl

endif !BUILD_MINIMUM_CORBA
endif BUILD_CORBA_MESSAGING

EXTRA_DIST += \
  PortableGroup.rc

## Makefile.CosLoadBalancing.am 

if BUILD_CORBA_MESSAGING
if BUILD_AMI
if !BUILD_MINIMUM_CORBA

BUILT_SOURCES += \
  CosLoadBalancingS_T.cpp \
  CosLoadBalancingC.inl \
  CosLoadBalancingS.inl \
  CosLoadBalancingS_T.inl \
  CosLoadBalancingC.cpp \
  CosLoadBalancingS.cpp \
  CosLoadBalancingC.h \
  CosLoadBalancingS.h \
  CosLoadBalancingS_T.h

CLEANFILES += \
  CosLoadBalancingS_T.cpp \
  CosLoadBalancingC.inl \
  CosLoadBalancingS.inl \
  CosLoadBalancingS_T.inl \
  CosLoadBalancingC.cpp \
  CosLoadBalancingS.cpp \
  CosLoadBalancingC.h \
  CosLoadBalancingS.h \
  CosLoadBalancingS_T.h \
  CosLoadBalancing-stamp

CosLoadBalancingS_T.cpp CosLoadBalancingC.inl CosLoadBalancingS.inl CosLoadBalancingS_T.inl CosLoadBalancingC.cpp CosLoadBalancingS.cpp CosLoadBalancingC.h CosLoadBalancingS.h CosLoadBalancingS_T.h: CosLoadBalancing-stamp

CosLoadBalancing-stamp: $(srcdir)/CosLoadBalancing.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -GC -Wb,export_macro=TAO_LoadBalancing_Export -Wb,export_include=orbsvcs/LoadBalancing/LoadBalancing_export.h $(srcdir)/CosLoadBalancing.idl
	@touch $@

BUILT_SOURCES += \
  LB_ORTC.inl \
  LB_ORTS.inl \
  LB_ORTS_T.inl \
  LB_ORTC.cpp \
  LB_ORTC.h \
  LB_ORTS.h \
  LB_ORTS_T.h

CLEANFILES += \
  LB_ORTC.inl \
  LB_ORTS.inl \
  LB_ORTS_T.inl \
  LB_ORTC.cpp \
  LB_ORTC.h \
  LB_ORTS.h \
  LB_ORTS_T.h \
  LB_ORT-stamp

LB_ORTC.inl LB_ORTS.inl LB_ORTS_T.inl LB_ORTC.cpp LB_ORTC.h LB_ORTS.h LB_ORTS_T.h: LB_ORT-stamp

LB_ORT-stamp: $(srcdir)/LB_ORT.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -GC -Wb,export_macro=TAO_LoadBalancing_Export -Wb,export_include=orbsvcs/LoadBalancing/LoadBalancing_export.h $(srcdir)/LB_ORT.idl
	@touch $@

CosLoadBalancing = \
  CosLoadBalancingC.cpp \
  CosLoadBalancingS.cpp \
  LB_ORTC.cpp \
  LoadBalancing/LB_CPU_Load_Average_Monitor.cpp \
  LoadBalancing/LB_CPU_Utilization_Monitor.cpp \
  LoadBalancing/LB_ClientComponent.cpp \
  LoadBalancing/LB_ClientORBInitializer.cpp \
  LoadBalancing/LB_ClientRequestInterceptor.cpp \
  LoadBalancing/LB_Component.cpp \
  LoadBalancing/LB_IORInterceptor.cpp \
  LoadBalancing/LB_LeastLoaded.cpp \
  LoadBalancing/LB_LoadAlert.cpp \
  LoadBalancing/LB_LoadAlertInfo.cpp \
  LoadBalancing/LB_LoadAlertMap.cpp \
  LoadBalancing/LB_LoadAlert_Handler.cpp \
  LoadBalancing/LB_LoadAverage.cpp \
  LoadBalancing/LB_LoadListMap.cpp \
  LoadBalancing/LB_LoadManager.cpp \
  LoadBalancing/LB_LoadMap.cpp \
  LoadBalancing/LB_LoadMinimum.cpp \
  LoadBalancing/LB_Location_Index_Map.cpp \
  LoadBalancing/LB_MemberLocator.cpp \
  LoadBalancing/LB_MonitorMap.cpp \
  LoadBalancing/LB_ORBInitializer.cpp \
  LoadBalancing/LB_ObjectReferenceFactory.cpp \
  LoadBalancing/LB_Pull_Handler.cpp \
  LoadBalancing/LB_Random.cpp \
  LoadBalancing/LB_RoundRobin.cpp \
  LoadBalancing/LB_ServerRequestInterceptor.cpp

lib_LTLIBRARIES += libTAO_CosLoadBalancing.la

libTAO_CosLoadBalancing_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_LOADBALANCING_BUILD_DLL

libTAO_CosLoadBalancing_la_SOURCES = \
  $(CosLoadBalancing)

libTAO_CosLoadBalancing_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_CosLoadBalancing_la_LIBADD = \
  $(TAO_BUILDDIR)/tao/libTAO_IORInterceptor.la \
  $(TAO_BUILDDIR)/tao/libTAO_ObjRefTemplate.la \
  libTAO_PortableGroup.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORManip.la \
  libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  CosLoadBalancingC.h \
  CosLoadBalancingS.h \
  CosLoadBalancingS_T.h \
  LB_ORTC.h \
  LB_ORTS.h \
  LB_ORTS_T.h \
  LoadBalancing/LB_CPU_Load_Average_Monitor.h \
  LoadBalancing/LB_CPU_Utilization_Monitor.h \
  LoadBalancing/LB_ClientComponent.h \
  LoadBalancing/LB_ClientORBInitializer.h \
  LoadBalancing/LB_ClientRequestInterceptor.h \
  LoadBalancing/LB_Component.h \
  LoadBalancing/LB_IORInterceptor.h \
  LoadBalancing/LB_LeastLoaded.h \
  LoadBalancing/LB_LoadAlert.h \
  LoadBalancing/LB_LoadAlertInfo.h \
  LoadBalancing/LB_LoadAlertMap.h \
  LoadBalancing/LB_LoadAlert_Handler.h \
  LoadBalancing/LB_LoadAverage.h \
  LoadBalancing/LB_LoadListMap.h \
  LoadBalancing/LB_LoadManager.h \
  LoadBalancing/LB_LoadMap.h \
  LoadBalancing/LB_LoadMinimum.h \
  LoadBalancing/LB_Location_Index_Map.h \
  LoadBalancing/LB_MemberLocator.h \
  LoadBalancing/LB_MonitorMap.h \
  LoadBalancing/LB_ORBInitializer.h \
  LoadBalancing/LB_ObjectReferenceFactory.h \
  LoadBalancing/LB_Pull_Handler.h \
  LoadBalancing/LB_Random.h \
  LoadBalancing/LB_RoundRobin.h \
  LoadBalancing/LB_ServerRequestInterceptor.h \
  CosLoadBalancingC.inl \
  CosLoadBalancingS.inl \
  CosLoadBalancingS_T.inl \
  LB_ORTC.inl \
  LB_ORTS.inl \
  LB_ORTS_T.inl \
  LoadBalancing/LB_LeastLoaded.inl \
  LoadBalancing/LB_LoadAlert.inl \
  LoadBalancing/LB_LoadAverage.inl \
  LoadBalancing/LB_LoadMinimum.inl \
  CosLoadBalancingS_T.cpp \
  CosLoadBalancing.idl \
  LB_ORT.idl

endif !BUILD_MINIMUM_CORBA
endif BUILD_AMI
endif BUILD_CORBA_MESSAGING

EXTRA_DIST += \
  CosLoadBalancing.rc

## Makefile.CosNaming_Skel.am 

lib_LTLIBRARIES += libTAO_CosNaming_Skel.la

libTAO_CosNaming_Skel_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_NAMING_SKEL_BUILD_DLL

libTAO_CosNaming_Skel_la_SOURCES = \
  CosNamingS.cpp

libTAO_CosNaming_Skel_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_CosNaming_Skel_la_LIBADD = \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  CosNamingS.h \
  CosNamingS_T.h \
  Naming/naming_skel_export.h \
  CosNamingS.inl \
  CosNamingS_T.inl \
  CosNamingS_T.cpp

EXTRA_DIST += \
  CosNaming_Skel.rc

## Makefile.CosNaming_Serv.am 

if BUILD_CORBA_MESSAGING

Naming = \
  Naming/Entries.cpp \
  Naming/Flat_File_Persistence.cpp \
  Naming/Hash_Naming_Context.cpp \
  Naming/Naming_Context_Interface.cpp \
  Naming/Naming_Loader.cpp \
  Naming/Naming_Server.cpp \
  Naming/Persistent_Context_Index.cpp \
  Naming/Persistent_Entries.cpp \
  Naming/Persistent_Naming_Context.cpp \
  Naming/Storable.cpp \
  Naming/Storable_Naming_Context.cpp \
  Naming/Storable_Naming_Context_Activator.cpp \
  Naming/Transient_Naming_Context.cpp

lib_LTLIBRARIES += libTAO_CosNaming_Serv.la

libTAO_CosNaming_Serv_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_NAMING_SERV_BUILD_DLL

libTAO_CosNaming_Serv_la_SOURCES = \
  $(Naming)

libTAO_CosNaming_Serv_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_CosNaming_Serv_la_LIBADD = \
  $(TAO_BUILDDIR)/tao/libTAO_IORTable.la \
  libTAO_Svc_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  libTAO_CosNaming_Skel.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  Naming/Entries.h \
  Naming/Flat_File_Persistence.h \
  Naming/Hash_Naming_Context.h \
  Naming/Naming_Context_Interface.h \
  Naming/Naming_Loader.h \
  Naming/Naming_Server.h \
  Naming/Persistent_Context_Index.h \
  Naming/Persistent_Entries.h \
  Naming/Persistent_Naming_Context.h \
  Naming/Storable.h \
  Naming/Storable_Naming_Context.h \
  Naming/Storable_Naming_Context_Activator.h \
  Naming/Transient_Naming_Context.h \
  Naming/Storable.inl

endif BUILD_CORBA_MESSAGING

EXTRA_DIST += \
  CosNaming_Serv.rc

## Makefile.CosNotification.am 

if !BUILD_MINIMUM_CORBA

BUILT_SOURCES += \
  CosNotificationS_T.cpp \
  CosNotificationC.inl \
  CosNotificationC.cpp \
  CosNotificationC.h

CLEANFILES += \
  CosNotificationS_T.cpp \
  CosNotificationC.inl \
  CosNotificationC.cpp \
  CosNotificationC.h \
  CosNotification-stamp

CosNotificationS_T.cpp CosNotificationC.inl CosNotificationC.cpp CosNotificationC.h: CosNotification-stamp

CosNotification-stamp: $(srcdir)/CosNotification.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,stub_export_macro=TAO_Notify_Export -Wb,stub_export_include=orbsvcs/Notify/notify_export.h -Wb,skel_export_macro=TAO_Notify_Skel_Export -Wb,skel_export_include=orbsvcs/Notify/notify_skel_export.h $(srcdir)/CosNotification.idl
	@touch $@

BUILT_SOURCES += \
  CosNotifyChannelAdminS_T.cpp \
  CosNotifyChannelAdminC.inl \
  CosNotifyChannelAdminC.cpp \
  CosNotifyChannelAdminC.h

CLEANFILES += \
  CosNotifyChannelAdminS_T.cpp \
  CosNotifyChannelAdminC.inl \
  CosNotifyChannelAdminC.cpp \
  CosNotifyChannelAdminC.h \
  CosNotifyChannelAdmin-stamp

CosNotifyChannelAdminS_T.cpp CosNotifyChannelAdminC.inl CosNotifyChannelAdminC.cpp CosNotifyChannelAdminC.h: CosNotifyChannelAdmin-stamp

CosNotifyChannelAdmin-stamp: $(srcdir)/CosNotifyChannelAdmin.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,stub_export_macro=TAO_Notify_Export -Wb,stub_export_include=orbsvcs/Notify/notify_export.h -Wb,skel_export_macro=TAO_Notify_Skel_Export -Wb,skel_export_include=orbsvcs/Notify/notify_skel_export.h $(srcdir)/CosNotifyChannelAdmin.idl
	@touch $@

BUILT_SOURCES += \
  CosNotifyCommS_T.cpp \
  CosNotifyCommC.inl \
  CosNotifyCommC.cpp \
  CosNotifyCommC.h

CLEANFILES += \
  CosNotifyCommS_T.cpp \
  CosNotifyCommC.inl \
  CosNotifyCommC.cpp \
  CosNotifyCommC.h \
  CosNotifyComm-stamp

CosNotifyCommS_T.cpp CosNotifyCommC.inl CosNotifyCommC.cpp CosNotifyCommC.h: CosNotifyComm-stamp

CosNotifyComm-stamp: $(srcdir)/CosNotifyComm.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,stub_export_macro=TAO_Notify_Export -Wb,stub_export_include=orbsvcs/Notify/notify_export.h -Wb,skel_export_macro=TAO_Notify_Skel_Export -Wb,skel_export_include=orbsvcs/Notify/notify_skel_export.h $(srcdir)/CosNotifyComm.idl
	@touch $@

BUILT_SOURCES += \
  CosNotifyFilterS_T.cpp \
  CosNotifyFilterC.inl \
  CosNotifyFilterC.cpp \
  CosNotifyFilterC.h

CLEANFILES += \
  CosNotifyFilterS_T.cpp \
  CosNotifyFilterC.inl \
  CosNotifyFilterC.cpp \
  CosNotifyFilterC.h \
  CosNotifyFilter-stamp

CosNotifyFilterS_T.cpp CosNotifyFilterC.inl CosNotifyFilterC.cpp CosNotifyFilterC.h: CosNotifyFilter-stamp

CosNotifyFilter-stamp: $(srcdir)/CosNotifyFilter.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,stub_export_macro=TAO_Notify_Export -Wb,stub_export_include=orbsvcs/Notify/notify_export.h -Wb,skel_export_macro=TAO_Notify_Skel_Export -Wb,skel_export_include=orbsvcs/Notify/notify_skel_export.h $(srcdir)/CosNotifyFilter.idl
	@touch $@

BUILT_SOURCES += \
  Event_ForwarderS_T.cpp \
  Event_ForwarderC.inl \
  Event_ForwarderC.cpp \
  Event_ForwarderC.h

CLEANFILES += \
  Event_ForwarderS_T.cpp \
  Event_ForwarderC.inl \
  Event_ForwarderC.cpp \
  Event_ForwarderC.h \
  Event_Forwarder-stamp

Event_ForwarderS_T.cpp Event_ForwarderC.inl Event_ForwarderC.cpp Event_ForwarderC.h: Event_Forwarder-stamp

Event_Forwarder-stamp: $(srcdir)/Event_Forwarder.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,stub_export_macro=TAO_Notify_Export -Wb,stub_export_include=orbsvcs/Notify/notify_export.h -Wb,skel_export_macro=TAO_Notify_Skel_Export -Wb,skel_export_include=orbsvcs/Notify/notify_skel_export.h $(srcdir)/Event_Forwarder.idl
	@touch $@

BUILT_SOURCES += \
  NotifyExtS_T.cpp \
  NotifyExtC.inl \
  NotifyExtC.cpp \
  NotifyExtC.h

CLEANFILES += \
  NotifyExtS_T.cpp \
  NotifyExtC.inl \
  NotifyExtC.cpp \
  NotifyExtC.h \
  NotifyExt-stamp

NotifyExtS_T.cpp NotifyExtC.inl NotifyExtC.cpp NotifyExtC.h: NotifyExt-stamp

NotifyExt-stamp: $(srcdir)/NotifyExt.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,stub_export_macro=TAO_Notify_Export -Wb,stub_export_include=orbsvcs/Notify/notify_export.h -Wb,skel_export_macro=TAO_Notify_Skel_Export -Wb,skel_export_include=orbsvcs/Notify/notify_skel_export.h $(srcdir)/NotifyExt.idl
	@touch $@

lib_LTLIBRARIES += libTAO_CosNotification.la

libTAO_CosNotification_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_HAS_TYPED_EVENT_CHANNEL \
  -DTAO_NOTIFY_BUILD_DLL

libTAO_CosNotification_la_SOURCES = \
  CosNotificationC.cpp \
  CosNotifyChannelAdminC.cpp \
  CosNotifyCommC.cpp \
  CosNotifyFilterC.cpp \
  Event_ForwarderC.cpp \
  Notify/Notify_Extensions.cpp \
  NotifyExtC.cpp

libTAO_CosNotification_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_CosNotification_la_LIBADD = \
  libTAO_CosEvent.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  CosNotificationC.h \
  CosNotifyChannelAdminC.h \
  CosNotifyCommC.h \
  CosNotifyFilterC.h \
  Event_ForwarderC.h \
  Notify/Notify_Extensions.h \
  Notify/notify_export.h \
  NotifyExtC.h \
  CosNotificationC.inl \
  CosNotifyChannelAdminC.inl \
  CosNotifyCommC.inl \
  CosNotifyFilterC.inl \
  Event_ForwarderC.inl \
  NotifyExtC.inl \
  CosNotification.idl \
  CosNotifyChannelAdmin.idl \
  CosNotifyComm.idl \
  CosNotifyFilter.idl \
  Event_Forwarder.idl \
  NotifyExt.idl

endif !BUILD_MINIMUM_CORBA

EXTRA_DIST += \
  CosNotification.rc

## Makefile.ETCL.am 

lib_LTLIBRARIES += libTAO_ETCL.la

libTAO_ETCL_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_ETCL_BUILD_DLL

libTAO_ETCL_la_SOURCES = \
  ETCL/ETCL_Constraint.cpp \
  ETCL/ETCL_Interpreter.cpp \
  ETCL/ETCL_l.cpp \
  ETCL/ETCL_y.cpp

libTAO_ETCL_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_ETCL_la_LIBADD = \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  ETCL/ETCL_Constraint.h \
  ETCL/ETCL_Interpreter.h \
  ETCL/ETCL_y.h \
  ETCL/ETCL_Constraint.i

EXTRA_DIST += \
  ETCL.rc

## Makefile.CosNotification_Skel.am 

if !BUILD_MINIMUM_CORBA

lib_LTLIBRARIES += libTAO_CosNotification_Skel.la

libTAO_CosNotification_Skel_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_HAS_TYPED_EVENT_CHANNEL \
  -DTAO_NOTIFY_SKEL_BUILD_DLL

libTAO_CosNotification_Skel_la_SOURCES = \
  CosNotificationS.cpp \
  CosNotifyChannelAdminS.cpp \
  CosNotifyCommS.cpp \
  CosNotifyFilterS.cpp \
  Event_ForwarderS.cpp \
  NotifyExtS.cpp

libTAO_CosNotification_Skel_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_CosNotification_Skel_la_LIBADD = \
  libTAO_CosEvent_Skel.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  libTAO_CosNotification.la \
  libTAO_CosEvent.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  CosNotificationS.h \
  CosNotificationS_T.h \
  CosNotifyChannelAdminS.h \
  CosNotifyChannelAdminS_T.h \
  CosNotifyCommS.h \
  CosNotifyCommS_T.h \
  CosNotifyFilterS.h \
  CosNotifyFilterS_T.h \
  Event_ForwarderS.h \
  Event_ForwarderS_T.h \
  Notify/notify_skel_export.h \
  NotifyExtS.h \
  NotifyExtS_T.h \
  CosNotificationS.inl \
  CosNotificationS_T.inl \
  CosNotifyChannelAdminS.inl \
  CosNotifyChannelAdminS_T.inl \
  CosNotifyCommS.inl \
  CosNotifyCommS_T.inl \
  CosNotifyFilterS.inl \
  CosNotifyFilterS_T.inl \
  Event_ForwarderS.inl \
  Event_ForwarderS_T.inl \
  NotifyExtS.inl \
  NotifyExtS_T.inl \
  CosNotificationS_T.cpp \
  CosNotifyChannelAdminS_T.cpp \
  CosNotifyCommS_T.cpp \
  CosNotifyFilterS_T.cpp \
  Event_ForwarderS_T.cpp \
  NotifyExtS_T.cpp

endif !BUILD_MINIMUM_CORBA

EXTRA_DIST += \
  CosNotification_Skel.rc

## Makefile.CosNotification_Serv.am 

if !BUILD_MINIMUM_CORBA

Notify = \
  Notify/Admin.cpp \
  Notify/AdminProperties.cpp \
  Notify/Any/AnyEvent.cpp \
  Notify/Any/CosEC_ProxyPushConsumer.cpp \
  Notify/Any/CosEC_ProxyPushSupplier.cpp \
  Notify/Any/ProxyPushConsumer.cpp \
  Notify/Any/ProxyPushSupplier.cpp \
  Notify/Any/PushConsumer.cpp \
  Notify/Any/PushSupplier.cpp \
  Notify/Bit_Vector.cpp \
  Notify/Buffering_Strategy.cpp \
  Notify/Builder.cpp \
  Notify/Consumer.cpp \
  Notify/ConsumerAdmin.cpp \
  Notify/CosNotify_Initializer.cpp \
  Notify/CosNotify_Service.cpp \
  Notify/Default_Factory.cpp \
  Notify/Delivery_Request.cpp \
  Notify/ETCL_Filter.cpp \
  Notify/ETCL_FilterFactory.cpp \
  Notify/Event.cpp \
  Notify/EventChannel.cpp \
  Notify/EventChannelFactory.cpp \
  Notify/EventType.cpp \
  Notify/EventTypeSeq.cpp \
  Notify/Event_Manager.cpp \
  Notify/FilterAdmin.cpp \
  Notify/ID_Factory.cpp \
  Notify/Method_Request.cpp \
  Notify/Method_Request_Dispatch.cpp \
  Notify/Method_Request_Event.cpp \
  Notify/Method_Request_Lookup.cpp \
  Notify/Method_Request_Shutdown.cpp \
  Notify/Method_Request_Updates.cpp \
  Notify/Name_Value_Pair.cpp \
  Notify/Notify_Constraint_Interpreter.cpp \
  Notify/Notify_Constraint_Visitors.cpp \
  Notify/Notify_Default_CO_Factory.cpp \
  Notify/Notify_Default_Collection_Factory.cpp \
  Notify/Notify_Default_EMO_Factory.cpp \
  Notify/Notify_Default_POA_Factory.cpp \
  Notify/Notify_EventChannelFactory_i.cpp \
  Notify/Object.cpp \
  Notify/POA_Helper.cpp \
  Notify/Peer.cpp \
  Notify/Persistent_File_Allocator.cpp \
  Notify/Properties.cpp \
  Notify/PropertySeq.cpp \
  Notify/Property_Boolean.cpp \
  Notify/Proxy.cpp \
  Notify/ProxyConsumer.cpp \
  Notify/ProxySupplier.cpp \
  Notify/QoSProperties.cpp \
  Notify/Random_File.cpp \
  Notify/Reactive_Task.cpp \
  Notify/Reconnection_Registry.cpp \
  Notify/Refcountable.cpp \
  Notify/Routing_Slip.cpp \
  Notify/Routing_Slip_Persistence_Manager.cpp \
  Notify/Routing_Slip_Queue.cpp \
  Notify/Sequence/Batch_Buffering_Strategy.cpp \
  Notify/Sequence/SequenceProxyPushConsumer.cpp \
  Notify/Sequence/SequenceProxyPushSupplier.cpp \
  Notify/Sequence/SequencePushConsumer.cpp \
  Notify/Sequence/SequencePushSupplier.cpp \
  Notify/Standard_Event_Persistence.cpp \
  Notify/Structured/StructuredEvent.cpp \
  Notify/Structured/StructuredProxyPushConsumer.cpp \
  Notify/Structured/StructuredProxyPushSupplier.cpp \
  Notify/Structured/StructuredPushConsumer.cpp \
  Notify/Structured/StructuredPushSupplier.cpp \
  Notify/Subscription_Change_Worker.cpp \
  Notify/Supplier.cpp \
  Notify/SupplierAdmin.cpp \
  Notify/ThreadPool_Task.cpp \
  Notify/Timer_Queue.cpp \
  Notify/Timer_Reactor.cpp \
  Notify/Topology_Loader.cpp \
  Notify/Topology_Object.cpp \
  Notify/Topology_Saver.cpp \
  Notify/Worker_Task.cpp

lib_LTLIBRARIES += libTAO_CosNotification_Serv.la

libTAO_CosNotification_Serv_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_HAS_TYPED_EVENT_CHANNEL \
  -DTAO_NOTIFY_SERV_BUILD_DLL

libTAO_CosNotification_Serv_la_SOURCES = \
  $(Notify)

libTAO_CosNotification_Serv_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_CosNotification_Serv_la_LIBADD = \
  libTAO_ETCL.la \
  $(TAO_BUILDDIR)/tao/libTAO_DynamicAny.la \
  libTAO_CosNotification_Skel.la \
  libTAO_CosEvent_Skel.la \
  libTAO_CosNotification.la \
  libTAO_CosEvent.la \
  libTAO_Svc_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  Notify/Admin.h \
  Notify/AdminProperties.h \
  Notify/Any/AnyEvent.h \
  Notify/Any/CosEC_ProxyPushConsumer.h \
  Notify/Any/CosEC_ProxyPushSupplier.h \
  Notify/Any/ProxyPushConsumer.h \
  Notify/Any/ProxyPushSupplier.h \
  Notify/Any/PushConsumer.h \
  Notify/Any/PushSupplier.h \
  Notify/Bit_Vector.h \
  Notify/Buffering_Strategy.h \
  Notify/Builder.h \
  Notify/Consumer.h \
  Notify/ConsumerAdmin.h \
  Notify/CosNotify_Initializer.h \
  Notify/CosNotify_Service.h \
  Notify/Default_Factory.h \
  Notify/Delivery_Request.h \
  Notify/ETCL_Filter.h \
  Notify/ETCL_FilterFactory.h \
  Notify/Event.h \
  Notify/EventChannel.h \
  Notify/EventChannelFactory.h \
  Notify/EventType.h \
  Notify/EventTypeSeq.h \
  Notify/Event_Manager.h \
  Notify/FilterAdmin.h \
  Notify/ID_Factory.h \
  Notify/Method_Request.h \
  Notify/Method_Request_Dispatch.h \
  Notify/Method_Request_Event.h \
  Notify/Method_Request_Lookup.h \
  Notify/Method_Request_Shutdown.h \
  Notify/Method_Request_Updates.h \
  Notify/Method_Request_Updates_T.h \
  Notify/Name_Value_Pair.h \
  Notify/Notify_Constraint_Interpreter.h \
  Notify/Notify_Constraint_Visitors.h \
  Notify/Notify_Default_CO_Factory.h \
  Notify/Notify_Default_Collection_Factory.h \
  Notify/Notify_Default_EMO_Factory.h \
  Notify/Notify_Default_POA_Factory.h \
  Notify/Notify_EventChannelFactory_i.h \
  Notify/Object.h \
  Notify/POA_Helper.h \
  Notify/Peer.h \
  Notify/Persistent_File_Allocator.h \
  Notify/Properties.h \
  Notify/PropertySeq.h \
  Notify/Property_Boolean.h \
  Notify/Proxy.h \
  Notify/ProxyConsumer.h \
  Notify/ProxyConsumer_T.h \
  Notify/ProxySupplier.h \
  Notify/ProxySupplier_T.h \
  Notify/Proxy_T.h \
  Notify/QoSProperties.h \
  Notify/Random_File.h \
  Notify/Reactive_Task.h \
  Notify/Reconnection_Registry.h \
  Notify/Refcountable.h \
  Notify/Routing_Slip.h \
  Notify/Routing_Slip_Persistence_Manager.h \
  Notify/Routing_Slip_Queue.h \
  Notify/Sequence/Batch_Buffering_Strategy.h \
  Notify/Sequence/SequenceProxyPushConsumer.h \
  Notify/Sequence/SequenceProxyPushSupplier.h \
  Notify/Sequence/SequencePushConsumer.h \
  Notify/Sequence/SequencePushSupplier.h \
  Notify/Standard_Event_Persistence.h \
  Notify/Structured/StructuredEvent.h \
  Notify/Structured/StructuredProxyPushConsumer.h \
  Notify/Structured/StructuredProxyPushSupplier.h \
  Notify/Structured/StructuredPushConsumer.h \
  Notify/Structured/StructuredPushSupplier.h \
  Notify/Subscription_Change_Worker.h \
  Notify/Supplier.h \
  Notify/SupplierAdmin.h \
  Notify/ThreadPool_Task.h \
  Notify/Timer_Queue.h \
  Notify/Timer_Reactor.h \
  Notify/Topology_Loader.h \
  Notify/Topology_Object.h \
  Notify/Topology_Saver.h \
  Notify/Worker_Task.h \
  Notify/Admin.inl \
  Notify/AdminProperties.inl \
  Notify/Any/AnyEvent.inl \
  Notify/Any/CosEC_ProxyPushConsumer.inl \
  Notify/Any/CosEC_ProxyPushSupplier.inl \
  Notify/Any/ProxyPushConsumer.inl \
  Notify/Any/ProxyPushSupplier.inl \
  Notify/Any/PushConsumer.inl \
  Notify/Any/PushSupplier.inl \
  Notify/Buffering_Strategy.inl \
  Notify/Builder.inl \
  Notify/Consumer.inl \
  Notify/ConsumerAdmin.inl \
  Notify/CosNotify_Service.inl \
  Notify/Default_Factory.inl \
  Notify/Delivery_Request.inl \
  Notify/ETCL_Filter.inl \
  Notify/ETCL_FilterFactory.inl \
  Notify/Event.inl \
  Notify/EventChannel.inl \
  Notify/EventChannelFactory.inl \
  Notify/EventType.inl \
  Notify/EventTypeSeq.inl \
  Notify/Event_Manager.inl \
  Notify/FilterAdmin.inl \
  Notify/ID_Factory.inl \
  Notify/Method_Request.inl \
  Notify/Method_Request_Dispatch.inl \
  Notify/Method_Request_Event.inl \
  Notify/Method_Request_Lookup.inl \
  Notify/Method_Request_Shutdown.inl \
  Notify/Method_Request_Updates.inl \
  Notify/Method_Request_Updates_T.inl \
  Notify/Name_Value_Pair.inl \
  Notify/Object.inl \
  Notify/POA_Helper.inl \
  Notify/Peer.inl \
  Notify/Properties.inl \
  Notify/PropertySeq.inl \
  Notify/Property_Boolean.inl \
  Notify/Proxy.inl \
  Notify/ProxyConsumer.inl \
  Notify/ProxyConsumer_T.inl \
  Notify/ProxySupplier.inl \
  Notify/ProxySupplier_T.inl \
  Notify/Proxy_T.inl \
  Notify/QoSProperties.inl \
  Notify/Reactive_Task.inl \
  Notify/Reconnection_Registry.inl \
  Notify/Refcountable.inl \
  Notify/Routing_Slip.inl \
  Notify/Routing_Slip_Queue.inl \
  Notify/Sequence/Batch_Buffering_Strategy.inl \
  Notify/Sequence/SequenceProxyPushConsumer.inl \
  Notify/Sequence/SequenceProxyPushSupplier.inl \
  Notify/Sequence/SequencePushConsumer.inl \
  Notify/Sequence/SequencePushSupplier.inl \
  Notify/Structured/StructuredEvent.inl \
  Notify/Structured/StructuredProxyPushConsumer.inl \
  Notify/Structured/StructuredProxyPushSupplier.inl \
  Notify/Structured/StructuredPushConsumer.inl \
  Notify/Structured/StructuredPushSupplier.inl \
  Notify/Subscription_Change_Worker.inl \
  Notify/Supplier.inl \
  Notify/SupplierAdmin.inl \
  Notify/ThreadPool_Task.inl \
  Notify/Timer_Queue.inl \
  Notify/Timer_Reactor.inl \
  Notify/Topology_Object.inl \
  Notify/Worker_Task.inl \
  Notify/Method_Request_Updates_T.cpp \
  Notify/ProxyConsumer_T.cpp \
  Notify/ProxySupplier_T.cpp \
  Notify/Proxy_T.cpp

endif !BUILD_MINIMUM_CORBA

EXTRA_DIST += \
  CosNotification_Serv.rc

## Makefile.CosNotification_Persist.am 

if BUILD_ACEXML
if !BUILD_MINIMUM_CORBA

noinst_LTLIBRARIES += libTAO_CosNotification_Persist.la

libTAO_CosNotification_Persist_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -I$(ACE_ROOT)/ACEXML/common \
  -DTAO_HAS_TYPED_EVENT_CHANNEL \
  -DTAO_NOTIFY_PERSIST_BUILD_DLL

libTAO_CosNotification_Persist_la_SOURCES = \
  Notify/XML_Loader.cpp \
  Notify/XML_Saver.cpp \
  Notify/XML_Topology_Factory.cpp

libTAO_CosNotification_Persist_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_CosNotification_Persist_la_LIBADD = \
  $(ACE_BUILDDIR)/ACEXML/parser/parser/libACEXML_Parser.la \
  $(ACE_BUILDDIR)/ACEXML/common/libACEXML.la \
  libTAO_CosNotification_Serv.la \
  $(TAO_BUILDDIR)/tao/libTAO_DynamicAny.la \
  libTAO_ETCL.la \
  libTAO_CosNotification_Skel.la \
  libTAO_CosEvent_Skel.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  libTAO_CosNotification.la \
  libTAO_CosEvent.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

noinst_HEADERS += \
  Notify/XML_Loader.h \
  Notify/XML_Saver.h \
  Notify/XML_Topology_Factory.h \
  Channel_Clients_T.i \
  Event_Utilities.i \
  Runtime_Scheduler.i \
  Scheduler_Factory.i \
  Scheduler_Utilities.i \
  Time_Utilities.i

endif !BUILD_MINIMUM_CORBA
endif BUILD_ACEXML

## Makefile.CosTime.am 

BUILT_SOURCES += \
  TimeServiceS_T.cpp \
  TimeServiceC.inl \
  TimeServiceS.inl \
  TimeServiceS_T.inl \
  TimeServiceC.cpp \
  TimeServiceS.cpp \
  TimeServiceC.h \
  TimeServiceS.h \
  TimeServiceS_T.h

CLEANFILES += \
  TimeServiceS_T.cpp \
  TimeServiceC.inl \
  TimeServiceS.inl \
  TimeServiceS_T.inl \
  TimeServiceC.cpp \
  TimeServiceS.cpp \
  TimeServiceC.h \
  TimeServiceS.h \
  TimeServiceS_T.h \
  TimeService-stamp

TimeServiceS_T.cpp TimeServiceC.inl TimeServiceS.inl TimeServiceS_T.inl TimeServiceC.cpp TimeServiceS.cpp TimeServiceC.h TimeServiceS.h TimeServiceS_T.h: TimeService-stamp

TimeService-stamp: $(srcdir)/TimeService.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_Time_Export -Wb,export_include=orbsvcs/Time/time_export.h $(srcdir)/TimeService.idl
	@touch $@

Time = \
  Time/TAO_TIO.cpp \
  Time/TAO_Time_Service_Clerk.cpp \
  Time/TAO_Time_Service_Server.cpp \
  Time/TAO_UTO.cpp \
  Time/Timer_Helper.cpp \
  TimeServiceC.cpp \
  TimeServiceS.cpp

lib_LTLIBRARIES += libTAO_CosTime.la

libTAO_CosTime_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_TIME_BUILD_DLL

libTAO_CosTime_la_SOURCES = \
  $(Time)

libTAO_CosTime_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_CosTime_la_LIBADD = \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  Time/TAO_TIO.h \
  Time/TAO_Time_Service_Clerk.h \
  Time/TAO_Time_Service_Server.h \
  Time/TAO_UTO.h \
  Time/Timer_Helper.h \
  TimeServiceC.h \
  TimeServiceS.h \
  TimeServiceS_T.h \
  TimeServiceC.inl \
  TimeServiceS.inl \
  TimeServiceS_T.inl \
  TimeServiceS_T.cpp \
  TimeService.idl

EXTRA_DIST += \
  CosTime.rc

## Makefile.CosTrading.am 

BUILT_SOURCES += \
  CosTradingS_T.cpp \
  CosTradingC.inl \
  CosTradingC.cpp \
  CosTradingC.h

CLEANFILES += \
  CosTradingS_T.cpp \
  CosTradingC.inl \
  CosTradingC.cpp \
  CosTradingC.h \
  CosTrading-stamp

CosTradingS_T.cpp CosTradingC.inl CosTradingC.cpp CosTradingC.h: CosTrading-stamp

CosTrading-stamp: $(srcdir)/CosTrading.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,stub_export_macro=TAO_Trading_Export -Wb,stub_export_include=orbsvcs/Trader/trading_export.h -Wb,skel_export_macro=TAO_Trading_Skel_Export -Wb,skel_export_include=orbsvcs/Trader/trading_skel_export.h $(srcdir)/CosTrading.idl
	@touch $@

BUILT_SOURCES += \
  CosTradingDynamicS_T.cpp \
  CosTradingDynamicC.inl \
  CosTradingDynamicC.cpp \
  CosTradingDynamicC.h

CLEANFILES += \
  CosTradingDynamicS_T.cpp \
  CosTradingDynamicC.inl \
  CosTradingDynamicC.cpp \
  CosTradingDynamicC.h \
  CosTradingDynamic-stamp

CosTradingDynamicS_T.cpp CosTradingDynamicC.inl CosTradingDynamicC.cpp CosTradingDynamicC.h: CosTradingDynamic-stamp

CosTradingDynamic-stamp: $(srcdir)/CosTradingDynamic.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,stub_export_macro=TAO_Trading_Export -Wb,stub_export_include=orbsvcs/Trader/trading_export.h -Wb,skel_export_macro=TAO_Trading_Skel_Export -Wb,skel_export_include=orbsvcs/Trader/trading_skel_export.h $(srcdir)/CosTradingDynamic.idl
	@touch $@

BUILT_SOURCES += \
  CosTradingReposS_T.cpp \
  CosTradingReposC.inl \
  CosTradingReposC.cpp \
  CosTradingReposC.h

CLEANFILES += \
  CosTradingReposS_T.cpp \
  CosTradingReposC.inl \
  CosTradingReposC.cpp \
  CosTradingReposC.h \
  CosTradingRepos-stamp

CosTradingReposS_T.cpp CosTradingReposC.inl CosTradingReposC.cpp CosTradingReposC.h: CosTradingRepos-stamp

CosTradingRepos-stamp: $(srcdir)/CosTradingRepos.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,stub_export_macro=TAO_Trading_Export -Wb,stub_export_include=orbsvcs/Trader/trading_export.h -Wb,skel_export_macro=TAO_Trading_Skel_Export -Wb,skel_export_include=orbsvcs/Trader/trading_skel_export.h $(srcdir)/CosTradingRepos.idl
	@touch $@

lib_LTLIBRARIES += libTAO_CosTrading.la

libTAO_CosTrading_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_TRADING_BUILD_DLL

libTAO_CosTrading_la_SOURCES = \
  CosTradingC.cpp \
  CosTradingDynamicC.cpp \
  CosTradingReposC.cpp

libTAO_CosTrading_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_CosTrading_la_LIBADD = \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  CosTradingC.h \
  CosTradingDynamicC.h \
  CosTradingReposC.h \
  Trader/trading_export.h \
  CosTradingC.inl \
  CosTradingDynamicC.inl \
  CosTradingReposC.inl \
  CosTrading.idl \
  CosTradingDynamic.idl \
  CosTradingRepos.idl

EXTRA_DIST += \
  CosTrading.rc

## Makefile.CosTrading_Skel.am 

lib_LTLIBRARIES += libTAO_CosTrading_Skel.la

libTAO_CosTrading_Skel_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_TRADING_SKEL_BUILD_DLL

libTAO_CosTrading_Skel_la_SOURCES = \
  CosTradingDynamicS.cpp \
  CosTradingReposS.cpp \
  CosTradingS.cpp

libTAO_CosTrading_Skel_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_CosTrading_Skel_la_LIBADD = \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  libTAO_CosTrading.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  CosTradingDynamicS.h \
  CosTradingDynamicS_T.h \
  CosTradingReposS.h \
  CosTradingReposS_T.h \
  CosTradingS.h \
  CosTradingS_T.h \
  Trader/trading_skel_export.h \
  CosTradingDynamicS.inl \
  CosTradingDynamicS_T.inl \
  CosTradingReposS.inl \
  CosTradingReposS_T.inl \
  CosTradingS.inl \
  CosTradingS_T.inl \
  CosTradingDynamicS_T.cpp \
  CosTradingReposS_T.cpp \
  CosTradingS_T.cpp

EXTRA_DIST += \
  CosTrading_Skel.rc

## Makefile.CosTrading_Serv.am 

Trader = \
  Trader/Constraint_Interpreter.cpp \
  Trader/Constraint_Nodes.cpp \
  Trader/Constraint_Visitors.cpp \
  Trader/Constraint_l.cpp \
  Trader/Constraint_y.cpp \
  Trader/Interpreter.cpp \
  Trader/Interpreter_Utils.cpp \
  Trader/Offer_Database.cpp \
  Trader/Offer_Iterators.cpp \
  Trader/Service_Type_Repository.cpp \
  Trader/Trader.cpp \
  Trader/Trader_Constraint_Visitors.cpp \
  Trader/Trader_Interfaces.cpp \
  Trader/Trader_Utils.cpp \
  Trader/Trading_Loader.cpp

lib_LTLIBRARIES += libTAO_CosTrading_Serv.la

libTAO_CosTrading_Serv_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_TRADING_SERV_BUILD_DLL

libTAO_CosTrading_Serv_la_SOURCES = \
  $(Trader)

libTAO_CosTrading_Serv_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_CosTrading_Serv_la_LIBADD = \
  libTAO_Svc_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_DynamicAny.la \
  libTAO_CosTrading_Skel.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  libTAO_CosTrading.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  Trader/Constraint_Interpreter.h \
  Trader/Constraint_Nodes.h \
  Trader/Constraint_Visitors.h \
  Trader/Interpreter.h \
  Trader/Interpreter_Utils.h \
  Trader/Interpreter_Utils_T.h \
  Trader/Offer_Database.h \
  Trader/Offer_Iterators.h \
  Trader/Offer_Iterators_T.h \
  Trader/Service_Type_Repository.h \
  Trader/Trader.h \
  Trader/Trader_Constraint_Visitors.h \
  Trader/Trader_Interfaces.h \
  Trader/Trader_T.h \
  Trader/Trader_Utils.h \
  Trader/Trading_Loader.h \
  Trader/Interpreter_Utils_T.cpp \
  Trader/Offer_Iterators_T.cpp \
  Trader/Trader_T.cpp

EXTRA_DIST += \
  CosTrading_Serv.rc

## Makefile.DsLogAdmin.am 

BUILT_SOURCES += \
  DsLogAdminS_T.cpp \
  DsLogAdminC.inl \
  DsLogAdminC.cpp \
  DsLogAdminC.h

CLEANFILES += \
  DsLogAdminS_T.cpp \
  DsLogAdminC.inl \
  DsLogAdminC.cpp \
  DsLogAdminC.h \
  DsLogAdmin-stamp

DsLogAdminS_T.cpp DsLogAdminC.inl DsLogAdminC.cpp DsLogAdminC.h: DsLogAdmin-stamp

DsLogAdmin-stamp: $(srcdir)/DsLogAdmin.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,stub_export_macro=TAO_Log_Export -Wb,stub_export_include=orbsvcs/Log/log_export.h -Wb,skel_export_macro=TAO_Log_Skel_Export -Wb,skel_export_include=orbsvcs/Log/log_skel_export.h $(srcdir)/DsLogAdmin.idl
	@touch $@

BUILT_SOURCES += \
  DsLogNotificationS_T.cpp \
  DsLogNotificationC.inl \
  DsLogNotificationC.cpp \
  DsLogNotificationC.h

CLEANFILES += \
  DsLogNotificationS_T.cpp \
  DsLogNotificationC.inl \
  DsLogNotificationC.cpp \
  DsLogNotificationC.h \
  DsLogNotification-stamp

DsLogNotificationS_T.cpp DsLogNotificationC.inl DsLogNotificationC.cpp DsLogNotificationC.h: DsLogNotification-stamp

DsLogNotification-stamp: $(srcdir)/DsLogNotification.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,stub_export_macro=TAO_Log_Export -Wb,stub_export_include=orbsvcs/Log/log_export.h -Wb,skel_export_macro=TAO_Log_Skel_Export -Wb,skel_export_include=orbsvcs/Log/log_skel_export.h $(srcdir)/DsLogNotification.idl
	@touch $@

lib_LTLIBRARIES += libTAO_DsLogAdmin.la

libTAO_DsLogAdmin_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_LOG_BUILD_DLL

libTAO_DsLogAdmin_la_SOURCES = \
  DsLogAdminC.cpp \
  DsLogNotificationC.cpp

libTAO_DsLogAdmin_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_DsLogAdmin_la_LIBADD = \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  DsLogAdminC.h \
  DsLogNotificationC.h \
  Log/log_export.h \
  DsLogAdminC.inl \
  DsLogNotificationC.inl \
  DsLogAdmin.idl \
  DsLogNotification.idl

EXTRA_DIST += \
  DsLogAdmin.rc

## Makefile.DsEventLogAdmin.am 

if !BUILD_MINIMUM_CORBA

BUILT_SOURCES += \
  DsEventLogAdminS_T.cpp \
  DsEventLogAdminC.inl \
  DsEventLogAdminC.cpp \
  DsEventLogAdminC.h

CLEANFILES += \
  DsEventLogAdminS_T.cpp \
  DsEventLogAdminC.inl \
  DsEventLogAdminC.cpp \
  DsEventLogAdminC.h \
  DsEventLogAdmin-stamp

DsEventLogAdminS_T.cpp DsEventLogAdminC.inl DsEventLogAdminC.cpp DsEventLogAdminC.h: DsEventLogAdmin-stamp

DsEventLogAdmin-stamp: $(srcdir)/DsEventLogAdmin.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,stub_export_macro=TAO_EventLog_Export -Wb,stub_export_include=orbsvcs/Log/eventlog_export.h -Wb,skel_export_macro=TAO_EventLog_Skel_Export -Wb,skel_export_include=orbsvcs/Log/eventlog_skel_export.h $(srcdir)/DsEventLogAdmin.idl
	@touch $@

lib_LTLIBRARIES += libTAO_DsEventLogAdmin.la

libTAO_DsEventLogAdmin_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_HAS_TYPED_EVENT_CHANNEL \
  -DTAO_EVENTLOG_BUILD_DLL

libTAO_DsEventLogAdmin_la_SOURCES = \
  DsEventLogAdminC.cpp

libTAO_DsEventLogAdmin_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_DsEventLogAdmin_la_LIBADD = \
  libTAO_CosEvent.la \
  libTAO_DsLogAdmin.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  DsEventLogAdminC.h \
  Log/eventlog_export.h \
  DsEventLogAdminC.inl \
  DsEventLogAdmin.idl

endif !BUILD_MINIMUM_CORBA

EXTRA_DIST += \
  DsEventLogAdmin.rc

## Makefile.DsLogAdmin_Skel.am 

lib_LTLIBRARIES += libTAO_DsLogAdmin_Skel.la

libTAO_DsLogAdmin_Skel_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_LOG_SKEL_BUILD_DLL

libTAO_DsLogAdmin_Skel_la_SOURCES = \
  DsLogAdminS.cpp \
  DsLogNotificationS.cpp

libTAO_DsLogAdmin_Skel_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_DsLogAdmin_Skel_la_LIBADD = \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  libTAO_DsLogAdmin.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  DsLogAdminS.h \
  DsLogAdminS_T.h \
  DsLogNotificationS.h \
  DsLogNotificationS_T.h \
  Log/log_skel_export.h \
  DsLogAdminS.inl \
  DsLogAdminS_T.inl \
  DsLogNotificationS.inl \
  DsLogNotificationS_T.inl \
  DsLogAdminS_T.cpp \
  DsLogNotificationS_T.cpp

## Makefile.DsLogAdmin_Serv.am 

DsLogAdmin = \
  Log/BasicLogFactory_i.cpp \
  Log/BasicLog_i.cpp \
  Log/Iterator_i.cpp \
  Log/LogMgr_i.cpp \
  Log/LogNotification.cpp \
  Log/LogRecordStore.cpp \
  Log/Log_Constraint_Interpreter.cpp \
  Log/Log_Constraint_Visitors.cpp \
  Log/Log_i.cpp

lib_LTLIBRARIES += libTAO_DsLogAdmin_Serv.la

libTAO_DsLogAdmin_Serv_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_LOG_SERV_BUILD_DLL

libTAO_DsLogAdmin_Serv_la_SOURCES = \
  $(DsLogAdmin)

libTAO_DsLogAdmin_Serv_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_DsLogAdmin_Serv_la_LIBADD = \
  libTAO_Svc_Utils.la \
  libTAO_ETCL.la \
  $(TAO_BUILDDIR)/tao/libTAO_DynamicAny.la \
  libTAO_DsLogAdmin_Skel.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  libTAO_DsLogAdmin.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  Log/BasicLogFactory_i.h \
  Log/BasicLog_i.h \
  Log/Iterator_i.h \
  Log/LogMgr_i.h \
  Log/LogNotification.h \
  Log/LogRecordStore.h \
  Log/Log_Constraint_Interpreter.h \
  Log/Log_Constraint_Visitors.h \
  Log/Log_i.h

## Makefile.DsEventLogAdmin_Skel.am 

if !BUILD_MINIMUM_CORBA

lib_LTLIBRARIES += libTAO_DsEventLogAdmin_Skel.la

libTAO_DsEventLogAdmin_Skel_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_HAS_TYPED_EVENT_CHANNEL \
  -DTAO_EVENTLOG_SKEL_BUILD_DLL

libTAO_DsEventLogAdmin_Skel_la_SOURCES = \
  DsEventLogAdminS.cpp

libTAO_DsEventLogAdmin_Skel_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_DsEventLogAdmin_Skel_la_LIBADD = \
  libTAO_CosEvent_Skel.la \
  libTAO_DsLogAdmin_Skel.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  libTAO_DsEventLogAdmin.la \
  libTAO_CosEvent.la \
  libTAO_DsLogAdmin.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  DsEventLogAdminS.h \
  DsEventLogAdminS_T.h \
  Log/eventlog_skel_export.h \
  DsEventLogAdminS.inl \
  DsEventLogAdminS_T.inl \
  DsEventLogAdminS_T.cpp

endif !BUILD_MINIMUM_CORBA

## Makefile.DsEventLogAdmin_Serv.am 

if BUILD_CORBA_MESSAGING
if !BUILD_MINIMUM_CORBA

DsEventLogAdmin = \
  Log/EventLogConsumer.cpp \
  Log/EventLogFactory_i.cpp \
  Log/EventLogNotification.cpp \
  Log/EventLog_i.cpp

lib_LTLIBRARIES += libTAO_DsEventLogAdmin_Serv.la

libTAO_DsEventLogAdmin_Serv_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_HAS_TYPED_EVENT_CHANNEL \
  -DTAO_EVENTLOG_SERV_BUILD_DLL

libTAO_DsEventLogAdmin_Serv_la_SOURCES = \
  $(DsEventLogAdmin)

libTAO_DsEventLogAdmin_Serv_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_DsEventLogAdmin_Serv_la_LIBADD = \
  libTAO_CosEvent_Serv.la \
  $(TAO_BUILDDIR)/tao/libTAO_IFR_Client.la \
  $(TAO_BUILDDIR)/tao/libTAO_DynamicInterface.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  libTAO_DsLogAdmin_Serv.la \
  libTAO_Svc_Utils.la \
  libTAO_ETCL.la \
  $(TAO_BUILDDIR)/tao/libTAO_DynamicAny.la \
  libTAO_DsEventLogAdmin_Skel.la \
  libTAO_CosEvent_Skel.la \
  libTAO_DsLogAdmin_Skel.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  libTAO_DsEventLogAdmin.la \
  libTAO_CosEvent.la \
  libTAO_DsLogAdmin.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  Log/EventLogConsumer.h \
  Log/EventLogFactory_i.h \
  Log/EventLogNotification.h \
  Log/EventLog_i.h

endif !BUILD_MINIMUM_CORBA
endif BUILD_CORBA_MESSAGING

## Makefile.DsNotifyLogAdmin.am 

if !BUILD_MINIMUM_CORBA

BUILT_SOURCES += \
  DsNotifyLogAdminS_T.cpp \
  DsNotifyLogAdminC.inl \
  DsNotifyLogAdminC.cpp \
  DsNotifyLogAdminC.h

CLEANFILES += \
  DsNotifyLogAdminS_T.cpp \
  DsNotifyLogAdminC.inl \
  DsNotifyLogAdminC.cpp \
  DsNotifyLogAdminC.h \
  DsNotifyLogAdmin-stamp

DsNotifyLogAdminS_T.cpp DsNotifyLogAdminC.inl DsNotifyLogAdminC.cpp DsNotifyLogAdminC.h: DsNotifyLogAdmin-stamp

DsNotifyLogAdmin-stamp: $(srcdir)/DsNotifyLogAdmin.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,stub_export_macro=TAO_NotifyLog_Export -Wb,stub_export_include=orbsvcs/Log/notifylog_export.h -Wb,skel_export_macro=TAO_NotifyLog_Skel_Export -Wb,skel_export_include=orbsvcs/Log/notifylog_skel_export.h $(srcdir)/DsNotifyLogAdmin.idl
	@touch $@

lib_LTLIBRARIES += libTAO_DsNotifyLogAdmin.la

libTAO_DsNotifyLogAdmin_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_HAS_TYPED_EVENT_CHANNEL \
  -DTAO_NOTIFYLOG_BUILD_DLL

libTAO_DsNotifyLogAdmin_la_SOURCES = \
  DsNotifyLogAdminC.cpp

libTAO_DsNotifyLogAdmin_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_DsNotifyLogAdmin_la_LIBADD = \
  libTAO_CosNotification.la \
  libTAO_DsEventLogAdmin.la \
  libTAO_CosEvent.la \
  libTAO_DsLogAdmin.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  DsNotifyLogAdminC.h \
  Log/notifylog_export.h \
  DsNotifyLogAdminC.inl \
  DsNotifyLogAdmin.idl

endif !BUILD_MINIMUM_CORBA

EXTRA_DIST += \
  DsNotifyLogAdmin.rc

## Makefile.DsNotifyLogAdmin_Skel.am 

if !BUILD_MINIMUM_CORBA

lib_LTLIBRARIES += libTAO_DsNotifyLogAdmin_Skel.la

libTAO_DsNotifyLogAdmin_Skel_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_HAS_TYPED_EVENT_CHANNEL \
  -DTAO_NOTIFYLOG_SKEL_BUILD_DLL

libTAO_DsNotifyLogAdmin_Skel_la_SOURCES = \
  DsNotifyLogAdminS.cpp

libTAO_DsNotifyLogAdmin_Skel_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_DsNotifyLogAdmin_Skel_la_LIBADD = \
  libTAO_CosNotification_Skel.la \
  libTAO_DsEventLogAdmin_Skel.la \
  libTAO_CosEvent_Skel.la \
  libTAO_DsLogAdmin_Skel.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  libTAO_DsNotifyLogAdmin.la \
  libTAO_CosNotification.la \
  libTAO_DsEventLogAdmin.la \
  libTAO_CosEvent.la \
  libTAO_DsLogAdmin.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  DsNotifyLogAdminS.h \
  DsNotifyLogAdminS_T.h \
  Log/notifylog_skel_export.h \
  DsNotifyLogAdminS.inl \
  DsNotifyLogAdminS_T.inl \
  DsNotifyLogAdminS_T.cpp

endif !BUILD_MINIMUM_CORBA

## Makefile.DsNotifyLogAdmin_Serv.am 

if !BUILD_MINIMUM_CORBA

DsNotifyLogAdmin = \
  Log/NotifyLogConsumer.cpp \
  Log/NotifyLogFactory_i.cpp \
  Log/NotifyLogNotification.cpp \
  Log/NotifyLog_i.cpp

lib_LTLIBRARIES += libTAO_DsNotifyLogAdmin_Serv.la

libTAO_DsNotifyLogAdmin_Serv_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_HAS_TYPED_EVENT_CHANNEL \
  -DTAO_NOTIFYLOG_SERV_BUILD_DLL

libTAO_DsNotifyLogAdmin_Serv_la_SOURCES = \
  $(DsNotifyLogAdmin)

libTAO_DsNotifyLogAdmin_Serv_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_DsNotifyLogAdmin_Serv_la_LIBADD = \
  libTAO_DsLogAdmin_Serv.la \
  libTAO_Svc_Utils.la \
  libTAO_ETCL.la \
  $(TAO_BUILDDIR)/tao/libTAO_DynamicAny.la \
  libTAO_DsNotifyLogAdmin_Skel.la \
  libTAO_CosNotification_Skel.la \
  libTAO_DsEventLogAdmin_Skel.la \
  libTAO_CosEvent_Skel.la \
  libTAO_DsLogAdmin_Skel.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  libTAO_DsNotifyLogAdmin.la \
  libTAO_CosNotification.la \
  libTAO_DsEventLogAdmin.la \
  libTAO_CosEvent.la \
  libTAO_DsLogAdmin.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  Log/NotifyLogConsumer.h \
  Log/NotifyLogFactory_i.h \
  Log/NotifyLogNotification.h \
  Log/NotifyLog_i.h

endif !BUILD_MINIMUM_CORBA

## Makefile.FTRT_ClientORB.am 

if BUILD_CORBA_MESSAGING

FTRT_ClientORB = \
  FtRtEvent/ClientORB/FTRT_ClientORB_Initializer.cpp \
  FtRtEvent/ClientORB/FTRT_ClientORB_Interceptor.cpp \
  FtRtEvent/ClientORB/FTRT_ClientORB_Loader.cpp

lib_LTLIBRARIES += libTAO_FTRT_ClientORB.la

libTAO_FTRT_ClientORB_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -I$(TAO_ROOT)/orbsvcs/orbsvcs/FtRtEvent/ClientORB \
  -DTAO_FTRT_BUILD_DLL

libTAO_FTRT_ClientORB_la_SOURCES = \
  $(FTRT_ClientORB)

libTAO_FTRT_ClientORB_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_FTRT_ClientORB_la_LIBADD = \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  FtRtEvent/ClientORB/FTRT_ClientORB_Initializer.h \
  FtRtEvent/ClientORB/FTRT_ClientORB_Interceptor.h \
  FtRtEvent/ClientORB/FTRT_ClientORB_Loader.h

endif BUILD_CORBA_MESSAGING

## Makefile.RTEvent.am 

if BUILD_CORBA_MESSAGING

BUILT_SOURCES += \
  RtecDefaultEventDataS_T.cpp \
  RtecDefaultEventDataC.inl \
  RtecDefaultEventDataS.inl \
  RtecDefaultEventDataS_T.inl \
  RtecDefaultEventDataC.cpp \
  RtecDefaultEventDataS.cpp \
  RtecDefaultEventDataC.h \
  RtecDefaultEventDataS.h \
  RtecDefaultEventDataS_T.h

CLEANFILES += \
  RtecDefaultEventDataS_T.cpp \
  RtecDefaultEventDataC.inl \
  RtecDefaultEventDataS.inl \
  RtecDefaultEventDataS_T.inl \
  RtecDefaultEventDataC.cpp \
  RtecDefaultEventDataS.cpp \
  RtecDefaultEventDataC.h \
  RtecDefaultEventDataS.h \
  RtecDefaultEventDataS_T.h \
  RtecDefaultEventData-stamp

RtecDefaultEventDataS_T.cpp RtecDefaultEventDataC.inl RtecDefaultEventDataS.inl RtecDefaultEventDataS_T.inl RtecDefaultEventDataC.cpp RtecDefaultEventDataS.cpp RtecDefaultEventDataC.h RtecDefaultEventDataS.h RtecDefaultEventDataS_T.h: RtecDefaultEventData-stamp

RtecDefaultEventData-stamp: $(srcdir)/RtecDefaultEventData.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_RTEvent_Export -Wb,export_include=orbsvcs/Event/event_export.h $(srcdir)/RtecDefaultEventData.idl
	@touch $@

BUILT_SOURCES += \
  RtecEventChannelAdminS_T.cpp \
  RtecEventChannelAdminC.inl \
  RtecEventChannelAdminS.inl \
  RtecEventChannelAdminS_T.inl \
  RtecEventChannelAdminC.cpp \
  RtecEventChannelAdminS.cpp \
  RtecEventChannelAdminC.h \
  RtecEventChannelAdminS.h \
  RtecEventChannelAdminS_T.h

CLEANFILES += \
  RtecEventChannelAdminS_T.cpp \
  RtecEventChannelAdminC.inl \
  RtecEventChannelAdminS.inl \
  RtecEventChannelAdminS_T.inl \
  RtecEventChannelAdminC.cpp \
  RtecEventChannelAdminS.cpp \
  RtecEventChannelAdminC.h \
  RtecEventChannelAdminS.h \
  RtecEventChannelAdminS_T.h \
  RtecEventChannelAdmin-stamp

RtecEventChannelAdminS_T.cpp RtecEventChannelAdminC.inl RtecEventChannelAdminS.inl RtecEventChannelAdminS_T.inl RtecEventChannelAdminC.cpp RtecEventChannelAdminS.cpp RtecEventChannelAdminC.h RtecEventChannelAdminS.h RtecEventChannelAdminS_T.h: RtecEventChannelAdmin-stamp

RtecEventChannelAdmin-stamp: $(srcdir)/RtecEventChannelAdmin.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_RTEvent_Export -Wb,export_include=orbsvcs/Event/event_export.h $(srcdir)/RtecEventChannelAdmin.idl
	@touch $@

BUILT_SOURCES += \
  RtecEventCommS_T.cpp \
  RtecEventCommC.inl \
  RtecEventCommS.inl \
  RtecEventCommS_T.inl \
  RtecEventCommC.cpp \
  RtecEventCommS.cpp \
  RtecEventCommC.h \
  RtecEventCommS.h \
  RtecEventCommS_T.h

CLEANFILES += \
  RtecEventCommS_T.cpp \
  RtecEventCommC.inl \
  RtecEventCommS.inl \
  RtecEventCommS_T.inl \
  RtecEventCommC.cpp \
  RtecEventCommS.cpp \
  RtecEventCommC.h \
  RtecEventCommS.h \
  RtecEventCommS_T.h \
  RtecEventComm-stamp

RtecEventCommS_T.cpp RtecEventCommC.inl RtecEventCommS.inl RtecEventCommS_T.inl RtecEventCommC.cpp RtecEventCommS.cpp RtecEventCommC.h RtecEventCommS.h RtecEventCommS_T.h: RtecEventComm-stamp

RtecEventComm-stamp: $(srcdir)/RtecEventComm.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_RTEvent_Export -Wb,export_include=orbsvcs/Event/event_export.h $(srcdir)/RtecEventComm.idl
	@touch $@

BUILT_SOURCES += \
  RtecUDPAdminS_T.cpp \
  RtecUDPAdminC.inl \
  RtecUDPAdminS.inl \
  RtecUDPAdminS_T.inl \
  RtecUDPAdminC.cpp \
  RtecUDPAdminS.cpp \
  RtecUDPAdminC.h \
  RtecUDPAdminS.h \
  RtecUDPAdminS_T.h

CLEANFILES += \
  RtecUDPAdminS_T.cpp \
  RtecUDPAdminC.inl \
  RtecUDPAdminS.inl \
  RtecUDPAdminS_T.inl \
  RtecUDPAdminC.cpp \
  RtecUDPAdminS.cpp \
  RtecUDPAdminC.h \
  RtecUDPAdminS.h \
  RtecUDPAdminS_T.h \
  RtecUDPAdmin-stamp

RtecUDPAdminS_T.cpp RtecUDPAdminC.inl RtecUDPAdminS.inl RtecUDPAdminS_T.inl RtecUDPAdminC.cpp RtecUDPAdminS.cpp RtecUDPAdminC.h RtecUDPAdminS.h RtecUDPAdminS_T.h: RtecUDPAdmin-stamp

RtecUDPAdmin-stamp: $(srcdir)/RtecUDPAdmin.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_RTEvent_Export -Wb,export_include=orbsvcs/Event/event_export.h $(srcdir)/RtecUDPAdmin.idl
	@touch $@

RTEvent = \
  Event/ECG_Adapters.cpp \
  Event/ECG_Address_Server_Base.cpp \
  Event/ECG_CDR_Message_Receiver.cpp \
  Event/ECG_CDR_Message_Sender.cpp \
  Event/ECG_Complex_Address_Server.cpp \
  Event/ECG_ConsumerEC_Control.cpp \
  Event/ECG_Mcast_EH.cpp \
  Event/ECG_Mcast_Gateway.cpp \
  Event/ECG_Reactive_ConsumerEC_Control.cpp \
  Event/ECG_Reconnect_ConsumerEC_Control.cpp \
  Event/ECG_Simple_Address_Server.cpp \
  Event/ECG_Simple_Mcast_EH.cpp \
  Event/ECG_UDP_EH.cpp \
  Event/ECG_UDP_Out_Endpoint.cpp \
  Event/ECG_UDP_Receiver.cpp \
  Event/ECG_UDP_Sender.cpp \
  Event/EC_And_Filter.cpp \
  Event/EC_Basic_Factory.cpp \
  Event/EC_Basic_Filter_Builder.cpp \
  Event/EC_Bitmask_Filter.cpp \
  Event/EC_Busy_Lock.cpp \
  Event/EC_Channel_Destroyer.cpp \
  Event/EC_Conjunction_Filter.cpp \
  Event/EC_ConsumerAdmin.cpp \
  Event/EC_ConsumerControl.cpp \
  Event/EC_Default_Factory.cpp \
  Event/EC_Default_ProxyConsumer.cpp \
  Event/EC_Default_ProxySupplier.cpp \
  Event/EC_Disjunction_Filter.cpp \
  Event/EC_Dispatching.cpp \
  Event/EC_Dispatching_Task.cpp \
  Event/EC_Event_Channel.cpp \
  Event/EC_Event_Channel_Base.cpp \
  Event/EC_Factory.cpp \
  Event/EC_Filter.cpp \
  Event/EC_Filter_Builder.cpp \
  Event/EC_Gateway.cpp \
  Event/EC_Gateway_IIOP.cpp \
  Event/EC_Gateway_IIOP_Factory.cpp \
  Event/EC_Group_Scheduling.cpp \
  Event/EC_Lifetime_Utils.cpp \
  Event/EC_Lifetime_Utils_T.cpp \
  Event/EC_MT_Dispatching.cpp \
  Event/EC_Masked_Type_Filter.cpp \
  Event/EC_Negation_Filter.cpp \
  Event/EC_Null_Factory.cpp \
  Event/EC_Null_Scheduling.cpp \
  Event/EC_ObserverStrategy.cpp \
  Event/EC_Per_Supplier_Filter.cpp \
  Event/EC_Prefix_Filter_Builder.cpp \
  Event/EC_ProxyConsumer.cpp \
  Event/EC_ProxySupplier.cpp \
  Event/EC_Proxy_Disconnector.cpp \
  Event/EC_QOS_Info.cpp \
  Event/EC_Reactive_ConsumerControl.cpp \
  Event/EC_Reactive_Dispatching.cpp \
  Event/EC_Reactive_SupplierControl.cpp \
  Event/EC_Reactive_Timeout_Generator.cpp \
  Event/EC_Scheduling_Strategy.cpp \
  Event/EC_SupplierAdmin.cpp \
  Event/EC_SupplierControl.cpp \
  Event/EC_Supplier_Filter.cpp \
  Event/EC_Supplier_Filter_Builder.cpp \
  Event/EC_Timeout_Filter.cpp \
  Event/EC_Timeout_Generator.cpp \
  Event/EC_Trivial_Supplier_Filter.cpp \
  Event/EC_Type_Filter.cpp \
  Event/EC_UDP_Admin.cpp \
  Event_Utilities.cpp \
  RtecDefaultEventDataC.cpp \
  RtecDefaultEventDataS.cpp \
  RtecEventChannelAdminC.cpp \
  RtecEventChannelAdminS.cpp \
  RtecEventCommC.cpp \
  RtecEventCommS.cpp \
  RtecUDPAdminC.cpp \
  RtecUDPAdminS.cpp

lib_LTLIBRARIES += libTAO_RTEvent.la

libTAO_RTEvent_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_RTEVENT_BUILD_DLL

libTAO_RTEvent_la_SOURCES = \
  $(RTEvent)

libTAO_RTEvent_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_RTEvent_la_LIBADD = \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  libTAO_Svc_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  Event/ECG_Adapters.h \
  Event/ECG_Address_Server_Base.h \
  Event/ECG_CDR_Message_Receiver.h \
  Event/ECG_CDR_Message_Sender.h \
  Event/ECG_Complex_Address_Server.h \
  Event/ECG_ConsumerEC_Control.h \
  Event/ECG_Mcast_EH.h \
  Event/ECG_Mcast_Gateway.h \
  Event/ECG_Reactive_ConsumerEC_Control.h \
  Event/ECG_Reconnect_ConsumerEC_Control.h \
  Event/ECG_Simple_Address_Server.h \
  Event/ECG_Simple_Mcast_EH.h \
  Event/ECG_UDP_EH.h \
  Event/ECG_UDP_Out_Endpoint.h \
  Event/ECG_UDP_Receiver.h \
  Event/ECG_UDP_Sender.h \
  Event/EC_And_Filter.h \
  Event/EC_Basic_Factory.h \
  Event/EC_Basic_Filter_Builder.h \
  Event/EC_Bitmask_Filter.h \
  Event/EC_Busy_Lock.h \
  Event/EC_Channel_Destroyer.h \
  Event/EC_Conjunction_Filter.h \
  Event/EC_ConsumerAdmin.h \
  Event/EC_ConsumerControl.h \
  Event/EC_Default_Factory.h \
  Event/EC_Default_ProxyConsumer.h \
  Event/EC_Default_ProxySupplier.h \
  Event/EC_Disjunction_Filter.h \
  Event/EC_Dispatching.h \
  Event/EC_Dispatching_Task.h \
  Event/EC_Event_Channel.h \
  Event/EC_Event_Channel_Base.h \
  Event/EC_Factory.h \
  Event/EC_Filter.h \
  Event/EC_Filter_Builder.h \
  Event/EC_Gateway.h \
  Event/EC_Gateway_IIOP.h \
  Event/EC_Gateway_IIOP_Factory.h \
  Event/EC_Group_Scheduling.h \
  Event/EC_Lifetime_Utils.h \
  Event/EC_Lifetime_Utils_T.h \
  Event/EC_MT_Dispatching.h \
  Event/EC_Masked_Type_Filter.h \
  Event/EC_Negation_Filter.h \
  Event/EC_Null_Factory.h \
  Event/EC_Null_Scheduling.h \
  Event/EC_ObserverStrategy.h \
  Event/EC_Per_Supplier_Filter.h \
  Event/EC_Prefix_Filter_Builder.h \
  Event/EC_ProxyConsumer.h \
  Event/EC_ProxySupplier.h \
  Event/EC_Proxy_Disconnector.h \
  Event/EC_QOS_Info.h \
  Event/EC_Reactive_ConsumerControl.h \
  Event/EC_Reactive_Dispatching.h \
  Event/EC_Reactive_SupplierControl.h \
  Event/EC_Reactive_Timeout_Generator.h \
  Event/EC_Scheduling_Strategy.h \
  Event/EC_SupplierAdmin.h \
  Event/EC_SupplierControl.h \
  Event/EC_Supplier_Filter.h \
  Event/EC_Supplier_Filter_Builder.h \
  Event/EC_Timeout_Filter.h \
  Event/EC_Timeout_Generator.h \
  Event/EC_Trivial_Supplier_Filter.h \
  Event/EC_Type_Filter.h \
  Event/EC_UDP_Admin.h \
  Event_Utilities.h \
  RtecDefaultEventDataC.h \
  RtecDefaultEventDataS.h \
  RtecDefaultEventDataS_T.h \
  RtecEventChannelAdminC.h \
  RtecEventChannelAdminS.h \
  RtecEventChannelAdminS_T.h \
  RtecEventCommC.h \
  RtecEventCommS.h \
  RtecEventCommS_T.h \
  RtecUDPAdminC.h \
  RtecUDPAdminS.h \
  RtecUDPAdminS_T.h \
  Event/ECG_Address_Server_Base.i \
  Event/ECG_CDR_Message_Receiver.i \
  Event/ECG_CDR_Message_Sender.i \
  Event/ECG_Complex_Address_Server.i \
  Event/ECG_ConsumerEC_Control.i \
  Event/ECG_Mcast_EH.i \
  Event/ECG_Mcast_Gateway.i \
  Event/ECG_Reactive_ConsumerEC_Control.i \
  Event/ECG_Simple_Address_Server.i \
  Event/ECG_Simple_Mcast_EH.i \
  Event/ECG_UDP_EH.i \
  Event/ECG_UDP_Out_Endpoint.i \
  Event/ECG_UDP_Receiver.i \
  Event/ECG_UDP_Sender.inl \
  Event/EC_And_Filter.i \
  Event/EC_Basic_Factory.i \
  Event/EC_Basic_Filter_Builder.i \
  Event/EC_Bitmask_Filter.i \
  Event/EC_Busy_Lock.i \
  Event/EC_Conjunction_Filter.i \
  Event/EC_ConsumerAdmin.i \
  Event/EC_ConsumerControl.i \
  Event/EC_Default_Factory.i \
  Event/EC_Default_ProxyConsumer.i \
  Event/EC_Default_ProxySupplier.i \
  Event/EC_Disjunction_Filter.i \
  Event/EC_Dispatching.i \
  Event/EC_Dispatching_Task.i \
  Event/EC_Event_Channel.i \
  Event/EC_Event_Channel_Base.i \
  Event/EC_Factory.i \
  Event/EC_Filter.i \
  Event/EC_Filter_Builder.i \
  Event/EC_Gateway_IIOP_Factory.i \
  Event/EC_Group_Scheduling.inl \
  Event/EC_Lifetime_Utils.i \
  Event/EC_Lifetime_Utils_T.i \
  Event/EC_MT_Dispatching.i \
  Event/EC_Masked_Type_Filter.i \
  Event/EC_Negation_Filter.i \
  Event/EC_Null_Factory.i \
  Event/EC_Null_Scheduling.i \
  Event/EC_ObserverStrategy.i \
  Event/EC_Per_Supplier_Filter.i \
  Event/EC_Prefix_Filter_Builder.i \
  Event/EC_ProxyConsumer.i \
  Event/EC_ProxySupplier.i \
  Event/EC_QOS_Info.i \
  Event/EC_Reactive_ConsumerControl.i \
  Event/EC_Reactive_Dispatching.i \
  Event/EC_Reactive_SupplierControl.i \
  Event/EC_Reactive_Timeout_Generator.i \
  Event/EC_Scheduling_Strategy.i \
  Event/EC_SupplierAdmin.i \
  Event/EC_SupplierControl.i \
  Event/EC_Supplier_Filter.i \
  Event/EC_Supplier_Filter_Builder.i \
  Event/EC_Timeout_Filter.i \
  Event/EC_Timeout_Generator.i \
  Event/EC_Trivial_Supplier_Filter.i \
  Event/EC_Type_Filter.i \
  Event_Utilities.i \
  RtecDefaultEventDataC.inl \
  RtecDefaultEventDataS.inl \
  RtecDefaultEventDataS_T.inl \
  RtecEventChannelAdminC.inl \
  RtecEventChannelAdminS.inl \
  RtecEventChannelAdminS_T.inl \
  RtecEventCommC.inl \
  RtecEventCommS.inl \
  RtecEventCommS_T.inl \
  RtecUDPAdminC.inl \
  RtecUDPAdminS.inl \
  RtecUDPAdminS_T.inl \
  Event/EC_Lifetime_Utils_T.cpp \
  RtecDefaultEventDataS_T.cpp \
  RtecEventChannelAdminS_T.cpp \
  RtecEventCommS_T.cpp \
  RtecUDPAdminS_T.cpp \
  RtecDefaultEventData.idl \
  RtecEventChannelAdmin.idl \
  RtecEventComm.idl \
  RtecUDPAdmin.idl

endif BUILD_CORBA_MESSAGING

EXTRA_DIST += \
  RTEvent.rc

## Makefile.FtRtEvent.am 

if BUILD_CORBA_MESSAGING
if !BUILD_MINIMUM_CORBA

BUILT_SOURCES += \
  FTRTS_T.cpp \
  FTRTC.inl \
  FTRTS.inl \
  FTRTS_T.inl \
  FTRTC.cpp \
  FTRTS.cpp \
  FTRTC.h \
  FTRTS.h \
  FTRTS_T.h

CLEANFILES += \
  FTRTS_T.cpp \
  FTRTC.inl \
  FTRTS.inl \
  FTRTS_T.inl \
  FTRTC.cpp \
  FTRTS.cpp \
  FTRTC.h \
  FTRTS.h \
  FTRTS_T.h \
  FTRT-stamp

FTRTS_T.cpp FTRTC.inl FTRTS.inl FTRTS_T.inl FTRTC.cpp FTRTS.cpp FTRTC.h FTRTS.h FTRTS_T.h: FTRT-stamp

FTRT-stamp: $(srcdir)/FTRT.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_FtRtEvent_Export -Wb,export_include=orbsvcs/FtRtEvent/Utils/ftrtevent_export.h -GC $(srcdir)/FTRT.idl
	@touch $@

BUILT_SOURCES += \
  FTRT_GroupManagerS_T.cpp \
  FTRT_GroupManagerC.inl \
  FTRT_GroupManagerS.inl \
  FTRT_GroupManagerS_T.inl \
  FTRT_GroupManagerC.cpp \
  FTRT_GroupManagerS.cpp \
  FTRT_GroupManagerC.h \
  FTRT_GroupManagerS.h \
  FTRT_GroupManagerS_T.h

CLEANFILES += \
  FTRT_GroupManagerS_T.cpp \
  FTRT_GroupManagerC.inl \
  FTRT_GroupManagerS.inl \
  FTRT_GroupManagerS_T.inl \
  FTRT_GroupManagerC.cpp \
  FTRT_GroupManagerS.cpp \
  FTRT_GroupManagerC.h \
  FTRT_GroupManagerS.h \
  FTRT_GroupManagerS_T.h \
  FTRT_GroupManager-stamp

FTRT_GroupManagerS_T.cpp FTRT_GroupManagerC.inl FTRT_GroupManagerS.inl FTRT_GroupManagerS_T.inl FTRT_GroupManagerC.cpp FTRT_GroupManagerS.cpp FTRT_GroupManagerC.h FTRT_GroupManagerS.h FTRT_GroupManagerS_T.h: FTRT_GroupManager-stamp

FTRT_GroupManager-stamp: $(srcdir)/FTRT_GroupManager.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_FtRtEvent_Export -Wb,export_include=orbsvcs/FtRtEvent/Utils/ftrtevent_export.h -GC $(srcdir)/FTRT_GroupManager.idl
	@touch $@

BUILT_SOURCES += \
  FtRtecEventChannelAdminS_T.cpp \
  FtRtecEventChannelAdminC.inl \
  FtRtecEventChannelAdminS.inl \
  FtRtecEventChannelAdminS_T.inl \
  FtRtecEventChannelAdminC.cpp \
  FtRtecEventChannelAdminS.cpp \
  FtRtecEventChannelAdminC.h \
  FtRtecEventChannelAdminS.h \
  FtRtecEventChannelAdminS_T.h

CLEANFILES += \
  FtRtecEventChannelAdminS_T.cpp \
  FtRtecEventChannelAdminC.inl \
  FtRtecEventChannelAdminS.inl \
  FtRtecEventChannelAdminS_T.inl \
  FtRtecEventChannelAdminC.cpp \
  FtRtecEventChannelAdminS.cpp \
  FtRtecEventChannelAdminC.h \
  FtRtecEventChannelAdminS.h \
  FtRtecEventChannelAdminS_T.h \
  FtRtecEventChannelAdmin-stamp

FtRtecEventChannelAdminS_T.cpp FtRtecEventChannelAdminC.inl FtRtecEventChannelAdminS.inl FtRtecEventChannelAdminS_T.inl FtRtecEventChannelAdminC.cpp FtRtecEventChannelAdminS.cpp FtRtecEventChannelAdminC.h FtRtecEventChannelAdminS.h FtRtecEventChannelAdminS_T.h: FtRtecEventChannelAdmin-stamp

FtRtecEventChannelAdmin-stamp: $(srcdir)/FtRtecEventChannelAdmin.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_FtRtEvent_Export -Wb,export_include=orbsvcs/FtRtEvent/Utils/ftrtevent_export.h $(srcdir)/FtRtecEventChannelAdmin.idl
	@touch $@

BUILT_SOURCES += \
  FtRtecEventCommS_T.cpp \
  FtRtecEventCommC.inl \
  FtRtecEventCommS.inl \
  FtRtecEventCommS_T.inl \
  FtRtecEventCommC.cpp \
  FtRtecEventCommS.cpp \
  FtRtecEventCommC.h \
  FtRtecEventCommS.h \
  FtRtecEventCommS_T.h

CLEANFILES += \
  FtRtecEventCommS_T.cpp \
  FtRtecEventCommC.inl \
  FtRtecEventCommS.inl \
  FtRtecEventCommS_T.inl \
  FtRtecEventCommC.cpp \
  FtRtecEventCommS.cpp \
  FtRtecEventCommC.h \
  FtRtecEventCommS.h \
  FtRtecEventCommS_T.h \
  FtRtecEventComm-stamp

FtRtecEventCommS_T.cpp FtRtecEventCommC.inl FtRtecEventCommS.inl FtRtecEventCommS_T.inl FtRtecEventCommC.cpp FtRtecEventCommS.cpp FtRtecEventCommC.h FtRtecEventCommS.h FtRtecEventCommS_T.h: FtRtecEventComm-stamp

FtRtecEventComm-stamp: $(srcdir)/FtRtecEventComm.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_FtRtEvent_Export -Wb,export_include=orbsvcs/FtRtEvent/Utils/ftrtevent_export.h -GC $(srcdir)/FtRtecEventComm.idl
	@touch $@

FtRtEvent = \
  FTRTC.cpp \
  FTRTS.cpp \
  FTRT_GroupManagerC.cpp \
  FTRT_GroupManagerS.cpp \
  FtRtEvent/Utils/FTEC_Gateway.cpp \
  FtRtEvent/Utils/Log.cpp \
  FtRtEvent/Utils/Safe_InputCDR.cpp \
  FtRtEvent/Utils/UUID.cpp \
  FtRtecEventChannelAdminC.cpp \
  FtRtecEventChannelAdminS.cpp \
  FtRtecEventCommC.cpp \
  FtRtecEventCommS.cpp

lib_LTLIBRARIES += libTAO_FtRtEvent.la

libTAO_FtRtEvent_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_FTRTEVENT_BUILD_DLL

libTAO_FtRtEvent_la_SOURCES = \
  $(FtRtEvent)

libTAO_FtRtEvent_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_FtRtEvent_la_LIBADD = \
  libTAO_CosNaming.la \
  libTAO_RTEvent.la \
  libTAO_Svc_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  libTAO_FTORB_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORManip.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  FTRTC.h \
  FTRTS.h \
  FTRTS_T.h \
  FTRT_GroupManagerC.h \
  FTRT_GroupManagerS.h \
  FTRT_GroupManagerS_T.h \
  FtRtEvent/Utils/FTEC_Gateway.h \
  FtRtEvent/Utils/Log.h \
  FtRtEvent/Utils/Safe_InputCDR.h \
  FtRtEvent/Utils/UUID.h \
  FtRtecEventChannelAdminC.h \
  FtRtecEventChannelAdminS.h \
  FtRtecEventChannelAdminS_T.h \
  FtRtecEventCommC.h \
  FtRtecEventCommS.h \
  FtRtecEventCommS_T.h \
  FTRTC.inl \
  FTRTS.inl \
  FTRTS_T.inl \
  FTRT_GroupManagerC.inl \
  FTRT_GroupManagerS.inl \
  FTRT_GroupManagerS_T.inl \
  FtRtEvent/Utils/FTEC_Gateway.inl \
  FtRtEvent/Utils/Log.inl \
  FtRtEvent/Utils/Safe_InputCDR.inl \
  FtRtEvent/Utils/UUID.inl \
  FtRtecEventChannelAdminC.inl \
  FtRtecEventChannelAdminS.inl \
  FtRtecEventChannelAdminS_T.inl \
  FtRtecEventCommC.inl \
  FtRtecEventCommS.inl \
  FtRtecEventCommS_T.inl \
  FTRTS_T.cpp \
  FTRT_GroupManagerS_T.cpp \
  FtRtecEventChannelAdminS_T.cpp \
  FtRtecEventCommS_T.cpp \
  FTRT.idl \
  FTRT_GroupManager.idl \
  FtRtecEventChannelAdmin.idl \
  FtRtecEventComm.idl

endif !BUILD_MINIMUM_CORBA
endif BUILD_CORBA_MESSAGING

## Makefile.FTRT_EventChannel.am 

if BUILD_CORBA_MESSAGING
if !BUILD_MINIMUM_CORBA

FTRT_EventChannel = \
  FtRtEvent/EventChannel/AMI_Primary_Replication_Strategy.cpp \
  FtRtEvent/EventChannel/AMI_Replication_Strategy.cpp \
  FtRtEvent/EventChannel/Basic_Replication_Strategy.cpp \
  FtRtEvent/EventChannel/Dynamic_Bitset.cpp \
  FtRtEvent/EventChannel/FTEC_Become_Primary_Listener.cpp \
  FtRtEvent/EventChannel/FTEC_ConsumerAdmin.cpp \
  FtRtEvent/EventChannel/FTEC_Event_Channel.cpp \
  FtRtEvent/EventChannel/FTEC_Event_Channel_Impl.cpp \
  FtRtEvent/EventChannel/FTEC_Factory.cpp \
  FtRtEvent/EventChannel/FTEC_Group_Manager.cpp \
  FtRtEvent/EventChannel/FTEC_ORBInitializer.cpp \
  FtRtEvent/EventChannel/FTEC_ProxyConsumer.cpp \
  FtRtEvent/EventChannel/FTEC_ProxySupplier.cpp \
  FtRtEvent/EventChannel/FTEC_SupplierAdmin.cpp \
  FtRtEvent/EventChannel/FT_ProxyAdmin_Base.cpp \
  FtRtEvent/EventChannel/Fault_Detector.cpp \
  FtRtEvent/EventChannel/Fault_Detector_Loader.cpp \
  FtRtEvent/EventChannel/ForwardCtrlServerInterceptor.cpp \
  FtRtEvent/EventChannel/FtEventServiceInterceptor.cpp \
  FtRtEvent/EventChannel/GroupInfoPublisher.cpp \
  FtRtEvent/EventChannel/IOGR_Maker.cpp \
  FtRtEvent/EventChannel/Identification_Service.cpp \
  FtRtEvent/EventChannel/ObjectGroupManagerHandler.cpp \
  FtRtEvent/EventChannel/ProxyConsumerStateWorker.cpp \
  FtRtEvent/EventChannel/ProxySupplierStateWorker.cpp \
  FtRtEvent/EventChannel/Replication_Service.cpp \
  FtRtEvent/EventChannel/Replication_Strategy.cpp \
  FtRtEvent/EventChannel/Request_Context_Repository.cpp \
  FtRtEvent/EventChannel/SCTP_Fault_Detector.cpp \
  FtRtEvent/EventChannel/Set_Update_Interceptor.cpp \
  FtRtEvent/EventChannel/Update_Manager.cpp \
  FtRtEvent/EventChannel/UpdateableHandler.cpp \
  FtRtEvent/EventChannel/create_persistent_poa.cpp \
  PortableGroup_Simple_DSC.cpp

lib_LTLIBRARIES += libTAO_FTRT_EventChannel.la

libTAO_FTRT_EventChannel_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -I$(TAO_ROOT)/orbsvcs/orbsvcs/FtRtEvent/EventChannel \
  -DTAO_PORTABLEGROUP_SIMPLE_HAS_DLL=0 \
  -DTAO_FTRTEC_BUILD_DLL

libTAO_FTRT_EventChannel_la_SOURCES = \
  $(FTRT_EventChannel)

libTAO_FTRT_EventChannel_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_FTRT_EventChannel_la_LIBADD = \
  $(TAO_BUILDDIR)/tao/libTAO_Utils.la \
  libTAO_FtRtEvent.la \
  libTAO_CosNaming.la \
  libTAO_RTEvent.la \
  libTAO_Svc_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  libTAO_FTORB_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORManip.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  FtRtEvent/EventChannel/AMI_Primary_Replication_Strategy.h \
  FtRtEvent/EventChannel/AMI_Replication_Strategy.h \
  FtRtEvent/EventChannel/Basic_Replication_Strategy.h \
  FtRtEvent/EventChannel/ConnectionHandler_T.h \
  FtRtEvent/EventChannel/Dynamic_Bitset.h \
  FtRtEvent/EventChannel/FTEC_Become_Primary_Listener.h \
  FtRtEvent/EventChannel/FTEC_ConsumerAdmin.h \
  FtRtEvent/EventChannel/FTEC_Event_Channel.h \
  FtRtEvent/EventChannel/FTEC_Event_Channel_Impl.h \
  FtRtEvent/EventChannel/FTEC_Factory.h \
  FtRtEvent/EventChannel/FTEC_Group_Manager.h \
  FtRtEvent/EventChannel/FTEC_ORBInitializer.h \
  FtRtEvent/EventChannel/FTEC_ProxyConsumer.h \
  FtRtEvent/EventChannel/FTEC_ProxySupplier.h \
  FtRtEvent/EventChannel/FTEC_SupplierAdmin.h \
  FtRtEvent/EventChannel/FT_ProxyAdmin_Base.h \
  FtRtEvent/EventChannel/FT_ProxyAdmin_T.h \
  FtRtEvent/EventChannel/Fault_Detector.h \
  FtRtEvent/EventChannel/Fault_Detector_Loader.h \
  FtRtEvent/EventChannel/Fault_Detector_T.h \
  FtRtEvent/EventChannel/ForwardCtrlServerInterceptor.h \
  FtRtEvent/EventChannel/FtEventServiceInterceptor.h \
  FtRtEvent/EventChannel/GroupInfoPublisher.h \
  FtRtEvent/EventChannel/IOGR_Maker.h \
  FtRtEvent/EventChannel/Identification_Service.h \
  FtRtEvent/EventChannel/ObjectGroupManagerHandler.h \
  FtRtEvent/EventChannel/ProxyConsumerStateWorker.h \
  FtRtEvent/EventChannel/ProxySupplierStateWorker.h \
  FtRtEvent/EventChannel/Replication_Service.h \
  FtRtEvent/EventChannel/Replication_Strategy.h \
  FtRtEvent/EventChannel/Request_Context_Repository.h \
  FtRtEvent/EventChannel/SCTP_Fault_Detector.h \
  FtRtEvent/EventChannel/Set_Update_Interceptor.h \
  FtRtEvent/EventChannel/Update_Manager.h \
  FtRtEvent/EventChannel/UpdateableHandler.h \
  FtRtEvent/EventChannel/create_persistent_poa.h \
  FtRtEvent/EventChannel/ftrtec_export.h \
  FtRtEvent/EventChannel/Dynamic_Bitset.inl \
  FtRtEvent/EventChannel/ConnectionHandler_T.cpp \
  FtRtEvent/EventChannel/FT_ProxyAdmin_T.cpp \
  FtRtEvent/EventChannel/Fault_Detector_T.cpp

endif !BUILD_MINIMUM_CORBA
endif BUILD_CORBA_MESSAGING

## Makefile.FT_ClientORB.am 

if BUILD_CORBA_MESSAGING
if BUILD_ACE_UUID
if !BUILD_MINIMUM_CORBA

FT_ClientORB = \
  FaultTolerance/FT_ClientORBInitializer.cpp \
  FaultTolerance/FT_ClientPolicyFactory.cpp \
  FaultTolerance/FT_ClientPolicy_i.cpp \
  FaultTolerance/FT_ClientRequest_Interceptor.cpp \
  FaultTolerance/FT_ClientService_Activate.cpp \
  FaultTolerance/FT_Endpoint_Selector_Factory.cpp \
  FaultTolerance/FT_Invocation_Endpoint_Selectors.cpp \
  FaultTolerance/FT_Service_Callbacks.cpp

lib_LTLIBRARIES += libTAO_FT_ClientORB.la

libTAO_FT_ClientORB_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_FT_CLIENTORB_BUILD_DLL

libTAO_FT_ClientORB_la_SOURCES = \
  $(FT_ClientORB)

libTAO_FT_ClientORB_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_FT_ClientORB_la_LIBADD = \
  libTAO_PortableGroup.la \
  libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  libTAO_FTORB_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORManip.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  FaultTolerance/FT_ClientORBInitializer.h \
  FaultTolerance/FT_ClientPolicyFactory.h \
  FaultTolerance/FT_ClientPolicy_i.h \
  FaultTolerance/FT_ClientRequest_Interceptor.h \
  FaultTolerance/FT_ClientService_Activate.h \
  FaultTolerance/FT_Endpoint_Selector_Factory.h \
  FaultTolerance/FT_Invocation_Endpoint_Selectors.h \
  FaultTolerance/FT_Service_Callbacks.h \
  FaultTolerance/FT_ClientPolicy_i.inl \
  FaultTolerance/FT_Invocation_Endpoint_Selectors.inl \
  FaultTolerance/FT_Service_Callbacks.i

endif !BUILD_MINIMUM_CORBA
endif BUILD_ACE_UUID
endif BUILD_CORBA_MESSAGING

## Makefile.FT_ServerORB.am 

if BUILD_CORBA_MESSAGING
if !BUILD_MINIMUM_CORBA

FT_ServerORB = \
  FaultTolerance/FT_ServerORBInitializer.cpp \
  FaultTolerance/FT_ServerPolicyFactory.cpp \
  FaultTolerance/FT_ServerPolicy_i.cpp \
  FaultTolerance/FT_ServerRequest_Interceptor.cpp \
  FaultTolerance/FT_ServerService_Activate.cpp

lib_LTLIBRARIES += libTAO_FT_ServerORB.la

libTAO_FT_ServerORB_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_FT_SERVERORB_BUILD_DLL

libTAO_FT_ServerORB_la_SOURCES = \
  $(FT_ServerORB)

libTAO_FT_ServerORB_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_FT_ServerORB_la_LIBADD = \
  libTAO_PortableGroup.la \
  libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  libTAO_FTORB_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORManip.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  FaultTolerance/FT_ServerORBInitializer.h \
  FaultTolerance/FT_ServerPolicyFactory.h \
  FaultTolerance/FT_ServerPolicy_i.h \
  FaultTolerance/FT_ServerRequest_Interceptor.h \
  FaultTolerance/FT_ServerService_Activate.h \
  FaultTolerance/FT_ServerPolicy_i.inl

endif !BUILD_MINIMUM_CORBA
endif BUILD_CORBA_MESSAGING

## Makefile.FaultTolerance.am 

if BUILD_ACE_UUID
if BUILD_CORBA_MESSAGING
if !BUILD_MINIMUM_CORBA

BUILT_SOURCES += \
  FT_CORBAS_T.cpp \
  FT_CORBAC.inl \
  FT_CORBAS.inl \
  FT_CORBAS_T.inl \
  FT_CORBAC.cpp \
  FT_CORBAS.cpp \
  FT_CORBAC.h \
  FT_CORBAS.h \
  FT_CORBAS_T.h

CLEANFILES += \
  FT_CORBAS_T.cpp \
  FT_CORBAC.inl \
  FT_CORBAS.inl \
  FT_CORBAS_T.inl \
  FT_CORBAC.cpp \
  FT_CORBAS.cpp \
  FT_CORBAC.h \
  FT_CORBAS.h \
  FT_CORBAS_T.h \
  FT_CORBA-stamp

FT_CORBAS_T.cpp FT_CORBAC.inl FT_CORBAS.inl FT_CORBAS_T.inl FT_CORBAC.cpp FT_CORBAS.cpp FT_CORBAC.h FT_CORBAS.h FT_CORBAS_T.h: FT_CORBA-stamp

FT_CORBA-stamp: $(srcdir)/FT_CORBA.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_FT_Export -Wb,export_include=orbsvcs/FaultTolerance/fault_tol_export.h -Wb,skel_export_include=tao/PortableServer/PolicyS.h $(srcdir)/FT_CORBA.idl
	@touch $@

BUILT_SOURCES += \
  FT_FaultDetectorFactoryS_T.cpp \
  FT_FaultDetectorFactoryC.inl \
  FT_FaultDetectorFactoryS.inl \
  FT_FaultDetectorFactoryS_T.inl \
  FT_FaultDetectorFactoryC.cpp \
  FT_FaultDetectorFactoryS.cpp \
  FT_FaultDetectorFactoryC.h \
  FT_FaultDetectorFactoryS.h \
  FT_FaultDetectorFactoryS_T.h

CLEANFILES += \
  FT_FaultDetectorFactoryS_T.cpp \
  FT_FaultDetectorFactoryC.inl \
  FT_FaultDetectorFactoryS.inl \
  FT_FaultDetectorFactoryS_T.inl \
  FT_FaultDetectorFactoryC.cpp \
  FT_FaultDetectorFactoryS.cpp \
  FT_FaultDetectorFactoryC.h \
  FT_FaultDetectorFactoryS.h \
  FT_FaultDetectorFactoryS_T.h \
  FT_FaultDetectorFactory-stamp

FT_FaultDetectorFactoryS_T.cpp FT_FaultDetectorFactoryC.inl FT_FaultDetectorFactoryS.inl FT_FaultDetectorFactoryS_T.inl FT_FaultDetectorFactoryC.cpp FT_FaultDetectorFactoryS.cpp FT_FaultDetectorFactoryC.h FT_FaultDetectorFactoryS.h FT_FaultDetectorFactoryS_T.h: FT_FaultDetectorFactory-stamp

FT_FaultDetectorFactory-stamp: $(srcdir)/FT_FaultDetectorFactory.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_FT_Export -Wb,export_include=orbsvcs/FaultTolerance/fault_tol_export.h -Wb,skel_export_include=tao/PortableServer/PolicyS.h $(srcdir)/FT_FaultDetectorFactory.idl
	@touch $@

BUILT_SOURCES += \
  FT_NotifierS_T.cpp \
  FT_NotifierC.inl \
  FT_NotifierS.inl \
  FT_NotifierS_T.inl \
  FT_NotifierC.cpp \
  FT_NotifierS.cpp \
  FT_NotifierC.h \
  FT_NotifierS.h \
  FT_NotifierS_T.h

CLEANFILES += \
  FT_NotifierS_T.cpp \
  FT_NotifierC.inl \
  FT_NotifierS.inl \
  FT_NotifierS_T.inl \
  FT_NotifierC.cpp \
  FT_NotifierS.cpp \
  FT_NotifierC.h \
  FT_NotifierS.h \
  FT_NotifierS_T.h \
  FT_Notifier-stamp

FT_NotifierS_T.cpp FT_NotifierC.inl FT_NotifierS.inl FT_NotifierS_T.inl FT_NotifierC.cpp FT_NotifierS.cpp FT_NotifierC.h FT_NotifierS.h FT_NotifierS_T.h: FT_Notifier-stamp

FT_Notifier-stamp: $(srcdir)/FT_Notifier.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_FT_Export -Wb,export_include=orbsvcs/FaultTolerance/fault_tol_export.h -Wb,skel_export_include=tao/PortableServer/PolicyS.h $(srcdir)/FT_Notifier.idl
	@touch $@

BUILT_SOURCES += \
  FT_ReplicaS_T.cpp \
  FT_ReplicaC.inl \
  FT_ReplicaS.inl \
  FT_ReplicaS_T.inl \
  FT_ReplicaC.cpp \
  FT_ReplicaS.cpp \
  FT_ReplicaC.h \
  FT_ReplicaS.h \
  FT_ReplicaS_T.h

CLEANFILES += \
  FT_ReplicaS_T.cpp \
  FT_ReplicaC.inl \
  FT_ReplicaS.inl \
  FT_ReplicaS_T.inl \
  FT_ReplicaC.cpp \
  FT_ReplicaS.cpp \
  FT_ReplicaC.h \
  FT_ReplicaS.h \
  FT_ReplicaS_T.h \
  FT_Replica-stamp

FT_ReplicaS_T.cpp FT_ReplicaC.inl FT_ReplicaS.inl FT_ReplicaS_T.inl FT_ReplicaC.cpp FT_ReplicaS.cpp FT_ReplicaC.h FT_ReplicaS.h FT_ReplicaS_T.h: FT_Replica-stamp

FT_Replica-stamp: $(srcdir)/FT_Replica.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_FT_Export -Wb,export_include=orbsvcs/FaultTolerance/fault_tol_export.h -Wb,skel_export_include=tao/PortableServer/PolicyS.h $(srcdir)/FT_Replica.idl
	@touch $@

BUILT_SOURCES += \
  FT_ReplicationManagerS_T.cpp \
  FT_ReplicationManagerC.inl \
  FT_ReplicationManagerS.inl \
  FT_ReplicationManagerS_T.inl \
  FT_ReplicationManagerC.cpp \
  FT_ReplicationManagerS.cpp \
  FT_ReplicationManagerC.h \
  FT_ReplicationManagerS.h \
  FT_ReplicationManagerS_T.h

CLEANFILES += \
  FT_ReplicationManagerS_T.cpp \
  FT_ReplicationManagerC.inl \
  FT_ReplicationManagerS.inl \
  FT_ReplicationManagerS_T.inl \
  FT_ReplicationManagerC.cpp \
  FT_ReplicationManagerS.cpp \
  FT_ReplicationManagerC.h \
  FT_ReplicationManagerS.h \
  FT_ReplicationManagerS_T.h \
  FT_ReplicationManager-stamp

FT_ReplicationManagerS_T.cpp FT_ReplicationManagerC.inl FT_ReplicationManagerS.inl FT_ReplicationManagerS_T.inl FT_ReplicationManagerC.cpp FT_ReplicationManagerS.cpp FT_ReplicationManagerC.h FT_ReplicationManagerS.h FT_ReplicationManagerS_T.h: FT_ReplicationManager-stamp

FT_ReplicationManager-stamp: $(srcdir)/FT_ReplicationManager.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_FT_Export -Wb,export_include=orbsvcs/FaultTolerance/fault_tol_export.h -Wb,skel_export_include=tao/PortableServer/PolicyS.h $(srcdir)/FT_ReplicationManager.idl
	@touch $@

FaultTolerance = \
  FT_CORBAC.cpp \
  FT_CORBAS.cpp \
  FT_FaultDetectorFactoryC.cpp \
  FT_FaultDetectorFactoryS.cpp \
  FT_NotifierC.cpp \
  FT_NotifierS.cpp \
  FT_ReplicaC.cpp \
  FT_ReplicaS.cpp \
  FT_ReplicationManagerC.cpp \
  FT_ReplicationManagerS.cpp

lib_LTLIBRARIES += libTAO_FaultTolerance.la

libTAO_FaultTolerance_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_HAS_TYPED_EVENT_CHANNEL \
  -DTAO_FT_BUILD_DLL

libTAO_FaultTolerance_la_SOURCES = \
  $(FaultTolerance)

libTAO_FaultTolerance_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_FaultTolerance_la_LIBADD = \
  libTAO_FT_ServerORB.la \
  libTAO_PortableGroup.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  libTAO_CosNaming.la \
  libTAO_FT_ClientORB.la \
  libTAO_FTORB_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORManip.la \
  libTAO_CosNotification.la \
  libTAO_CosEvent.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  FT_CORBAC.h \
  FT_CORBAS.h \
  FT_CORBAS_T.h \
  FT_FaultDetectorFactoryC.h \
  FT_FaultDetectorFactoryS.h \
  FT_FaultDetectorFactoryS_T.h \
  FT_NotifierC.h \
  FT_NotifierS.h \
  FT_NotifierS_T.h \
  FT_ReplicaC.h \
  FT_ReplicaS.h \
  FT_ReplicaS_T.h \
  FT_ReplicationManagerC.h \
  FT_ReplicationManagerS.h \
  FT_ReplicationManagerS_T.h \
  FaultTolerance/fault_tol_export.h \
  FT_CORBAC.inl \
  FT_CORBAS.inl \
  FT_CORBAS_T.inl \
  FT_FaultDetectorFactoryC.inl \
  FT_FaultDetectorFactoryS.inl \
  FT_FaultDetectorFactoryS_T.inl \
  FT_NotifierC.inl \
  FT_NotifierS.inl \
  FT_NotifierS_T.inl \
  FT_ReplicaC.inl \
  FT_ReplicaS.inl \
  FT_ReplicaS_T.inl \
  FT_ReplicationManagerC.inl \
  FT_ReplicationManagerS.inl \
  FT_ReplicationManagerS_T.inl \
  FT_CORBAS_T.cpp \
  FT_FaultDetectorFactoryS_T.cpp \
  FT_NotifierS_T.cpp \
  FT_ReplicaS_T.cpp \
  FT_ReplicationManagerS_T.cpp \
  FT_CORBA.idl \
  FT_FaultDetectorFactory.idl \
  FT_Notifier.idl \
  FT_Replica.idl \
  FT_ReplicationManager.idl

endif !BUILD_MINIMUM_CORBA
endif BUILD_CORBA_MESSAGING
endif BUILD_ACE_UUID

EXTRA_DIST += \
  FaultTolerance.rc

## Makefile.HTIOP.am 

BUILT_SOURCES += \
  HTIOPS_T.cpp \
  HTIOPC.inl \
  HTIOPS.inl \
  HTIOPS_T.inl \
  HTIOPC.cpp \
  HTIOPC.h \
  HTIOPS.h \
  HTIOPS_T.h

CLEANFILES += \
  HTIOPS_T.cpp \
  HTIOPC.inl \
  HTIOPS.inl \
  HTIOPS_T.inl \
  HTIOPC.cpp \
  HTIOPC.h \
  HTIOPS.h \
  HTIOPS_T.h \
  HTIOP-stamp

HTIOPS_T.cpp HTIOPC.inl HTIOPS.inl HTIOPS_T.inl HTIOPC.cpp HTIOPC.h HTIOPS.h HTIOPS_T.h: HTIOP-stamp

HTIOP-stamp: $(srcdir)/HTIOP.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=HTIOP_Export -Wb,export_include=orbsvcs/HTIOP/HTIOP_Export.h $(srcdir)/HTIOP.idl
	@touch $@

HTIOP = \
  HTIOP/HTIOP_Acceptor.cpp \
  HTIOP/HTIOP_Acceptor_Impl.cpp \
  HTIOP/HTIOP_Completion_Handler.cpp \
  HTIOP/HTIOP_Connection_Handler.cpp \
  HTIOP/HTIOP_Connector.cpp \
  HTIOP/HTIOP_Connector_Impl.cpp \
  HTIOP/HTIOP_Endpoint.cpp \
  HTIOP/HTIOP_Factory.cpp \
  HTIOP/HTIOP_Profile.cpp \
  HTIOP/HTIOP_Transport.cpp \
  HTIOP/htiop_endpointsC.cpp \
  HTIOPC.cpp

lib_LTLIBRARIES += libTAO_HTIOP.la

libTAO_HTIOP_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(ACE_ROOT)/protocols \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DHTIOP_BUILD_DLL

libTAO_HTIOP_la_SOURCES = \
  $(HTIOP)

libTAO_HTIOP_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_HTIOP_la_LIBADD = \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/protocols/ace/HTBP/libACE_HTBP.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  HTIOP/HTIOP_Acceptor.h \
  HTIOP/HTIOP_Acceptor_Impl.h \
  HTIOP/HTIOP_Completion_Handler.h \
  HTIOP/HTIOP_Connection_Handler.h \
  HTIOP/HTIOP_Connector.h \
  HTIOP/HTIOP_Connector_Impl.h \
  HTIOP/HTIOP_Endpoint.h \
  HTIOP/HTIOP_Factory.h \
  HTIOP/HTIOP_Profile.h \
  HTIOP/HTIOP_Transport.h \
  HTIOP/htiop_endpointsC.h \
  HTIOPC.h \
  HTIOPS.h \
  HTIOPS_T.h \
  HTIOP/HTIOP_Acceptor.i \
  HTIOP/HTIOP_Acceptor_Impl.i \
  HTIOP/HTIOP_Completion_Handler.i \
  HTIOP/HTIOP_Connection_Handler.i \
  HTIOP/HTIOP_Connector_Impl.i \
  HTIOP/HTIOP_Endpoint.i \
  HTIOP/HTIOP_Profile.i \
  HTIOP/HTIOP_Transport.i \
  HTIOP/htiop_endpointsC.inl \
  HTIOPC.inl \
  HTIOPS.inl \
  HTIOPS_T.inl \
  HTIOPS_T.cpp \
  HTIOP.idl

## Makefile.IFRService.am 

if !BUILD_MINIMUM_CORBA

IFRService = \
  IFRService/AbstractInterfaceDef_i.cpp \
  IFRService/AliasDef_i.cpp \
  IFRService/ArrayDef_i.cpp \
  IFRService/AttributeDef_i.cpp \
  IFRService/ComponentContainer_i.cpp \
  IFRService/ComponentDef_i.cpp \
  IFRService/ComponentModuleDef_i.cpp \
  IFRService/ComponentRepository_i.cpp \
  IFRService/ConstantDef_i.cpp \
  IFRService/ConsumesDef_i.cpp \
  IFRService/Contained_i.cpp \
  IFRService/Container_i.cpp \
  IFRService/EmitsDef_i.cpp \
  IFRService/EnumDef_i.cpp \
  IFRService/EventDef_i.cpp \
  IFRService/EventPortDef_i.cpp \
  IFRService/ExceptionDef_i.cpp \
  IFRService/ExtAbstractInterfaceDef_i.cpp \
  IFRService/ExtAttributeDef_i.cpp \
  IFRService/ExtInterfaceDef_i.cpp \
  IFRService/ExtLocalInterfaceDef_i.cpp \
  IFRService/ExtValueDef_i.cpp \
  IFRService/FactoryDef_i.cpp \
  IFRService/FinderDef_i.cpp \
  IFRService/FixedDef_i.cpp \
  IFRService/HomeDef_i.cpp \
  IFRService/IDLType_i.cpp \
  IFRService/IFR_BaseS.cpp \
  IFRService/IFR_BasicS.cpp \
  IFRService/IFR_ComponentsS.cpp \
  IFRService/IFR_ExtendedS.cpp \
  IFRService/IFR_Service_Loader.cpp \
  IFRService/IFR_Service_Utils.cpp \
  IFRService/IRObject_i.cpp \
  IFRService/InterfaceAttrExtension_i.cpp \
  IFRService/InterfaceDef_i.cpp \
  IFRService/LocalInterfaceDef_i.cpp \
  IFRService/ModuleDef_i.cpp \
  IFRService/NativeDef_i.cpp \
  IFRService/OperationDef_i.cpp \
  IFRService/Options.cpp \
  IFRService/PrimitiveDef_i.cpp \
  IFRService/ProvidesDef_i.cpp \
  IFRService/PublishesDef_i.cpp \
  IFRService/Repository_i.cpp \
  IFRService/SequenceDef_i.cpp \
  IFRService/StringDef_i.cpp \
  IFRService/StructDef_i.cpp \
  IFRService/TypedefDef_i.cpp \
  IFRService/UnionDef_i.cpp \
  IFRService/UsesDef_i.cpp \
  IFRService/ValueBoxDef_i.cpp \
  IFRService/ValueDef_i.cpp \
  IFRService/ValueMemberDef_i.cpp \
  IFRService/WstringDef_i.cpp \
  IFRService/tmplinst.cpp

lib_LTLIBRARIES += libTAO_IFRService.la

libTAO_IFRService_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_IFRSERVICE_BUILD_DLL

libTAO_IFRService_la_SOURCES = \
  $(IFRService)

libTAO_IFRService_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_IFRService_la_LIBADD = \
  $(TAO_BUILDDIR)/tao/libTAO_IORTable.la \
  libTAO_Svc_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_TypeCodeFactory.la \
  $(TAO_BUILDDIR)/tao/libTAO_IFR_Client.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  IFRService/AbstractInterfaceDef_i.h \
  IFRService/AliasDef_i.h \
  IFRService/ArrayDef_i.h \
  IFRService/AttributeDef_i.h \
  IFRService/ComponentContainer_i.h \
  IFRService/ComponentDef_i.h \
  IFRService/ComponentModuleDef_i.h \
  IFRService/ComponentRepository_i.h \
  IFRService/ConstantDef_i.h \
  IFRService/ConsumesDef_i.h \
  IFRService/Contained_i.h \
  IFRService/Container_i.h \
  IFRService/EmitsDef_i.h \
  IFRService/EnumDef_i.h \
  IFRService/EventDef_i.h \
  IFRService/EventPortDef_i.h \
  IFRService/ExceptionDef_i.h \
  IFRService/ExtAbstractInterfaceDef_i.h \
  IFRService/ExtAttributeDef_i.h \
  IFRService/ExtInterfaceDef_i.h \
  IFRService/ExtLocalInterfaceDef_i.h \
  IFRService/ExtValueDef_i.h \
  IFRService/FactoryDef_i.h \
  IFRService/FinderDef_i.h \
  IFRService/FixedDef_i.h \
  IFRService/HomeDef_i.h \
  IFRService/IDLType_i.h \
  IFRService/IFR_BaseS.h \
  IFRService/IFR_BaseS_T.h \
  IFRService/IFR_BasicS.h \
  IFRService/IFR_BasicS_T.h \
  IFRService/IFR_ComponentsS.h \
  IFRService/IFR_ComponentsS_T.h \
  IFRService/IFR_ExtendedS.h \
  IFRService/IFR_ExtendedS_T.h \
  IFRService/IFR_Service_Loader.h \
  IFRService/IFR_Service_Utils.h \
  IFRService/IFR_Service_Utils_T.h \
  IFRService/IRObject_i.h \
  IFRService/InterfaceAttrExtension_i.h \
  IFRService/InterfaceDef_i.h \
  IFRService/LocalInterfaceDef_i.h \
  IFRService/ModuleDef_i.h \
  IFRService/NativeDef_i.h \
  IFRService/OperationDef_i.h \
  IFRService/Options.h \
  IFRService/PrimitiveDef_i.h \
  IFRService/ProvidesDef_i.h \
  IFRService/PublishesDef_i.h \
  IFRService/Repository_i.h \
  IFRService/SequenceDef_i.h \
  IFRService/StringDef_i.h \
  IFRService/StructDef_i.h \
  IFRService/TypedefDef_i.h \
  IFRService/UnionDef_i.h \
  IFRService/UsesDef_i.h \
  IFRService/ValueBoxDef_i.h \
  IFRService/ValueDef_i.h \
  IFRService/ValueMemberDef_i.h \
  IFRService/WstringDef_i.h \
  IFRService/IFR_BaseS.i \
  IFRService/IFR_BaseS_T.i \
  IFRService/IFR_BasicS.i \
  IFRService/IFR_BasicS_T.i \
  IFRService/IFR_ComponentsS.i \
  IFRService/IFR_ComponentsS_T.i \
  IFRService/IFR_ExtendedS.i \
  IFRService/IFR_ExtendedS_T.i \
  IFRService/IFR_BaseS_T.cpp \
  IFRService/IFR_BasicS_T.cpp \
  IFRService/IFR_ComponentsS_T.cpp \
  IFRService/IFR_ExtendedS_T.cpp \
  IFRService/IFR_Service_Utils_T.cpp

endif !BUILD_MINIMUM_CORBA

EXTRA_DIST += \
  IFRService.rc

## Makefile.RTCORBAEvent.am 

if BUILD_CORBA_MESSAGING
if BUILD_RT_CORBA

RTCORBAEvent = \
  Event/EC_RTCORBA_Dispatching.cpp \
  Event/EC_RTCORBA_Factory.cpp

lib_LTLIBRARIES += libTAO_RTCORBAEvent.la

libTAO_RTCORBAEvent_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_RTCORBAEVENT_BUILD_DLL

libTAO_RTCORBAEvent_la_SOURCES = \
  $(RTCORBAEvent)

libTAO_RTCORBAEvent_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_RTCORBAEvent_la_LIBADD = \
  $(TAO_BUILDDIR)/tao/libTAO_RTPortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_RTCORBA.la \
  libTAO_CosNaming.la \
  libTAO_RTEvent.la \
  libTAO_Svc_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  Event/EC_RTCORBA_Dispatching.h \
  Event/EC_RTCORBA_Factory.h \
  Event/EC_RTCORBA_Dispatching.inl \
  Event/EC_RTCORBA_Factory.inl

endif BUILD_RT_CORBA
endif BUILD_CORBA_MESSAGING

## Makefile.RTCosScheduling.am 

if BUILD_RT_CORBA
if BUILD_INTERCEPTORS

BUILT_SOURCES += \
  RTCosSchedulingS_T.cpp \
  RTCosSchedulingC.inl \
  RTCosSchedulingS.inl \
  RTCosSchedulingS_T.inl \
  RTCosSchedulingC.cpp \
  RTCosSchedulingS.cpp \
  RTCosSchedulingC.h \
  RTCosSchedulingS.h \
  RTCosSchedulingS_T.h

CLEANFILES += \
  RTCosSchedulingS_T.cpp \
  RTCosSchedulingC.inl \
  RTCosSchedulingS.inl \
  RTCosSchedulingS_T.inl \
  RTCosSchedulingC.cpp \
  RTCosSchedulingS.cpp \
  RTCosSchedulingC.h \
  RTCosSchedulingS.h \
  RTCosSchedulingS_T.h \
  RTCosScheduling-stamp

RTCosSchedulingS_T.cpp RTCosSchedulingC.inl RTCosSchedulingS.inl RTCosSchedulingS_T.inl RTCosSchedulingC.cpp RTCosSchedulingS.cpp RTCosSchedulingC.h RTCosSchedulingS.h RTCosSchedulingS_T.h: RTCosScheduling-stamp

RTCosScheduling-stamp: $(srcdir)/RTCosScheduling.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Gd -Wb,export_macro=TAO_RTCosScheduling_Export -Wb,export_include=orbsvcs/RTCosScheduling/RTCosScheduling_export.h $(srcdir)/RTCosScheduling.idl
	@touch $@

RTCosScheduling = \
  RTCosScheduling/RTCosScheduling_ClientScheduler_i.cpp \
  RTCosScheduling/RTCosScheduling_PCP_Manager.cpp \
  RTCosScheduling/RTCosScheduling_ServerScheduler_i.cpp \
  RTCosSchedulingC.cpp \
  RTCosSchedulingS.cpp

lib_LTLIBRARIES += libTAO_RTCosScheduling.la

libTAO_RTCosScheduling_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_RTCOSSCHEDULING_BUILD_DLL

libTAO_RTCosScheduling_la_SOURCES = \
  $(RTCosScheduling)

libTAO_RTCosScheduling_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_RTCosScheduling_la_LIBADD = \
  $(TAO_BUILDDIR)/tao/libTAO_RTPortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_RTCORBA.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  RTCosScheduling/RTCosScheduling_ClientScheduler_i.h \
  RTCosScheduling/RTCosScheduling_PCP_Manager.h \
  RTCosScheduling/RTCosScheduling_ServerScheduler_i.h \
  RTCosSchedulingC.h \
  RTCosSchedulingS.h \
  RTCosSchedulingS_T.h \
  RTCosScheduling/RTCosScheduling_ClientScheduler_i.i \
  RTCosScheduling/RTCosScheduling_PCP_Manager.i \
  RTCosScheduling/RTCosScheduling_ServerScheduler_i.i \
  RTCosSchedulingC.inl \
  RTCosSchedulingS.inl \
  RTCosSchedulingS_T.inl \
  RTCosSchedulingS_T.cpp \
  RTCosScheduling.idl

endif BUILD_INTERCEPTORS
endif BUILD_RT_CORBA

## Makefile.RTSched.am 

BUILT_SOURCES += \
  RtecSchedulerS_T.cpp \
  RtecSchedulerC.inl \
  RtecSchedulerS.inl \
  RtecSchedulerS_T.inl \
  RtecSchedulerC.cpp \
  RtecSchedulerS.cpp \
  RtecSchedulerC.h \
  RtecSchedulerS.h \
  RtecSchedulerS_T.h

CLEANFILES += \
  RtecSchedulerS_T.cpp \
  RtecSchedulerC.inl \
  RtecSchedulerS.inl \
  RtecSchedulerS_T.inl \
  RtecSchedulerC.cpp \
  RtecSchedulerS.cpp \
  RtecSchedulerC.h \
  RtecSchedulerS.h \
  RtecSchedulerS_T.h \
  RtecScheduler-stamp

RtecSchedulerS_T.cpp RtecSchedulerC.inl RtecSchedulerS.inl RtecSchedulerS_T.inl RtecSchedulerC.cpp RtecSchedulerS.cpp RtecSchedulerC.h RtecSchedulerS.h RtecSchedulerS_T.h: RtecScheduler-stamp

RtecScheduler-stamp: $(srcdir)/RtecScheduler.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_RTSched_Export -Wb,export_include=orbsvcs/Sched/sched_export.h $(srcdir)/RtecScheduler.idl
	@touch $@

Sched = \
  RtecSchedulerC.cpp \
  RtecSchedulerS.cpp \
  Runtime_Scheduler.cpp \
  Sched/Config_Scheduler.cpp \
  Sched/DynSched.cpp \
  Sched/Reconfig_Sched_Utils.cpp \
  Sched/Reconfig_Scheduler.cpp \
  Sched/SchedEntry.cpp \
  Sched/Scheduler.cpp \
  Sched/Scheduler_Generic.cpp \
  Sched/Strategy_Scheduler.cpp \
  Scheduler_Factory.cpp \
  Scheduler_Utilities.cpp

lib_LTLIBRARIES += libTAO_RTSched.la

libTAO_RTSched_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_RTSCHED_BUILD_DLL

libTAO_RTSched_la_SOURCES = \
  $(Sched)

libTAO_RTSched_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_RTSched_la_LIBADD = \
  libTAO_Svc_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  RtecSchedulerC.h \
  RtecSchedulerS.h \
  RtecSchedulerS_T.h \
  Runtime_Scheduler.h \
  Sched/Config_Scheduler.h \
  Sched/DynSched.h \
  Sched/Reconfig_Sched_Utils.h \
  Sched/Reconfig_Sched_Utils_T.h \
  Sched/Reconfig_Scheduler.h \
  Sched/Reconfig_Scheduler_T.h \
  Sched/SchedEntry.h \
  Sched/Scheduler.h \
  Sched/Scheduler_Generic.h \
  Sched/Strategy_Scheduler.h \
  Scheduler_Factory.h \
  Scheduler_Utilities.h \
  RtecSchedulerC.inl \
  RtecSchedulerS.inl \
  RtecSchedulerS_T.inl \
  Runtime_Scheduler.i \
  Sched/Config_Scheduler.i \
  Sched/DynSched.i \
  Sched/Reconfig_Sched_Utils.i \
  Sched/Reconfig_Sched_Utils_T.i \
  Sched/Reconfig_Scheduler.i \
  Sched/Reconfig_Scheduler_T.i \
  Sched/SchedEntry.i \
  Sched/Scheduler.i \
  Sched/Scheduler_Generic.i \
  Sched/Strategy_Scheduler.i \
  Scheduler_Factory.i \
  Scheduler_Utilities.i \
  RtecSchedulerS_T.cpp \
  Sched/Reconfig_Sched_Utils_T.cpp \
  Sched/Reconfig_Scheduler_T.cpp \
  RtecScheduler.idl

EXTRA_DIST += \
  RTSched.rc

## Makefile.RTOLDEvent.am 

if BUILD_CORBA_MESSAGING

RTOLDEvent = \
  Event/BCU.cpp \
  Event/Dispatching_Modules.cpp \
  Event/Event_Channel.cpp \
  Event/Event_Manip.cpp \
  Event/GPlot_File.cpp \
  Event/Local_ESTypes.cpp \
  Event/Memory_Pools.cpp \
  Event/Module_Factory.cpp \
  Event/RT_Task.cpp \
  Event/ReactorTask.cpp \
  Event/Timer_Module.cpp

lib_LTLIBRARIES += libTAO_RTOLDEvent.la

libTAO_RTOLDEvent_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_RTOLDEVENT_BUILD_DLL

libTAO_RTOLDEvent_la_SOURCES = \
  $(RTOLDEvent)

libTAO_RTOLDEvent_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_RTOLDEvent_la_LIBADD = \
  libTAO_RTSched.la \
  libTAO_CosNaming.la \
  libTAO_RTEvent.la \
  libTAO_Svc_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  Event/BCU.h \
  Event/Dispatching_Modules.h \
  Event/Event_Channel.h \
  Event/Event_Manip.h \
  Event/GPlot_File.h \
  Event/Local_ESTypes.h \
  Event/Memory_Pools.h \
  Event/Module_Factory.h \
  Event/RT_Task.h \
  Event/ReactorTask.h \
  Event/Timer_Module.h \
  Event/Dispatching_Modules.i \
  Event/Event_Channel.i \
  Event/Event_Manip.i \
  Event/GPlot_File.i \
  Event/Local_ESTypes.i \
  Event/Memory_Pools.i \
  Event/RT_Task.i \
  Event/Timer_Module.i

endif BUILD_CORBA_MESSAGING

EXTRA_DIST += \
  RTOldEvent.rc

## Makefile.RTEventLogAdmin.am 

if BUILD_CORBA_MESSAGING

BUILT_SOURCES += \
  RTEventLogAdminS_T.cpp \
  RTEventLogAdminC.inl \
  RTEventLogAdminS.inl \
  RTEventLogAdminS_T.inl \
  RTEventLogAdminC.cpp \
  RTEventLogAdminS.cpp \
  RTEventLogAdminC.h \
  RTEventLogAdminS.h \
  RTEventLogAdminS_T.h

CLEANFILES += \
  RTEventLogAdminS_T.cpp \
  RTEventLogAdminC.inl \
  RTEventLogAdminS.inl \
  RTEventLogAdminS_T.inl \
  RTEventLogAdminC.cpp \
  RTEventLogAdminS.cpp \
  RTEventLogAdminC.h \
  RTEventLogAdminS.h \
  RTEventLogAdminS_T.h \
  RTEventLogAdmin-stamp

RTEventLogAdminS_T.cpp RTEventLogAdminC.inl RTEventLogAdminS.inl RTEventLogAdminS_T.inl RTEventLogAdminC.cpp RTEventLogAdminS.cpp RTEventLogAdminC.h RTEventLogAdminS.h RTEventLogAdminS_T.h: RTEventLogAdmin-stamp

RTEventLogAdmin-stamp: $(srcdir)/RTEventLogAdmin.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_RTEventLog_Export -Wb,export_include=orbsvcs/Log/rteventlog_export.h $(srcdir)/RTEventLogAdmin.idl
	@touch $@

RTEventLog = \
  Log/RTEventLogConsumer.cpp \
  Log/RTEventLogFactory_i.cpp \
  Log/RTEventLogNotification.cpp \
  Log/RTEventLog_i.cpp \
  RTEventLogAdminC.cpp \
  RTEventLogAdminS.cpp

lib_LTLIBRARIES += libTAO_RTEventLogAdmin.la

libTAO_RTEventLogAdmin_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_RTEVENTLOG_BUILD_DLL

libTAO_RTEventLogAdmin_la_SOURCES = \
  $(RTEventLog)

libTAO_RTEventLogAdmin_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_RTEventLogAdmin_la_LIBADD = \
  libTAO_DsLogAdmin_Serv.la \
  libTAO_ETCL.la \
  $(TAO_BUILDDIR)/tao/libTAO_DynamicAny.la \
  libTAO_DsLogAdmin_Skel.la \
  libTAO_DsLogAdmin.la \
  libTAO_RTOLDEvent.la \
  libTAO_RTSched.la \
  libTAO_RTEvent.la \
  libTAO_Svc_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  Log/RTEventLogConsumer.h \
  Log/RTEventLogFactory_i.h \
  Log/RTEventLogNotification.h \
  Log/RTEventLog_i.h \
  RTEventLogAdminC.h \
  RTEventLogAdminS.h \
  RTEventLogAdminS_T.h \
  RTEventLogAdminC.inl \
  RTEventLogAdminS.inl \
  RTEventLogAdminS_T.inl \
  RTEventLogAdminS_T.cpp \
  RTEventLogAdmin.idl

endif BUILD_CORBA_MESSAGING

## Makefile.RTKokyuEvent.am 

if BUILD_CORBA_MESSAGING

RTKokyuEvent = \
  Event/EC_Kokyu_Dispatching.cpp \
  Event/EC_Kokyu_Factory.cpp \
  Event/EC_Kokyu_Filter.cpp \
  Event/EC_Kokyu_Filter_Builder.cpp \
  Event/EC_Kokyu_Scheduling.cpp

lib_LTLIBRARIES += libTAO_RTKokyuEvent.la

libTAO_RTKokyuEvent_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -I$(ACE_ROOT)/Kokyu \
  -DTAO_RTKOKYUEVENT_BUILD_DLL

libTAO_RTKokyuEvent_la_SOURCES = \
  $(RTKokyuEvent)

libTAO_RTKokyuEvent_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_RTKokyuEvent_la_LIBADD = \
  $(ACE_BUILDDIR)/Kokyu/libKokyu.la \
  libTAO_RTSched.la \
  libTAO_CosNaming.la \
  libTAO_RTEvent.la \
  libTAO_Svc_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  Event/EC_Kokyu_Dispatching.h \
  Event/EC_Kokyu_Factory.h \
  Event/EC_Kokyu_Filter.h \
  Event/EC_Kokyu_Filter_Builder.h \
  Event/EC_Kokyu_Scheduling.h \
  Event/EC_Kokyu_Dispatching.i \
  Event/EC_Kokyu_Factory.i \
  Event/EC_Kokyu_Filter.i \
  Event/EC_Kokyu_Filter_Builder.i \
  Event/EC_Kokyu_Scheduling.i

endif BUILD_CORBA_MESSAGING

## Makefile.RTSchedEvent.am 

if BUILD_CORBA_MESSAGING

RTSchedEvent = \
  Event/EC_Gateway_Sched.cpp \
  Event/EC_Priority_Dispatching.cpp \
  Event/EC_Priority_Scheduling.cpp \
  Event/EC_Sched_Factory.cpp \
  Event/EC_Sched_Filter.cpp \
  Event/EC_Sched_Filter_Builder.cpp

lib_LTLIBRARIES += libTAO_RTSchedEvent.la

libTAO_RTSchedEvent_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_RTSCHEDEVENT_BUILD_DLL

libTAO_RTSchedEvent_la_SOURCES = \
  $(RTSchedEvent)

libTAO_RTSchedEvent_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_RTSchedEvent_la_LIBADD = \
  libTAO_RTSched.la \
  libTAO_CosNaming.la \
  libTAO_RTEvent.la \
  libTAO_Svc_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  Event/EC_Gateway_Sched.h \
  Event/EC_Priority_Dispatching.h \
  Event/EC_Priority_Scheduling.h \
  Event/EC_Sched_Factory.h \
  Event/EC_Sched_Filter.h \
  Event/EC_Sched_Filter_Builder.h \
  Event/EC_Priority_Dispatching.i \
  Event/EC_Priority_Scheduling.i \
  Event/EC_Sched_Factory.i \
  Event/EC_Sched_Filter.i \
  Event/EC_Sched_Filter_Builder.i

endif BUILD_CORBA_MESSAGING

EXTRA_DIST += \
  RTSchedEvent.rc

## Makefile.RT_Notification.am 

if BUILD_RT_CORBA
if !BUILD_MINIMUM_CORBA

RT_Notify = \
  Notify/RT_Builder.cpp \
  Notify/RT_Factory.cpp \
  Notify/RT_Notify_Service.cpp \
  Notify/RT_POA_Helper.cpp \
  Notify/RT_Properties.cpp \
  Notify/Structured/RT_StructuredProxyPushSupplier.cpp

lib_LTLIBRARIES += libTAO_RT_Notification.la

libTAO_RT_Notification_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_HAS_TYPED_EVENT_CHANNEL \
  -DTAO_RT_NOTIFY_BUILD_DLL

libTAO_RT_Notification_la_SOURCES = \
  $(RT_Notify)

libTAO_RT_Notification_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_RT_Notification_la_LIBADD = \
  libTAO_CosNotification_Serv.la \
  $(TAO_BUILDDIR)/tao/libTAO_DynamicAny.la \
  libTAO_ETCL.la \
  libTAO_CosNotification_Skel.la \
  libTAO_CosEvent_Skel.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  libTAO_CosNotification.la \
  libTAO_CosEvent.la \
  $(TAO_BUILDDIR)/tao/libTAO_RTCORBA.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  Notify/RT_Builder.h \
  Notify/RT_Factory.h \
  Notify/RT_Notify_Service.h \
  Notify/RT_POA_Helper.h \
  Notify/RT_Properties.h \
  Notify/Structured/RT_StructuredProxyPushSupplier.h \
  Notify/RT_Builder.inl \
  Notify/RT_Factory.inl \
  Notify/RT_Notify_Service.inl \
  Notify/RT_POA_Helper.inl \
  Notify/RT_Properties.inl \
  Notify/Structured/RT_StructuredProxyPushSupplier.inl

endif !BUILD_MINIMUM_CORBA
endif BUILD_RT_CORBA

EXTRA_DIST += \
  RT_Notification.rc

## Makefile.Security.am 

if BUILD_CORBA_MESSAGING
if BUILD_INTERCEPTORS

BUILT_SOURCES += \
  CSIC.inl \
  CSIS.inl \
  CSIS_T.inl \
  CSIC.cpp \
  CSIC.h \
  CSIS.h \
  CSIS_T.h

CLEANFILES += \
  CSIC.inl \
  CSIS.inl \
  CSIS_T.inl \
  CSIC.cpp \
  CSIC.h \
  CSIS.h \
  CSIS_T.h \
  CSI-stamp

CSIC.inl CSIS.inl CSIS_T.inl CSIC.cpp CSIC.h CSIS.h CSIS_T.h: CSI-stamp

CSI-stamp: $(srcdir)/CSI.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_Security_Export -Wb,export_include=orbsvcs/Security/security_export.h $(srcdir)/CSI.idl
	@touch $@

BUILT_SOURCES += \
  CSIIOPC.inl \
  CSIIOPS.inl \
  CSIIOPS_T.inl \
  CSIIOPC.cpp \
  CSIIOPC.h \
  CSIIOPS.h \
  CSIIOPS_T.h

CLEANFILES += \
  CSIIOPC.inl \
  CSIIOPS.inl \
  CSIIOPS_T.inl \
  CSIIOPC.cpp \
  CSIIOPC.h \
  CSIIOPS.h \
  CSIIOPS_T.h \
  CSIIOP-stamp

CSIIOPC.inl CSIIOPS.inl CSIIOPS_T.inl CSIIOPC.cpp CSIIOPC.h CSIIOPS.h CSIIOPS_T.h: CSIIOP-stamp

CSIIOP-stamp: $(srcdir)/CSIIOP.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_Security_Export -Wb,export_include=orbsvcs/Security/security_export.h $(srcdir)/CSIIOP.idl
	@touch $@

BUILT_SOURCES += \
  GSSUPC.inl \
  GSSUPS.inl \
  GSSUPS_T.inl \
  GSSUPC.cpp \
  GSSUPC.h \
  GSSUPS.h \
  GSSUPS_T.h

CLEANFILES += \
  GSSUPC.inl \
  GSSUPS.inl \
  GSSUPS_T.inl \
  GSSUPC.cpp \
  GSSUPC.h \
  GSSUPS.h \
  GSSUPS_T.h \
  GSSUP-stamp

GSSUPC.inl GSSUPS.inl GSSUPS_T.inl GSSUPC.cpp GSSUPC.h GSSUPS.h GSSUPS_T.h: GSSUP-stamp

GSSUP-stamp: $(srcdir)/GSSUP.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_Security_Export -Wb,export_include=orbsvcs/Security/security_export.h $(srcdir)/GSSUP.idl
	@touch $@

BUILT_SOURCES += \
  SecurityS_T.cpp \
  SecurityC.inl \
  SecurityS.inl \
  SecurityS_T.inl \
  SecurityC.cpp \
  SecurityS.cpp \
  SecurityC.h \
  SecurityS.h \
  SecurityS_T.h

CLEANFILES += \
  SecurityS_T.cpp \
  SecurityC.inl \
  SecurityS.inl \
  SecurityS_T.inl \
  SecurityC.cpp \
  SecurityS.cpp \
  SecurityC.h \
  SecurityS.h \
  SecurityS_T.h \
  Security-stamp

SecurityS_T.cpp SecurityC.inl SecurityS.inl SecurityS_T.inl SecurityC.cpp SecurityS.cpp SecurityC.h SecurityS.h SecurityS_T.h: Security-stamp

Security-stamp: $(srcdir)/Security.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_Security_Export -Wb,export_include=orbsvcs/Security/security_export.h $(srcdir)/Security.idl
	@touch $@

BUILT_SOURCES += \
  SecurityLevel1S_T.cpp \
  SecurityLevel1C.inl \
  SecurityLevel1S.inl \
  SecurityLevel1S_T.inl \
  SecurityLevel1C.cpp \
  SecurityLevel1S.cpp \
  SecurityLevel1C.h \
  SecurityLevel1S.h \
  SecurityLevel1S_T.h

CLEANFILES += \
  SecurityLevel1S_T.cpp \
  SecurityLevel1C.inl \
  SecurityLevel1S.inl \
  SecurityLevel1S_T.inl \
  SecurityLevel1C.cpp \
  SecurityLevel1S.cpp \
  SecurityLevel1C.h \
  SecurityLevel1S.h \
  SecurityLevel1S_T.h \
  SecurityLevel1-stamp

SecurityLevel1S_T.cpp SecurityLevel1C.inl SecurityLevel1S.inl SecurityLevel1S_T.inl SecurityLevel1C.cpp SecurityLevel1S.cpp SecurityLevel1C.h SecurityLevel1S.h SecurityLevel1S_T.h: SecurityLevel1-stamp

SecurityLevel1-stamp: $(srcdir)/SecurityLevel1.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_Security_Export -Wb,export_include=orbsvcs/Security/security_export.h $(srcdir)/SecurityLevel1.idl
	@touch $@

BUILT_SOURCES += \
  SecurityLevel2S_T.cpp \
  SecurityLevel2C.inl \
  SecurityLevel2S.inl \
  SecurityLevel2S_T.inl \
  SecurityLevel2C.cpp \
  SecurityLevel2S.cpp \
  SecurityLevel2C.h \
  SecurityLevel2S.h \
  SecurityLevel2S_T.h

CLEANFILES += \
  SecurityLevel2S_T.cpp \
  SecurityLevel2C.inl \
  SecurityLevel2S.inl \
  SecurityLevel2S_T.inl \
  SecurityLevel2C.cpp \
  SecurityLevel2S.cpp \
  SecurityLevel2C.h \
  SecurityLevel2S.h \
  SecurityLevel2S_T.h \
  SecurityLevel2-stamp

SecurityLevel2S_T.cpp SecurityLevel2C.inl SecurityLevel2S.inl SecurityLevel2S_T.inl SecurityLevel2C.cpp SecurityLevel2S.cpp SecurityLevel2C.h SecurityLevel2S.h SecurityLevel2S_T.h: SecurityLevel2-stamp

SecurityLevel2-stamp: $(srcdir)/SecurityLevel2.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_Security_Export -Wb,export_include=orbsvcs/Security/security_export.h $(srcdir)/SecurityLevel2.idl
	@touch $@

BUILT_SOURCES += \
  SecurityLevel3C.inl \
  SecurityLevel3S.inl \
  SecurityLevel3S_T.inl \
  SecurityLevel3C.cpp \
  SecurityLevel3C.h \
  SecurityLevel3S.h \
  SecurityLevel3S_T.h

CLEANFILES += \
  SecurityLevel3C.inl \
  SecurityLevel3S.inl \
  SecurityLevel3S_T.inl \
  SecurityLevel3C.cpp \
  SecurityLevel3C.h \
  SecurityLevel3S.h \
  SecurityLevel3S_T.h \
  SecurityLevel3-stamp

SecurityLevel3C.inl SecurityLevel3S.inl SecurityLevel3S_T.inl SecurityLevel3C.cpp SecurityLevel3C.h SecurityLevel3S.h SecurityLevel3S_T.h: SecurityLevel3-stamp

SecurityLevel3-stamp: $(srcdir)/SecurityLevel3.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_Security_Export -Wb,export_include=orbsvcs/Security/security_export.h $(srcdir)/SecurityLevel3.idl
	@touch $@

Security = \
  CSIC.cpp \
  CSIIOPC.cpp \
  GSSUPC.cpp \
  Security/CSI_Utils.cpp \
  Security/SL2_EstablishTrustPolicy.cpp \
  Security/SL2_QOPPolicy.cpp \
  Security/SL3_ContextEstablishmentPolicy.cpp \
  Security/SL3_CredentialsAcquirerFactory.cpp \
  Security/SL3_CredentialsCurator.cpp \
  Security/SL3_ObjectCredentialsPolicy.cpp \
  Security/SL3_PolicyFactory.cpp \
  Security/SL3_SecurityCurrent.cpp \
  Security/SL3_SecurityCurrent_Impl.cpp \
  Security/SL3_SecurityManager.cpp \
  Security/Security_Current.cpp \
  Security/Security_Current_Impl.cpp \
  Security/Security_ORBInitializer.cpp \
  Security/Security_PolicyFactory.cpp \
  SecurityC.cpp \
  SecurityLevel1C.cpp \
  SecurityLevel1S.cpp \
  SecurityLevel2C.cpp \
  SecurityLevel2S.cpp \
  SecurityLevel3C.cpp \
  SecurityS.cpp

lib_LTLIBRARIES += libTAO_Security.la

libTAO_Security_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -DTAO_SECURITY_BUILD_DLL

libTAO_Security_la_SOURCES = \
  $(Security)

libTAO_Security_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_Security_la_LIBADD = \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  CSIC.h \
  CSIIOPC.h \
  CSIIOPS.h \
  CSIIOPS_T.h \
  CSIS.h \
  CSIS_T.h \
  GSSUPC.h \
  GSSUPS.h \
  GSSUPS_T.h \
  Security/CSI_Utils.h \
  Security/SL2_EstablishTrustPolicy.h \
  Security/SL2_QOPPolicy.h \
  Security/SL3_ContextEstablishmentPolicy.h \
  Security/SL3_CredentialsAcquirerFactory.h \
  Security/SL3_CredentialsCurator.h \
  Security/SL3_ObjectCredentialsPolicy.h \
  Security/SL3_PolicyFactory.h \
  Security/SL3_SecurityCurrent.h \
  Security/SL3_SecurityCurrent_Impl.h \
  Security/SL3_SecurityManager.h \
  Security/Security_Current.h \
  Security/Security_Current_Impl.h \
  Security/Security_ORBInitializer.h \
  Security/Security_PolicyFactory.h \
  SecurityC.h \
  SecurityLevel1C.h \
  SecurityLevel1S.h \
  SecurityLevel1S_T.h \
  SecurityLevel2C.h \
  SecurityLevel2S.h \
  SecurityLevel2S_T.h \
  SecurityLevel3C.h \
  SecurityLevel3S.h \
  SecurityLevel3S_T.h \
  SecurityS.h \
  SecurityS_T.h \
  CSIC.inl \
  CSIIOPC.inl \
  CSIIOPS.inl \
  CSIIOPS_T.inl \
  CSIS.inl \
  CSIS_T.inl \
  GSSUPC.inl \
  GSSUPS.inl \
  GSSUPS_T.inl \
  Security/SL3_SecurityCurrent.inl \
  Security/Security_Current.inl \
  SecurityC.inl \
  SecurityLevel1C.inl \
  SecurityLevel1S.inl \
  SecurityLevel1S_T.inl \
  SecurityLevel2C.inl \
  SecurityLevel2S.inl \
  SecurityLevel2S_T.inl \
  SecurityLevel3C.inl \
  SecurityLevel3S.inl \
  SecurityLevel3S_T.inl \
  SecurityS.inl \
  SecurityS_T.inl \
  SecurityLevel1S_T.cpp \
  SecurityLevel2S_T.cpp \
  SecurityS_T.cpp \
  CSI.idl \
  CSIIOP.idl \
  GSSUP.idl \
  Security.idl \
  SecurityLevel1.idl \
  SecurityLevel2.idl \
  SecurityLevel3.idl

endif BUILD_INTERCEPTORS
endif BUILD_CORBA_MESSAGING

EXTRA_DIST += \
  Security.rc

## Makefile.SSLIOP.am 

if BUILD_INTERCEPTORS
if BUILD_CORBA_MESSAGING
if BUILD_SSL

BUILT_SOURCES += \
  SSLIOPS_T.cpp \
  SSLIOPC.inl \
  SSLIOPS.inl \
  SSLIOPS_T.inl \
  SSLIOPC.cpp \
  SSLIOPC.h \
  SSLIOPS.h \
  SSLIOPS_T.h

CLEANFILES += \
  SSLIOPS_T.cpp \
  SSLIOPC.inl \
  SSLIOPS.inl \
  SSLIOPS_T.inl \
  SSLIOPC.cpp \
  SSLIOPC.h \
  SSLIOPS.h \
  SSLIOPS_T.h \
  SSLIOP-stamp

SSLIOPS_T.cpp SSLIOPC.inl SSLIOPS.inl SSLIOPS_T.inl SSLIOPC.cpp SSLIOPC.h SSLIOPS.h SSLIOPS_T.h: SSLIOP-stamp

SSLIOP-stamp: $(srcdir)/SSLIOP.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -Wb,export_macro=TAO_SSLIOP_Export -Wb,export_include=orbsvcs/SSLIOP/SSLIOP_Export.h $(srcdir)/SSLIOP.idl
	@touch $@

SSLIOP = \
  SSLIOP/IIOP_SSL_Acceptor.cpp \
  SSLIOP/IIOP_SSL_Connection_Handler.cpp \
  SSLIOP/IIOP_SSL_Connector.cpp \
  SSLIOP/IIOP_SSL_Transport.cpp \
  SSLIOP/SSLIOP_Accept_Strategy.cpp \
  SSLIOP/SSLIOP_Acceptor.cpp \
  SSLIOP/SSLIOP_ClientCredentials.cpp \
  SSLIOP/SSLIOP_Connection_Handler.cpp \
  SSLIOP/SSLIOP_Connector.cpp \
  SSLIOP/SSLIOP_Credentials.cpp \
  SSLIOP/SSLIOP_CredentialsAcquirer.cpp \
  SSLIOP/SSLIOP_CredentialsAcquirerFactory.cpp \
  SSLIOP/SSLIOP_Current.cpp \
  SSLIOP/SSLIOP_Current_Impl.cpp \
  SSLIOP/SSLIOP_EVP_PKEY.cpp \
  SSLIOP/SSLIOP_Endpoint.cpp \
  SSLIOP/SSLIOP_Factory.cpp \
  SSLIOP/SSLIOP_Invocation_Interceptor.cpp \
  SSLIOP/SSLIOP_ORBInitializer.cpp \
  SSLIOP/SSLIOP_OwnCredentials.cpp \
  SSLIOP/SSLIOP_Profile.cpp \
  SSLIOP/SSLIOP_SSL.cpp \
  SSLIOP/SSLIOP_TargetCredentials.cpp \
  SSLIOP/SSLIOP_Transport.cpp \
  SSLIOP/SSLIOP_Util.cpp \
  SSLIOP/SSLIOP_X509.cpp \
  SSLIOP/ssl_endpoints.cpp \
  SSLIOPC.cpp

lib_LTLIBRARIES += libTAO_SSLIOP.la

libTAO_SSLIOP_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs \
  -I$(SSL_ROOT)/include \
  -I/usr/kerberos/include \
  -DACE_HAS_SSL=1 \
  -DTAO_SSLIOP_BUILD_DLL

libTAO_SSLIOP_la_SOURCES = \
  $(SSLIOP)

libTAO_SSLIOP_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_SSLIOP_la_LIBADD = \
  $(ACE_BUILDDIR)/ace/SSL/libACE_SSL.la \
  libTAO_Security.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la \
  -lssl \
  -lcrypto

nobase_include_HEADERS += \
  SSLIOP/IIOP_SSL_Acceptor.h \
  SSLIOP/IIOP_SSL_Connection_Handler.h \
  SSLIOP/IIOP_SSL_Connector.h \
  SSLIOP/IIOP_SSL_Transport.h \
  SSLIOP/SSLIOP_Accept_Strategy.h \
  SSLIOP/SSLIOP_Acceptor.h \
  SSLIOP/SSLIOP_ClientCredentials.h \
  SSLIOP/SSLIOP_Connection_Handler.h \
  SSLIOP/SSLIOP_Connector.h \
  SSLIOP/SSLIOP_Credentials.h \
  SSLIOP/SSLIOP_CredentialsAcquirer.h \
  SSLIOP/SSLIOP_CredentialsAcquirerFactory.h \
  SSLIOP/SSLIOP_Current.h \
  SSLIOP/SSLIOP_Current_Impl.h \
  SSLIOP/SSLIOP_EVP_PKEY.h \
  SSLIOP/SSLIOP_Endpoint.h \
  SSLIOP/SSLIOP_Factory.h \
  SSLIOP/SSLIOP_Invocation_Interceptor.h \
  SSLIOP/SSLIOP_ORBInitializer.h \
  SSLIOP/SSLIOP_OwnCredentials.h \
  SSLIOP/SSLIOP_Profile.h \
  SSLIOP/SSLIOP_SSL.h \
  SSLIOP/SSLIOP_TargetCredentials.h \
  SSLIOP/SSLIOP_Transport.h \
  SSLIOP/SSLIOP_Util.h \
  SSLIOP/SSLIOP_X509.h \
  SSLIOP/ssl_endpoints.h \
  SSLIOPC.h \
  SSLIOPS.h \
  SSLIOPS_T.h \
  SSLIOP/IIOP_SSL_Connection_Handler.inl \
  SSLIOP/SSLIOP_Acceptor.i \
  SSLIOP/SSLIOP_Connection_Handler.i \
  SSLIOP/SSLIOP_Credentials.inl \
  SSLIOP/SSLIOP_Current.inl \
  SSLIOP/SSLIOP_Current_Impl.inl \
  SSLIOP/SSLIOP_Endpoint.i \
  SSLIOP/SSLIOP_Profile.i \
  SSLIOP/ssl_endpoints.i \
  SSLIOPC.inl \
  SSLIOPS.inl \
  SSLIOPS_T.inl \
  SSLIOP.idl

endif BUILD_SSL
endif BUILD_CORBA_MESSAGING
endif BUILD_INTERCEPTORS

EXTRA_DIST += \
  SSLIOP.rc

## Clean up template repositories, etc.
clean-local:
	-rm -f *~ *.bak *.rpo *.sym lib*.*_pure_* core core.*
	-rm -f gcctemp.c gcctemp so_locations *.ics
	-rm -rf cxx_repository ptrepository ti_files
	-rm -rf templateregistry ir.out
	-rm -rf ptrepository SunWS_cache Templates.DB
