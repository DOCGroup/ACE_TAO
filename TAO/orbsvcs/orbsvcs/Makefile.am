##  Process this file with automake to create Makefile.in

ACE_ROOT = $(top_srcdir)/..
ACE_BUILDDIR = $(top_builddir)/..
TAO_ROOT = $(top_srcdir)
TAO_BUILDDIR = $(top_builddir)
TAO_IDL = ACE_ROOT=$(ACE_ROOT) TAO_ROOT=$(TAO_ROOT) $(top_builddir)/TAO_IDL/tao_idl
TAO_IDLFLAGS = -Ge 1 -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -I$(TAO_ROOT) -I$(srcdir) -g $(ACE_BUILDDIR)/apps/gperf/src/gperf

## Makefile.Svc_Utils.am 

BUILT_SOURCES = \
  TimeBaseS_T.cpp \
  TimeBaseC.cpp \
  TimeBaseS.cpp \
  TimeBaseC.h \
  TimeBaseS.h \
  TimeBaseS_T.h \
  TimeBaseC.inl \
  TimeBaseS.inl \
  TimeBaseS_T.inl

CLEANFILES = \
  TimeBaseS_T.cpp \
  TimeBaseC.cpp \
  TimeBaseS.cpp \
  TimeBaseC.h \
  TimeBaseS.h \
  TimeBaseS_T.h \
  TimeBaseC.inl \
  TimeBaseS.inl \
  TimeBaseS_T.inl

TimeBaseS_T.cpp TimeBaseC.cpp TimeBaseS.cpp TimeBaseC.h TimeBaseS.h TimeBaseS_T.h TimeBaseC.inl TimeBaseS.inl TimeBaseS_T.inl: TimeBase.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_Svc_Utils_Export -Wb,export_include=svc_utils_export.h $(srcdir)/TimeBase.idl

BUILT_SOURCES += \
  RtecBaseS_T.cpp \
  RtecBaseC.cpp \
  RtecBaseS.cpp \
  RtecBaseC.h \
  RtecBaseS.h \
  RtecBaseS_T.h \
  RtecBaseC.inl \
  RtecBaseS.inl \
  RtecBaseS_T.inl

CLEANFILES += \
  RtecBaseS_T.cpp \
  RtecBaseC.cpp \
  RtecBaseS.cpp \
  RtecBaseC.h \
  RtecBaseS.h \
  RtecBaseS_T.h \
  RtecBaseC.inl \
  RtecBaseS.inl \
  RtecBaseS_T.inl

RtecBaseS_T.cpp RtecBaseC.cpp RtecBaseS.cpp RtecBaseC.h RtecBaseS.h RtecBaseS_T.h RtecBaseC.inl RtecBaseS.inl RtecBaseS_T.inl: RtecBase.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_Svc_Utils_Export -Wb,export_include=svc_utils_export.h $(srcdir)/RtecBase.idl

lib_LTLIBRARIES = libTAO_Svc_Utils.la

libTAO_Svc_Utils_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_SVC_UTILS_BUILD_DLL

libTAO_Svc_Utils_la_SOURCES = \
  TimeBaseC.cpp \
  TimeBaseS.cpp \
  RtecBaseC.cpp \
  RtecBaseS.cpp \
  Time_Utilities.cpp \
  IOR_Multicast.cpp

libTAO_Svc_Utils_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_Svc_Utils_la_LIBADD = \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS = \
  TimeBaseS_T.h \
  Channel_Clients_T.h \
  TimeBaseC.h \
  TimeBaseS.h \
  RtecBaseS_T.h \
  RtecBaseC.h \
  RtecBaseS.h \
  Time_Utilities.h \
  IOR_Multicast.h \
  TimeBaseS_T.inl \
  Channel_Clients_T.i \
  TimeBaseC.inl \
  TimeBaseS.inl \
  RtecBaseS_T.inl \
  RtecBaseC.inl \
  RtecBaseS.inl \
  Time_Utilities.i \
  TimeBaseS_T.cpp \
  RtecBaseS_T.cpp \
  Channel_Clients_T.cpp


## Makefile.CosNaming.am 

BUILT_SOURCES += \
  CosNamingS_T.cpp \
  CosNamingC.cpp \
  CosNamingS.cpp \
  CosNamingC.h \
  CosNamingS.h \
  CosNamingS_T.h \
  CosNamingC.inl \
  CosNamingS.inl \
  CosNamingS_T.inl

CLEANFILES += \
  CosNamingS_T.cpp \
  CosNamingC.cpp \
  CosNamingS.cpp \
  CosNamingC.h \
  CosNamingS.h \
  CosNamingS_T.h \
  CosNamingC.inl \
  CosNamingS.inl \
  CosNamingS_T.inl

CosNamingS_T.cpp CosNamingC.cpp CosNamingS.cpp CosNamingC.h CosNamingS.h CosNamingS_T.h CosNamingC.inl CosNamingS.inl CosNamingS_T.inl: CosNaming.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_Naming_Export -Wb,export_include=Naming/naming_export.h $(srcdir)/CosNaming.idl

Naming = \
  CosNamingC.cpp \
  CosNamingS.cpp \
  Naming/Entries.cpp \
  Naming/Flat_File_Persistence.cpp \
  Naming/Hash_Naming_Context.cpp \
  Naming/Naming_Context_Interface.cpp \
  Naming/Naming_Loader.cpp \
  Naming/Naming_Utils.cpp \
  Naming/Persistent_Context_Index.cpp \
  Naming/Persistent_Entries.cpp \
  Naming/Persistent_Naming_Context.cpp \
  Naming/Storable.cpp \
  Naming/Storable_Naming_Context.cpp \
  Naming/Storable_Naming_Context_Activator.cpp \
  Naming/Transient_Naming_Context.cpp

lib_LTLIBRARIES += libTAO_CosNaming.la

libTAO_CosNaming_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_NAMING_BUILD_DLL

libTAO_CosNaming_la_SOURCES = \
  $(Naming)

libTAO_CosNaming_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_CosNaming_la_LIBADD = \
 $(top_builddir)/tao/libTAO_IORTable.la \
 libTAO_Svc_Utils.la \
 $(top_builddir)/tao/libTAO_Messaging.la \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  Naming/naming_export.h \
  Channel_Clients_T.h \
  CosNamingS_T.h \
  Naming/Transient_Naming_Context.h \
  Naming/Entries.h \
  Naming/Persistent_Context_Index.h \
  Naming/Naming_Loader.h \
  Naming/Naming_Context_Interface.h \
  Naming/Storable_Naming_Context_Activator.h \
  Naming/Naming_Utils.h \
  Naming/Flat_File_Persistence.h \
  Naming/Storable.h \
  Naming/Hash_Naming_Context.h \
  CosNamingC.h \
  CosNamingS.h \
  Naming/Persistent_Naming_Context.h \
  Naming/Storable_Naming_Context.h \
  Naming/Persistent_Entries.h \
  Channel_Clients_T.i \
  CosNamingS_T.inl \
  Naming/Storable.inl \
  CosNamingC.inl \
  CosNamingS.inl \
  CosNamingS_T.cpp \
  Channel_Clients_T.cpp


## Makefile.CosProperty.am 

BUILT_SOURCES += \
  CosPropertyServiceS_T.cpp \
  CosPropertyServiceC.cpp \
  CosPropertyServiceS.cpp \
  CosPropertyServiceC.h \
  CosPropertyServiceS.h \
  CosPropertyServiceS_T.h \
  CosPropertyServiceC.inl \
  CosPropertyServiceS.inl \
  CosPropertyServiceS_T.inl

CLEANFILES += \
  CosPropertyServiceS_T.cpp \
  CosPropertyServiceC.cpp \
  CosPropertyServiceS.cpp \
  CosPropertyServiceC.h \
  CosPropertyServiceS.h \
  CosPropertyServiceS_T.h \
  CosPropertyServiceC.inl \
  CosPropertyServiceS.inl \
  CosPropertyServiceS_T.inl

CosPropertyServiceS_T.cpp CosPropertyServiceC.cpp CosPropertyServiceS.cpp CosPropertyServiceC.h CosPropertyServiceS.h CosPropertyServiceS_T.h CosPropertyServiceC.inl CosPropertyServiceS.inl CosPropertyServiceS_T.inl: CosPropertyService.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_Property_Export -Wb,export_include=Property/property_export.h $(srcdir)/CosPropertyService.idl

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
  -DTAO_PROPERTY_BUILD_DLL

libTAO_CosProperty_la_SOURCES = \
  $(Property)

libTAO_CosProperty_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_CosProperty_la_LIBADD = \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  CosPropertyServiceS_T.h \
  Channel_Clients_T.h \
  Property/CosPropertyService_i.h \
  CosPropertyServiceC.h \
  CosPropertyServiceS.h \
  CosPropertyServiceS_T.inl \
  Channel_Clients_T.i \
  CosPropertyServiceS_T.cpp \
  Channel_Clients_T.cpp


## Makefile.AV.am 

BUILT_SOURCES += \
  AVStreamsS_T.cpp \
  AVStreamsC.cpp \
  AVStreamsS.cpp \
  AVStreamsC.h \
  AVStreamsS.h \
  AVStreamsS_T.h \
  AVStreamsC.inl \
  AVStreamsS.inl \
  AVStreamsS_T.inl

CLEANFILES += \
  AVStreamsS_T.cpp \
  AVStreamsC.cpp \
  AVStreamsS.cpp \
  AVStreamsC.h \
  AVStreamsS.h \
  AVStreamsS_T.h \
  AVStreamsC.inl \
  AVStreamsS.inl \
  AVStreamsS_T.inl

AVStreamsS_T.cpp AVStreamsC.cpp AVStreamsS.cpp AVStreamsC.h AVStreamsS.h AVStreamsS_T.h AVStreamsC.inl AVStreamsS.inl AVStreamsS_T.inl: AVStreams.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_AV_Export -Wb,export_include=AV/AV_export.h $(srcdir)/AVStreams.idl

BUILT_SOURCES += \
  Null_MediaCtrlS_T.cpp \
  Null_MediaCtrlC.cpp \
  Null_MediaCtrlS.cpp \
  Null_MediaCtrlS_T.h \
  Null_MediaCtrlC.inl \
  Null_MediaCtrlS.inl \
  Null_MediaCtrlS_T.inl

CLEANFILES += \
  Null_MediaCtrlS_T.cpp \
  Null_MediaCtrlC.cpp \
  Null_MediaCtrlS.cpp \
  Null_MediaCtrlS_T.h \
  Null_MediaCtrlC.inl \
  Null_MediaCtrlS.inl \
  Null_MediaCtrlS_T.inl

Null_MediaCtrlS_T.cpp Null_MediaCtrlC.cpp Null_MediaCtrlS.cpp Null_MediaCtrlS_T.h Null_MediaCtrlC.inl Null_MediaCtrlS.inl Null_MediaCtrlS_T.inl: Null_MediaCtrl.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_AV_Export -Wb,export_include=AV/AV_export.h $(srcdir)/Null_MediaCtrl.idl

BUILT_SOURCES += \
  sfpS_T.cpp \
  sfpC.cpp \
  sfpS.cpp \
  sfpC.h \
  sfpS.h \
  sfpS_T.h \
  sfpC.inl \
  sfpS.inl \
  sfpS_T.inl

CLEANFILES += \
  sfpS_T.cpp \
  sfpC.cpp \
  sfpS.cpp \
  sfpC.h \
  sfpS.h \
  sfpS_T.h \
  sfpC.inl \
  sfpS.inl \
  sfpS_T.inl

sfpS_T.cpp sfpC.cpp sfpS.cpp sfpC.h sfpS.h sfpS_T.h sfpC.inl sfpS.inl sfpS_T.inl: sfp.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_AV_Export -Wb,export_include=AV/AV_export.h $(srcdir)/sfp.idl

AV = \
  AVStreamsC.cpp \
  AVStreamsS.cpp \
  Null_MediaCtrlC.cpp \
  Null_MediaCtrlS.cpp \
  sfpC.cpp \
  sfpS.cpp \
  AV/AVStreams_i.cpp \
  AV/AV_Core.cpp \
  AV/Endpoint_Strategy.cpp \
  AV/Fill_ACE_QoS.cpp \
  AV/FlowSpec_Entry.cpp \
  AV/MCast.cpp \
  AV/md5c.cpp \
  AV/Policy.cpp \
  AV/Protocol_Factory.cpp \
  AV/QoS_UDP.cpp \
  AV/RTCP.cpp \
  AV/RTCP_Channel.cpp \
  AV/RTCP_Packet.cpp \
  AV/RTP.cpp \
  AV/TCP.cpp \
  AV/Transport.cpp \
  AV/UDP.cpp \
  AV/default_resource.cpp \
  AV/media_timer.cpp \
  AV/sfp.cpp \
  AV/SCTP_SEQ.cpp

lib_LTLIBRARIES += libTAO_AV.la

libTAO_AV_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_AV_BUILD_DLL

libTAO_AV_la_SOURCES = \
  $(AV)

libTAO_AV_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_AV_la_LIBADD = \
 libTAO_CosNaming.la \
 libTAO_Svc_Utils.la \
 $(top_builddir)/tao/libTAO_Messaging.la \
 $(top_builddir)/tao/libTAO_IORTable.la \
 libTAO_CosProperty.la \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  AV/TCP.h \
  AV/Transport.h \
  sfpC.h \
  sfpS.h \
  AVStreamsC.h \
  AVStreamsS.h \
  Channel_Clients_T.h \
  AV/AVStreams_i.h \
  AV/AV_Core.h \
  AV/AV_export.h \
  AV/Endpoint_Strategy.h \
  AV/Endpoint_Strategy_T.h \
  AV/Fill_ACE_QoS.h \
  AV/FlowSpec_Entry.h \
  AV/Flows_T.h \
  AV/MCast.h \
  AV/md5.h \
  AV/Policy.h \
  AV/Protocol_Factory.h \
  AV/QoS_UDP.h \
  AV/RTCP.h \
  AV/RTCP_Channel.h \
  AV/RTCP_Packet.h \
  AV/RTP.h \
  AV/Resource_Factory.h \
  AV/UDP.h \
  AV/default_resource.h \
  AV/global.h \
  AV/media_timer.h \
  AV/ntp-time.h \
  AV/sfp.h \
  AV/SCTP_SEQ.h \
  Null_MediaCtrlS_T.h \
  AVStreamsS_T.h \
  sfpS_T.h \
  AV/QoS_UDP.i \
  AV/Transport.i \
  sfpC.inl \
  sfpS.inl \
  AV/UDP.i \
  AV/AVStreams_i.i \
  AV/Policy.i \
  AV/FlowSpec_Entry.i \
  AV/MCast.i \
  AVStreamsC.inl \
  AVStreamsS.inl \
  Null_MediaCtrlS_T.inl \
  AVStreamsS_T.inl \
  sfpS_T.inl \
  Channel_Clients_T.i \
  AVStreamsS_T.cpp \
  Null_MediaCtrlS_T.cpp \
  sfpS_T.cpp \
  Channel_Clients_T.cpp \
  AV/Endpoint_Strategy_T.cpp \
  AV/Flows_T.cpp


## Makefile.CosConcurrency.am 

BUILT_SOURCES += \
  CosConcurrencyControlS_T.cpp \
  CosConcurrencyControlC.cpp \
  CosConcurrencyControlS.cpp \
  CosConcurrencyControlC.h \
  CosConcurrencyControlS.h \
  CosConcurrencyControlS_T.h \
  CosConcurrencyControlC.inl \
  CosConcurrencyControlS.inl \
  CosConcurrencyControlS_T.inl

CLEANFILES += \
  CosConcurrencyControlS_T.cpp \
  CosConcurrencyControlC.cpp \
  CosConcurrencyControlS.cpp \
  CosConcurrencyControlC.h \
  CosConcurrencyControlS.h \
  CosConcurrencyControlS_T.h \
  CosConcurrencyControlC.inl \
  CosConcurrencyControlS.inl \
  CosConcurrencyControlS_T.inl

CosConcurrencyControlS_T.cpp CosConcurrencyControlC.cpp CosConcurrencyControlS.cpp CosConcurrencyControlC.h CosConcurrencyControlS.h CosConcurrencyControlS_T.h CosConcurrencyControlC.inl CosConcurrencyControlS.inl CosConcurrencyControlS_T.inl: CosConcurrencyControl.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_Concurrency_Export -Wb,export_include=Concurrency/concurrency_export.h $(srcdir)/CosConcurrencyControl.idl

Concurrency = \
  CosConcurrencyControlC.cpp \
  CosConcurrencyControlS.cpp \
  Concurrency/CC_LockSet.cpp \
  Concurrency/CC_Lock.cpp \
  Concurrency/CC_LockSetFactory.cpp \
  Concurrency/Concurrency_Loader.cpp \
  Concurrency/Concurrency_Utils.cpp

lib_LTLIBRARIES += libTAO_CosConcurrency.la

libTAO_CosConcurrency_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_CONCURRENCY_BUILD_DLL

libTAO_CosConcurrency_la_SOURCES = \
  $(Concurrency)

libTAO_CosConcurrency_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_CosConcurrency_la_LIBADD = \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  Channel_Clients_T.h \
  CosConcurrencyControlS_T.h \
  Concurrency/CC_LockSetFactory.h \
  CosConcurrencyControlC.h \
  CosConcurrencyControlS.h \
  Concurrency/Concurrency_Utils.h \
  Concurrency/CC_LockSet.h \
  Concurrency/Concurrency_Loader.h \
  Concurrency/CC_Lock.h \
  Channel_Clients_T.i \
  CosConcurrencyControlS_T.inl \
  CosConcurrencyControlS_T.cpp \
  Channel_Clients_T.cpp


## Makefile.CosEvent.am 

BUILT_SOURCES += \
  CosTypedEventCommS_T.cpp \
  CosTypedEventCommC.cpp \
  CosTypedEventCommS.cpp \
  CosTypedEventCommC.h \
  CosTypedEventCommS.h \
  CosTypedEventCommS_T.h \
  CosTypedEventCommC.inl \
  CosTypedEventCommS.inl \
  CosTypedEventCommS_T.inl

CLEANFILES += \
  CosTypedEventCommS_T.cpp \
  CosTypedEventCommC.cpp \
  CosTypedEventCommS.cpp \
  CosTypedEventCommC.h \
  CosTypedEventCommS.h \
  CosTypedEventCommS_T.h \
  CosTypedEventCommC.inl \
  CosTypedEventCommS.inl \
  CosTypedEventCommS_T.inl

CosTypedEventCommS_T.cpp CosTypedEventCommC.cpp CosTypedEventCommS.cpp CosTypedEventCommC.h CosTypedEventCommS.h CosTypedEventCommS_T.h CosTypedEventCommC.inl CosTypedEventCommS.inl CosTypedEventCommS_T.inl: CosTypedEventComm.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_Event_Export -Wb,export_include=CosEvent/event_export.h $(srcdir)/CosTypedEventComm.idl

BUILT_SOURCES += \
  CosTypedEventChannelAdminS_T.cpp \
  CosTypedEventChannelAdminC.cpp \
  CosTypedEventChannelAdminS.cpp \
  CosTypedEventChannelAdminC.h \
  CosTypedEventChannelAdminS.h \
  CosTypedEventChannelAdminS_T.h \
  CosTypedEventChannelAdminC.inl \
  CosTypedEventChannelAdminS.inl \
  CosTypedEventChannelAdminS_T.inl

CLEANFILES += \
  CosTypedEventChannelAdminS_T.cpp \
  CosTypedEventChannelAdminC.cpp \
  CosTypedEventChannelAdminS.cpp \
  CosTypedEventChannelAdminC.h \
  CosTypedEventChannelAdminS.h \
  CosTypedEventChannelAdminS_T.h \
  CosTypedEventChannelAdminC.inl \
  CosTypedEventChannelAdminS.inl \
  CosTypedEventChannelAdminS_T.inl

CosTypedEventChannelAdminS_T.cpp CosTypedEventChannelAdminC.cpp CosTypedEventChannelAdminS.cpp CosTypedEventChannelAdminC.h CosTypedEventChannelAdminS.h CosTypedEventChannelAdminS_T.h CosTypedEventChannelAdminC.inl CosTypedEventChannelAdminS.inl CosTypedEventChannelAdminS_T.inl: CosTypedEventChannelAdmin.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_Event_Export -Wb,export_include=CosEvent/event_export.h $(srcdir)/CosTypedEventChannelAdmin.idl

CosEvent = \
  CosTypedEventCommC.cpp \
  CosTypedEventChannelAdminC.cpp \
  CosTypedEventCommS.cpp \
  CosTypedEventChannelAdminS.cpp \
  CosEvent/CEC_TypedEvent.cpp \
  CosEvent/CEC_TypedEventChannel.cpp \
  CosEvent/CEC_TypedProxyPushConsumer.cpp \
  CosEvent/CEC_TypedConsumerAdmin.cpp \
  CosEvent/CEC_TypedSupplierAdmin.cpp \
  CosEvent/CEC_DynamicImplementation.cpp \
  CosEventCommC.cpp \
  CosEventChannelAdminC.cpp \
  CosEventCommS.cpp \
  CosEventChannelAdminS.cpp \
  CosEvent/CEC_ConsumerAdmin.cpp \
  CosEvent/CEC_ConsumerControl.cpp \
  CosEvent/CEC_Default_Factory.cpp \
  CosEvent/CEC_Dispatching.cpp \
  CosEvent/CEC_Dispatching_Task.cpp \
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
  CosEvent/CEC_SupplierControl.cpp

lib_LTLIBRARIES += libTAO_CosEvent.la

libTAO_CosEvent_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_HAS_TYPED_EVENT_CHANNEL \
  -DTAO_EVENT_BUILD_DLL -DTAO_NOTIFY_BUILD_DLL

libTAO_CosEvent_la_SOURCES = \
  $(CosEvent)

libTAO_CosEvent_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_CosEvent_la_LIBADD = \
 libTAO_CosNaming.la \
 libTAO_Svc_Utils.la \
 $(top_builddir)/tao/libTAO_IORTable.la \
 $(top_builddir)/tao/libTAO_Messaging.la \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO_IFR_Client.la \
 $(top_builddir)/tao/libTAO_DynamicInterface.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la 

noinst_HEADERS += \
  CosEvent/CEC_Factory.h \
  CosEvent/CEC_Pulling_Strategy.h \
  CosEvent/CEC_DynamicImplementation.h \
  CosEvent/CEC_ConsumerControl.h \
  CosTypedEventChannelAdminC.h \
  CosTypedEventCommC.h \
  CosTypedEventChannelAdminS.h \
  CosTypedEventCommS.h \
  CosEvent/CEC_Dispatching.h \
  CosEvent/CEC_ConsumerAdmin.h \
  CosEvent/CEC_ProxyPushSupplier.h \
  CosEvent/CEC_ProxyPushConsumer.h \
  CosEventChannelAdminC.h \
  CosEvent/CEC_TypedConsumerAdmin.h \
  CosEventChannelAdminS.h \
  CosEvent/CEC_EventChannel.h \
  CosEventCommC.h \
  CosEventCommS.h \
  CosEvent/CEC_SupplierAdmin.h \
  CosEvent/CEC_ProxyPullSupplier.h \
  CosEvent/CEC_ProxyPullConsumer.h \
  CosEvent/CEC_TypedEvent.h \
  CosEvent/CEC_TypedEventChannel.h \
  CosEvent/CEC_Reactive_SupplierControl.h \
  CosEvent/CEC_TypedSupplierAdmin.h \
  CosEvent/CEC_Default_Factory.h \
  CosEvent/CEC_TypedProxyPushConsumer.h \
  CosEvent/CEC_SupplierControl.h \
  CosEvent/CEC_Dispatching_Task.h \
  CosEvent/CEC_Reactive_ConsumerControl.h \
  CosEvent/CEC_Event_Loader.h \
  CosEvent/CEC_Reactive_Pulling_Strategy.h \
  CosEvent/CEC_MT_Dispatching.h \
  CosTypedEventChannelAdminS_T.h \
  CosEventChannelAdminS_T.h \
  Channel_Clients_T.h \
  CosTypedEventCommS_T.h \
  CosEventCommS_T.h \
  CosEvent/CEC_Factory.i \
  CosEvent/CEC_Pulling_Strategy.i \
  CosEvent/CEC_DynamicImplementation.i \
  CosEvent/CEC_ConsumerControl.i \
  CosTypedEventChannelAdminC.inl \
  CosTypedEventCommC.inl \
  CosTypedEventChannelAdminS.inl \
  CosTypedEventCommS.inl \
  CosEvent/CEC_Dispatching.i \
  CosEvent/CEC_ConsumerAdmin.i \
  CosEvent/CEC_ProxyPushSupplier.i \
  CosEvent/CEC_ProxyPushConsumer.i \
  CosEventChannelAdminC.inl \
  CosEvent/CEC_TypedConsumerAdmin.i \
  CosEventChannelAdminS.inl \
  CosEvent/CEC_EventChannel.i \
  CosEventCommC.inl \
  CosEventCommS.inl \
  CosEvent/CEC_SupplierAdmin.i \
  CosEvent/CEC_ProxyPullSupplier.i \
  CosEvent/CEC_ProxyPullConsumer.i \
  CosEvent/CEC_TypedEvent.i \
  CosEvent/CEC_TypedEventChannel.i \
  CosEvent/CEC_Reactive_SupplierControl.i \
  CosEvent/CEC_TypedSupplierAdmin.i \
  CosEvent/CEC_Default_Factory.i \
  CosEvent/CEC_TypedProxyPushConsumer.i \
  CosEvent/CEC_SupplierControl.i \
  CosEvent/CEC_Dispatching_Task.i \
  CosEvent/CEC_Reactive_ConsumerControl.i \
  CosEvent/CEC_Reactive_Pulling_Strategy.i \
  CosEvent/CEC_MT_Dispatching.i \
  CosTypedEventChannelAdminS_T.inl \
  CosEventChannelAdminS_T.inl \
  Channel_Clients_T.i \
  CosTypedEventCommS_T.inl \
  CosEventCommS_T.inl \
  CosTypedEventCommS_T.cpp \
  CosTypedEventChannelAdminS_T.cpp \
  CosEventCommS_T.cpp \
  CosEventChannelAdminS_T.cpp \
  Channel_Clients_T.cpp


## Makefile.CosLifeCycle.am 

BUILT_SOURCES += \
  CosLifeCycleS_T.cpp \
  CosLifeCycleC.cpp \
  CosLifeCycleS.cpp \
  CosLifeCycleC.h \
  CosLifeCycleS.h \
  CosLifeCycleS_T.h \
  CosLifeCycleC.inl \
  CosLifeCycleS.inl \
  CosLifeCycleS_T.inl

CLEANFILES += \
  CosLifeCycleS_T.cpp \
  CosLifeCycleC.cpp \
  CosLifeCycleS.cpp \
  CosLifeCycleC.h \
  CosLifeCycleS.h \
  CosLifeCycleS_T.h \
  CosLifeCycleC.inl \
  CosLifeCycleS.inl \
  CosLifeCycleS_T.inl

CosLifeCycleS_T.cpp CosLifeCycleC.cpp CosLifeCycleS.cpp CosLifeCycleC.h CosLifeCycleS.h CosLifeCycleS_T.h CosLifeCycleC.inl CosLifeCycleS.inl CosLifeCycleS_T.inl: CosLifeCycle.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_LifeCycle_Export -Wb,export_include=LifeCycle/lifecycle_export.h $(srcdir)/CosLifeCycle.idl

BUILT_SOURCES += \
  LifeCycleServiceS_T.cpp \
  LifeCycleServiceC.cpp \
  LifeCycleServiceS.cpp \
  LifeCycleServiceC.h \
  LifeCycleServiceS.h \
  LifeCycleServiceS_T.h \
  LifeCycleServiceC.inl \
  LifeCycleServiceS.inl \
  LifeCycleServiceS_T.inl

CLEANFILES += \
  LifeCycleServiceS_T.cpp \
  LifeCycleServiceC.cpp \
  LifeCycleServiceS.cpp \
  LifeCycleServiceC.h \
  LifeCycleServiceS.h \
  LifeCycleServiceS_T.h \
  LifeCycleServiceC.inl \
  LifeCycleServiceS.inl \
  LifeCycleServiceS_T.inl

LifeCycleServiceS_T.cpp LifeCycleServiceC.cpp LifeCycleServiceS.cpp LifeCycleServiceC.h LifeCycleServiceS.h LifeCycleServiceS_T.h LifeCycleServiceC.inl LifeCycleServiceS.inl LifeCycleServiceS_T.inl: LifeCycleService.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_LifeCycle_Export -Wb,export_include=LifeCycle/lifecycle_export.h $(srcdir)/LifeCycleService.idl

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
  -DTAO_LIFECYCLE_BUILD_DLL

libTAO_CosLifeCycle_la_SOURCES = \
  $(LifeCycle)

libTAO_CosLifeCycle_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_CosLifeCycle_la_LIBADD = \
 libTAO_CosNaming.la \
 libTAO_Svc_Utils.la \
 $(top_builddir)/tao/libTAO_Messaging.la \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO_IORTable.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  LifeCycleServiceS_T.h \
  Channel_Clients_T.h \
  CosLifeCycleS_T.h \
  LifeCycleServiceS_T.inl \
  Channel_Clients_T.i \
  CosLifeCycleS_T.inl \
  CosLifeCycleS_T.cpp \
  LifeCycleServiceS_T.cpp \
  Channel_Clients_T.cpp


## Makefile.FTORB_Utils.am 

BUILT_SOURCES += \
  PortableGroup_Simple_DSS_T.cpp \
  PortableGroup_Simple_DSC.cpp \
  PortableGroup_Simple_DSC.h \
  PortableGroup_Simple_DSS.h \
  PortableGroup_Simple_DSS_T.h \
  PortableGroup_Simple_DSC.inl \
  PortableGroup_Simple_DSS.inl \
  PortableGroup_Simple_DSS_T.inl

CLEANFILES += \
  PortableGroup_Simple_DSS_T.cpp \
  PortableGroup_Simple_DSC.cpp \
  PortableGroup_Simple_DSC.h \
  PortableGroup_Simple_DSS.h \
  PortableGroup_Simple_DSS_T.h \
  PortableGroup_Simple_DSC.inl \
  PortableGroup_Simple_DSS.inl \
  PortableGroup_Simple_DSS_T.inl

PortableGroup_Simple_DSS_T.cpp PortableGroup_Simple_DSC.cpp PortableGroup_Simple_DSC.h PortableGroup_Simple_DSS.h PortableGroup_Simple_DSS_T.h PortableGroup_Simple_DSC.inl PortableGroup_Simple_DSS.inl PortableGroup_Simple_DSS_T.inl: PortableGroup_Simple_DS.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,skel_export_include=tao/PortableServer/PolicyS.h $(srcdir)/PortableGroup_Simple_DS.idl

BUILT_SOURCES += \
  FT_CORBA_ORBS_T.cpp \
  FT_CORBA_ORBC.cpp \
  FT_CORBA_ORBS.cpp \
  FT_CORBA_ORBC.h \
  FT_CORBA_ORBS.h \
  FT_CORBA_ORBS_T.h \
  FT_CORBA_ORBC.inl \
  FT_CORBA_ORBS.inl \
  FT_CORBA_ORBS_T.inl

CLEANFILES += \
  FT_CORBA_ORBS_T.cpp \
  FT_CORBA_ORBC.cpp \
  FT_CORBA_ORBS.cpp \
  FT_CORBA_ORBC.h \
  FT_CORBA_ORBS.h \
  FT_CORBA_ORBS_T.h \
  FT_CORBA_ORBC.inl \
  FT_CORBA_ORBS.inl \
  FT_CORBA_ORBS_T.inl

FT_CORBA_ORBS_T.cpp FT_CORBA_ORBC.cpp FT_CORBA_ORBS.cpp FT_CORBA_ORBC.h FT_CORBA_ORBS.h FT_CORBA_ORBS_T.h FT_CORBA_ORBC.inl FT_CORBA_ORBS.inl FT_CORBA_ORBS_T.inl: FT_CORBA_ORB.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,skel_export_include=tao/PortableServer/PolicyS.h -Wb,export_macro=TAO_FT_ORB_Utils_Export -Wb,export_include=FaultTolerance/FT_ORB_Utils_export.h $(srcdir)/FT_CORBA_ORB.idl

FTORB_Utils = \
  PortableGroup_Simple_DSC.cpp \
  FT_CORBA_ORBC.cpp \
  FT_CORBA_ORBS.cpp \
  FaultTolerance/FT_IOGR_Property.cpp

lib_LTLIBRARIES += libTAO_FTORB_Utils.la

libTAO_FTORB_Utils_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_FT_ORB_UTILS_BUILD_DLL

libTAO_FTORB_Utils_la_SOURCES = \
  $(FTORB_Utils)

libTAO_FTORB_Utils_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_FTORB_Utils_la_LIBADD = \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO_IORManip.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  Channel_Clients_T.h \
  PortableGroup_Simple_DSS_T.h \
  FT_CORBA_ORBS_T.h \
  FaultTolerance/FT_IOGR_Property.h \
  FT_CORBA_ORBC.h \
  FT_CORBA_ORBS.h \
  Channel_Clients_T.i \
  PortableGroup_Simple_DSS_T.inl \
  FT_CORBA_ORBS_T.inl \
  FaultTolerance/FT_IOGR_Property.i \
  FT_CORBA_ORBC.inl \
  FT_CORBA_ORBS.inl \
  PortableGroup_Simple_DSS_T.cpp \
  FT_CORBA_ORBS_T.cpp \
  Channel_Clients_T.cpp


## Makefile.PortableGroup.am 

BUILT_SOURCES += \
  miopS_T.cpp \
  miopC.cpp \
  miopS.cpp \
  miopC.h \
  miopS.h \
  miopS_T.h \
  miopC.inl \
  miopS.inl \
  miopS_T.inl

CLEANFILES += \
  miopS_T.cpp \
  miopC.cpp \
  miopS.cpp \
  miopC.h \
  miopS.h \
  miopS_T.h \
  miopC.inl \
  miopS.inl \
  miopS_T.inl

miopS_T.cpp miopC.cpp miopS.cpp miopC.h miopS.h miopS_T.h miopC.inl miopS.inl miopS_T.inl: miop.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_PortableGroup_Export -Wb,export_include=PortableGroup/portablegroup_export.h $(srcdir)/miop.idl

BUILT_SOURCES += \
  PortableGroupS_T.cpp \
  PortableGroupC.cpp \
  PortableGroupS.cpp \
  PortableGroupC.h \
  PortableGroupS.h \
  PortableGroupS_T.h \
  PortableGroupC.inl \
  PortableGroupS.inl \
  PortableGroupS_T.inl

CLEANFILES += \
  PortableGroupS_T.cpp \
  PortableGroupC.cpp \
  PortableGroupS.cpp \
  PortableGroupC.h \
  PortableGroupS.h \
  PortableGroupS_T.h \
  PortableGroupC.inl \
  PortableGroupS.inl \
  PortableGroupS_T.inl

PortableGroupS_T.cpp PortableGroupC.cpp PortableGroupS.cpp PortableGroupC.h PortableGroupS.h PortableGroupS_T.h PortableGroupC.inl PortableGroupS.inl PortableGroupS_T.inl: PortableGroup.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_PortableGroup_Export -Wb,export_include=PortableGroup/portablegroup_export.h -GC $(srcdir)/PortableGroup.idl

PortableGroup = \
  miopC.cpp \
  miopS.cpp \
  PortableGroupC.cpp \
  PortableGroupS.cpp \
  PortableGroup_Simple_DSC.cpp \
  PortableGroup/PG_Default_Property_Validator.cpp \
  PortableGroup/PG_FactoryRegistry.cpp \
  PortableGroup/PG_Factory_Map.cpp \
  PortableGroup/PG_Factory_Set.cpp \
  PortableGroup/PG_GenericFactory.cpp \
  PortableGroup/PG_Group_Factory.cpp \
  PortableGroup/PG_Group_Guard.cpp \
  PortableGroup/PG_Null_Property_Validator.cpp \
  PortableGroup/PG_Location_Equal_To.cpp \
  PortableGroup/PG_Location_Hash.cpp \
  PortableGroup/PG_Location_Map.cpp \
  PortableGroup/PG_MemberInfo.cpp \
  PortableGroup/PG_Object_Group_Manipulator.cpp \
  PortableGroup/PG_ObjectGroupManager.cpp \
  PortableGroup/PG_ObjectGroup_Map.cpp \
  PortableGroup/PG_Object_Group.cpp \
  PortableGroup/PG_Operators.cpp \
  PortableGroup/PG_Properties_Encoder.cpp \
  PortableGroup/PG_Properties_Support.cpp \
  PortableGroup/PG_PropertyManager.cpp \
  PortableGroup/PG_Property_Set.cpp \
  PortableGroup/PG_Property_Utils.cpp \
  PortableGroup/PG_Utils.cpp \
  PortableGroup/POA_Hooks.cpp \
  PortableGroup/PortableGroup_Loader.cpp \
  PortableGroup/PortableGroup_Acceptor_Registry.cpp \
  PortableGroup/PortableGroup_ORBInitializer.cpp \
  PortableGroup/PortableGroup_Request_Dispatcher.cpp \
  PortableGroup/UIPMC_Transport.cpp \
  PortableGroup/Portable_Group_Map.cpp \
  PortableGroup/UIPMC_Acceptor.cpp \
  PortableGroup/UIPMC_Connection_Handler.cpp \
  PortableGroup/UIPMC_Connector.cpp \
  PortableGroup/UIPMC_Endpoint.cpp \
  PortableGroup/UIPMC_Factory.cpp \
  PortableGroup/UIPMC_Message_Block_Data_Iterator.cpp \
  PortableGroup/UIPMC_Profile.cpp \
  PortableGroup/UIPMC_Wait_Never.cpp

lib_LTLIBRARIES += libTAO_PortableGroup.la

libTAO_PortableGroup_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_PORTABLEGROUP_BUILD_DLL

libTAO_PortableGroup_la_SOURCES = \
  $(PortableGroup)

libTAO_PortableGroup_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_PortableGroup_la_LIBADD = \
 $(top_builddir)/tao/libTAO_IORManip.la \
 libTAO_CosNaming.la \
 libTAO_Svc_Utils.la \
 $(top_builddir)/tao/libTAO_Messaging.la \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO_IORTable.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  PortableGroup/PG_Factory_Map.h \
  PortableGroup/PG_Location_Map.h \
  PortableGroup/PG_Group_Factory.h \
  PortableGroup/PG_PropertyManager.h \
  PortableGroup/PG_Null_Property_Validator.h \
  PortableGroupC.h \
  PortableGroupS.h \
  miopC.h \
  PortableGroup/PG_Object_Group.h \
  PortableGroup/UIPMC_Acceptor.h \
  PortableGroup/PortableGroup_ORBInitializer.h \
  miopS.h \
  PortableGroup/UIPMC_Endpoint.h \
  PortableGroup/PG_Factory_Set.h \
  PortableGroup/PG_Operators.h \
  PortableGroup/PG_Location_Hash.h \
  PortableGroup/PG_Group_Guard.h \
  PortableGroup/UIPMC_Transport.h \
  PortableGroup/PG_Utils.h \
  PortableGroup/PG_MemberInfo.h \
  PortableGroup/Portable_Group_Map.h \
  PortableGroup/PG_Properties_Support.h \
  PortableGroup/UIPMC_Wait_Never.h \
  PortableGroup/PG_ObjectGroupManager.h \
  PortableGroup/POA_Hooks.h \
  PortableGroup/PG_FactoryRegistry.h \
  PortableGroup/PG_GenericFactory.h \
  PortableGroup/UIPMC_Factory.h \
  PortableGroup/PG_ObjectGroup_Map.h \
  PortableGroup/PG_Default_Property_Validator.h \
  PortableGroup/UIPMC_Profile.h \
  PortableGroup/PG_Properties_Encoder.h \
  PortableGroup/UIPMC_Message_Block_Data_Iterator.h \
  PortableGroup/UIPMC_Connection_Handler.h \
  PortableGroup/PG_Property_Set.h \
  PortableGroup/PortableGroup_Loader.h \
  PortableGroup/PG_Object_Group_Manipulator.h \
  PortableGroup/UIPMC_Connector.h \
  PortableGroup/PortableGroup_Request_Dispatcher.h \
  PortableGroup/PG_Location_Equal_To.h \
  PortableGroup/PG_Property_Utils.h \
  PortableGroup/PortableGroup_Acceptor_Registry.h \
  PortableGroupS_T.h \
  miopS_T.h \
  Channel_Clients_T.h \
  PortableGroup/UIPMC_Acceptor.i \
  miopS.inl \
  PortableGroup/UIPMC_Endpoint.i \
  PortableGroup/PG_Location_Hash.inl \
  PortableGroup/UIPMC_Transport.i \
  PortableGroup/UIPMC_Profile.i \
  PortableGroup/PG_Properties_Encoder.inl \
  PortableGroup/UIPMC_Connection_Handler.i \
  PortableGroup/PG_Location_Equal_To.inl \
  PortableGroupS_T.inl \
  miopS_T.inl \
  Channel_Clients_T.i \
  miopS_T.cpp \
  PortableGroupS_T.cpp \
  Channel_Clients_T.cpp


## Makefile.CosLoadBalancing.am 

BUILT_SOURCES += \
  CosLoadBalancingS_T.cpp \
  CosLoadBalancingC.cpp \
  CosLoadBalancingS.cpp \
  CosLoadBalancingC.h \
  CosLoadBalancingS.h \
  CosLoadBalancingS_T.h \
  CosLoadBalancingC.inl \
  CosLoadBalancingS.inl \
  CosLoadBalancingS_T.inl

CLEANFILES += \
  CosLoadBalancingS_T.cpp \
  CosLoadBalancingC.cpp \
  CosLoadBalancingS.cpp \
  CosLoadBalancingC.h \
  CosLoadBalancingS.h \
  CosLoadBalancingS_T.h \
  CosLoadBalancingC.inl \
  CosLoadBalancingS.inl \
  CosLoadBalancingS_T.inl

CosLoadBalancingS_T.cpp CosLoadBalancingC.cpp CosLoadBalancingS.cpp CosLoadBalancingC.h CosLoadBalancingS.h CosLoadBalancingS_T.h CosLoadBalancingC.inl CosLoadBalancingS.inl CosLoadBalancingS_T.inl: CosLoadBalancing.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -GC -Wb,export_macro=TAO_LoadBalancing_Export -Wb,export_include=LoadBalancing/LoadBalancing_export.h $(srcdir)/CosLoadBalancing.idl

BUILT_SOURCES += \
  LB_ORTS_T.cpp \
  LB_ORTC.cpp \
  LB_ORTC.h \
  LB_ORTS.h \
  LB_ORTS_T.h \
  LB_ORTC.inl \
  LB_ORTS.inl \
  LB_ORTS_T.inl

CLEANFILES += \
  LB_ORTS_T.cpp \
  LB_ORTC.cpp \
  LB_ORTC.h \
  LB_ORTS.h \
  LB_ORTS_T.h \
  LB_ORTC.inl \
  LB_ORTS.inl \
  LB_ORTS_T.inl

LB_ORTS_T.cpp LB_ORTC.cpp LB_ORTC.h LB_ORTS.h LB_ORTS_T.h LB_ORTC.inl LB_ORTS.inl LB_ORTS_T.inl: LB_ORT.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -GC -Wb,export_macro=TAO_LoadBalancing_Export -Wb,export_include=LoadBalancing/LoadBalancing_export.h $(srcdir)/LB_ORT.idl

CosLoadBalancing = \
  CosLoadBalancingC.cpp \
  CosLoadBalancingS.cpp \
  LB_ORTC.cpp \
  LoadBalancing/LB_LeastLoaded.cpp \
  LoadBalancing/LB_LoadMinimum.cpp \
  LoadBalancing/LB_LoadAverage.cpp \
  LoadBalancing/LB_LoadAlert.cpp \
  LoadBalancing/LB_LoadAlertInfo.cpp \
  LoadBalancing/LB_LoadAlertMap.cpp \
  LoadBalancing/LB_LoadAlert_Handler.cpp \
  LoadBalancing/LB_LoadListMap.cpp \
  LoadBalancing/LB_LoadManager.cpp \
  LoadBalancing/LB_LoadMap.cpp \
  LoadBalancing/LB_Location_Index_Map.cpp \
  LoadBalancing/LB_MemberLocator.cpp \
  LoadBalancing/LB_MonitorMap.cpp \
  LoadBalancing/LB_Pull_Handler.cpp \
  LoadBalancing/LB_Random.cpp \
  LoadBalancing/LB_RoundRobin.cpp \
  LoadBalancing/LB_ClientComponent.cpp \
  LoadBalancing/LB_ClientORBInitializer.cpp \
  LoadBalancing/LB_ClientRequestInterceptor.cpp \
  LoadBalancing/LB_ServerRequestInterceptor.cpp \
  LoadBalancing/LB_IORInterceptor.cpp \
  LoadBalancing/LB_ObjectReferenceFactory.cpp \
  LoadBalancing/LB_ORBInitializer.cpp \
  LoadBalancing/LB_Component.cpp \
  LoadBalancing/LB_CPU_Utilization_Monitor.cpp \
  LoadBalancing/LB_CPU_Load_Average_Monitor.cpp

lib_LTLIBRARIES += libTAO_CosLoadBalancing.la

libTAO_CosLoadBalancing_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_LOADBALANCING_BUILD_DLL

libTAO_CosLoadBalancing_la_SOURCES = \
  $(CosLoadBalancing)

libTAO_CosLoadBalancing_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_CosLoadBalancing_la_LIBADD = \
 libTAO_PortableGroup.la \
 $(top_builddir)/tao/libTAO_IORManip.la \
 libTAO_CosNaming.la \
 libTAO_Svc_Utils.la \
 $(top_builddir)/tao/libTAO_Messaging.la \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO_IORTable.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  LoadBalancing/LB_ObjectReferenceFactory.h \
  LoadBalancing/LB_LoadAlertInfo.h \
  LoadBalancing/LB_LoadMap.h \
  LoadBalancing/LB_Component.h \
  LoadBalancing/LB_LoadAverage.h \
  LoadBalancing/LB_CPU_Load_Average_Monitor.h \
  LoadBalancing/LB_ClientORBInitializer.h \
  LoadBalancing/LB_Pull_Handler.h \
  LoadBalancing/LB_ClientRequestInterceptor.h \
  LoadBalancing/LB_CPU_Utilization_Monitor.h \
  CosLoadBalancingC.h \
  LoadBalancing/LB_LoadMinimum.h \
  CosLoadBalancingS.h \
  LoadBalancing/LB_RoundRobin.h \
  LoadBalancing/LB_ORBInitializer.h \
  LoadBalancing/LB_MonitorMap.h \
  LoadBalancing/LB_LoadAlert_Handler.h \
  LoadBalancing/LB_LeastLoaded.h \
  LoadBalancing/LB_LoadManager.h \
  LoadBalancing/LB_Random.h \
  LoadBalancing/LB_ServerRequestInterceptor.h \
  LoadBalancing/LB_Location_Index_Map.h \
  LoadBalancing/LB_LoadAlertMap.h \
  LoadBalancing/LB_LoadAlert.h \
  LB_ORTC.h \
  LoadBalancing/LB_ClientComponent.h \
  LoadBalancing/LB_MemberLocator.h \
  LoadBalancing/LB_IORInterceptor.h \
  LoadBalancing/LB_LoadListMap.h \
  Channel_Clients_T.h \
  LB_ORTS_T.h \
  CosLoadBalancingS_T.h \
  LoadBalancing/LB_LoadAverage.inl \
  CosLoadBalancingC.inl \
  LoadBalancing/LB_LoadMinimum.inl \
  CosLoadBalancingS.inl \
  LoadBalancing/LB_LeastLoaded.inl \
  LoadBalancing/LB_LoadAlert.inl \
  LB_ORTC.inl \
  Channel_Clients_T.i \
  LB_ORTS_T.inl \
  CosLoadBalancingS_T.inl \
  CosLoadBalancingS_T.cpp \
  LB_ORTS_T.cpp \
  Channel_Clients_T.cpp


## Makefile.ETCL.am 

lib_LTLIBRARIES += libTAO_ETCL.la

libTAO_ETCL_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_ETCL_BUILD_DLL

libTAO_ETCL_la_SOURCES = \
  ETCL/ETCL_l.cpp \
  ETCL/ETCL_Constraint.cpp \
  ETCL/ETCL_Interpreter.cpp \
  ETCL/ETCL_y.cpp

libTAO_ETCL_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_ETCL_la_LIBADD = \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  ETCL/ETCL_Constraint.h \
  Channel_Clients_T.h \
  ETCL/ETCL_Interpreter.h \
  ETCL/ETCL_y.h \
  ETCL/ETCL_Constraint.i \
  Channel_Clients_T.i \
  Channel_Clients_T.cpp


## Makefile.CosNotification.am 

BUILT_SOURCES += \
  CosEventCommS_T.cpp \
  CosEventCommC.cpp \
  CosEventCommS.cpp \
  CosEventCommC.h \
  CosEventCommS.h \
  CosEventCommS_T.h \
  CosEventCommC.inl \
  CosEventCommS.inl \
  CosEventCommS_T.inl

CLEANFILES += \
  CosEventCommS_T.cpp \
  CosEventCommC.cpp \
  CosEventCommS.cpp \
  CosEventCommC.h \
  CosEventCommS.h \
  CosEventCommS_T.h \
  CosEventCommC.inl \
  CosEventCommS.inl \
  CosEventCommS_T.inl

CosEventCommS_T.cpp CosEventCommC.cpp CosEventCommS.cpp CosEventCommC.h CosEventCommS.h CosEventCommS_T.h CosEventCommC.inl CosEventCommS.inl CosEventCommS_T.inl: CosEventComm.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_Event_Export -Wb,export_include=CosEvent/event_export.h $(srcdir)/CosEventComm.idl

BUILT_SOURCES += \
  CosEventChannelAdminS_T.cpp \
  CosEventChannelAdminC.cpp \
  CosEventChannelAdminS.cpp \
  CosEventChannelAdminC.h \
  CosEventChannelAdminS.h \
  CosEventChannelAdminS_T.h \
  CosEventChannelAdminC.inl \
  CosEventChannelAdminS.inl \
  CosEventChannelAdminS_T.inl

CLEANFILES += \
  CosEventChannelAdminS_T.cpp \
  CosEventChannelAdminC.cpp \
  CosEventChannelAdminS.cpp \
  CosEventChannelAdminC.h \
  CosEventChannelAdminS.h \
  CosEventChannelAdminS_T.h \
  CosEventChannelAdminC.inl \
  CosEventChannelAdminS.inl \
  CosEventChannelAdminS_T.inl

CosEventChannelAdminS_T.cpp CosEventChannelAdminC.cpp CosEventChannelAdminS.cpp CosEventChannelAdminC.h CosEventChannelAdminS.h CosEventChannelAdminS_T.h CosEventChannelAdminC.inl CosEventChannelAdminS.inl CosEventChannelAdminS_T.inl: CosEventChannelAdmin.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_Event_Export -Wb,export_include=CosEvent/event_export.h $(srcdir)/CosEventChannelAdmin.idl

BUILT_SOURCES += \
  CosNotificationS_T.cpp \
  CosNotificationC.cpp \
  CosNotificationS.cpp \
  CosNotificationC.h \
  CosNotificationS.h \
  CosNotificationS_T.h \
  CosNotificationC.inl \
  CosNotificationS.inl \
  CosNotificationS_T.inl

CLEANFILES += \
  CosNotificationS_T.cpp \
  CosNotificationC.cpp \
  CosNotificationS.cpp \
  CosNotificationC.h \
  CosNotificationS.h \
  CosNotificationS_T.h \
  CosNotificationC.inl \
  CosNotificationS.inl \
  CosNotificationS_T.inl

CosNotificationS_T.cpp CosNotificationC.cpp CosNotificationS.cpp CosNotificationC.h CosNotificationS.h CosNotificationS_T.h CosNotificationC.inl CosNotificationS.inl CosNotificationS_T.inl: CosNotification.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_Event_Export -Wb,export_include=CosEvent/event_export.h $(srcdir)/CosNotification.idl

BUILT_SOURCES += \
  CosNotifyCommS_T.cpp \
  CosNotifyCommC.cpp \
  CosNotifyCommS.cpp \
  CosNotifyCommC.h \
  CosNotifyCommS.h \
  CosNotifyCommS_T.h \
  CosNotifyCommC.inl \
  CosNotifyCommS.inl \
  CosNotifyCommS_T.inl

CLEANFILES += \
  CosNotifyCommS_T.cpp \
  CosNotifyCommC.cpp \
  CosNotifyCommS.cpp \
  CosNotifyCommC.h \
  CosNotifyCommS.h \
  CosNotifyCommS_T.h \
  CosNotifyCommC.inl \
  CosNotifyCommS.inl \
  CosNotifyCommS_T.inl

CosNotifyCommS_T.cpp CosNotifyCommC.cpp CosNotifyCommS.cpp CosNotifyCommC.h CosNotifyCommS.h CosNotifyCommS_T.h CosNotifyCommC.inl CosNotifyCommS.inl CosNotifyCommS_T.inl: CosNotifyComm.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_Event_Export -Wb,export_include=CosEvent/event_export.h $(srcdir)/CosNotifyComm.idl

BUILT_SOURCES += \
  CosNotifyFilterS_T.cpp \
  CosNotifyFilterC.cpp \
  CosNotifyFilterS.cpp \
  CosNotifyFilterC.h \
  CosNotifyFilterS.h \
  CosNotifyFilterS_T.h \
  CosNotifyFilterC.inl \
  CosNotifyFilterS.inl \
  CosNotifyFilterS_T.inl

CLEANFILES += \
  CosNotifyFilterS_T.cpp \
  CosNotifyFilterC.cpp \
  CosNotifyFilterS.cpp \
  CosNotifyFilterC.h \
  CosNotifyFilterS.h \
  CosNotifyFilterS_T.h \
  CosNotifyFilterC.inl \
  CosNotifyFilterS.inl \
  CosNotifyFilterS_T.inl

CosNotifyFilterS_T.cpp CosNotifyFilterC.cpp CosNotifyFilterS.cpp CosNotifyFilterC.h CosNotifyFilterS.h CosNotifyFilterS_T.h CosNotifyFilterC.inl CosNotifyFilterS.inl CosNotifyFilterS_T.inl: CosNotifyFilter.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_Event_Export -Wb,export_include=CosEvent/event_export.h $(srcdir)/CosNotifyFilter.idl

BUILT_SOURCES += \
  CosNotifyChannelAdminS_T.cpp \
  CosNotifyChannelAdminC.cpp \
  CosNotifyChannelAdminS.cpp \
  CosNotifyChannelAdminC.h \
  CosNotifyChannelAdminS.h \
  CosNotifyChannelAdminS_T.h \
  CosNotifyChannelAdminC.inl \
  CosNotifyChannelAdminS.inl \
  CosNotifyChannelAdminS_T.inl

CLEANFILES += \
  CosNotifyChannelAdminS_T.cpp \
  CosNotifyChannelAdminC.cpp \
  CosNotifyChannelAdminS.cpp \
  CosNotifyChannelAdminC.h \
  CosNotifyChannelAdminS.h \
  CosNotifyChannelAdminS_T.h \
  CosNotifyChannelAdminC.inl \
  CosNotifyChannelAdminS.inl \
  CosNotifyChannelAdminS_T.inl

CosNotifyChannelAdminS_T.cpp CosNotifyChannelAdminC.cpp CosNotifyChannelAdminS.cpp CosNotifyChannelAdminC.h CosNotifyChannelAdminS.h CosNotifyChannelAdminS_T.h CosNotifyChannelAdminC.inl CosNotifyChannelAdminS.inl CosNotifyChannelAdminS_T.inl: CosNotifyChannelAdmin.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_Event_Export -Wb,export_include=CosEvent/event_export.h $(srcdir)/CosNotifyChannelAdmin.idl

BUILT_SOURCES += \
  NotifyExtS_T.cpp \
  NotifyExtC.cpp \
  NotifyExtS.cpp \
  NotifyExtC.h \
  NotifyExtS.h \
  NotifyExtS_T.h \
  NotifyExtC.inl \
  NotifyExtS.inl \
  NotifyExtS_T.inl

CLEANFILES += \
  NotifyExtS_T.cpp \
  NotifyExtC.cpp \
  NotifyExtS.cpp \
  NotifyExtC.h \
  NotifyExtS.h \
  NotifyExtS_T.h \
  NotifyExtC.inl \
  NotifyExtS.inl \
  NotifyExtS_T.inl

NotifyExtS_T.cpp NotifyExtC.cpp NotifyExtS.cpp NotifyExtC.h NotifyExtS.h NotifyExtS_T.h NotifyExtC.inl NotifyExtS.inl NotifyExtS_T.inl: NotifyExt.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_Event_Export -Wb,export_include=CosEvent/event_export.h $(srcdir)/NotifyExt.idl

BUILT_SOURCES += \
  Event_ForwarderS_T.cpp \
  Event_ForwarderC.cpp \
  Event_ForwarderS.cpp \
  Event_ForwarderC.h \
  Event_ForwarderS.h \
  Event_ForwarderS_T.h \
  Event_ForwarderC.inl \
  Event_ForwarderS.inl \
  Event_ForwarderS_T.inl

CLEANFILES += \
  Event_ForwarderS_T.cpp \
  Event_ForwarderC.cpp \
  Event_ForwarderS.cpp \
  Event_ForwarderC.h \
  Event_ForwarderS.h \
  Event_ForwarderS_T.h \
  Event_ForwarderC.inl \
  Event_ForwarderS.inl \
  Event_ForwarderS_T.inl

Event_ForwarderS_T.cpp Event_ForwarderC.cpp Event_ForwarderS.cpp Event_ForwarderC.h Event_ForwarderS.h Event_ForwarderS_T.h Event_ForwarderC.inl Event_ForwarderS.inl Event_ForwarderS_T.inl: Event_Forwarder.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_Event_Export -Wb,export_include=CosEvent/event_export.h $(srcdir)/Event_Forwarder.idl

Notify = \
  CosEventCommC.cpp \
  CosEventCommS.cpp \
  CosEventChannelAdminC.cpp \
  CosEventChannelAdminS.cpp \
  CosNotificationC.cpp \
  CosNotificationS.cpp \
  CosNotifyCommC.cpp \
  CosNotifyCommS.cpp \
  CosNotifyFilterC.cpp \
  CosNotifyFilterS.cpp \
  CosNotifyChannelAdminC.cpp \
  CosNotifyChannelAdminS.cpp \
  NotifyExtC.cpp \
  NotifyExtS.cpp \
  Event_ForwarderC.cpp \
  Event_ForwarderS.cpp \
  Notify/Admin.cpp \
  Notify/AdminProperties.cpp \
  Notify/Buffering_Strategy.cpp \
  Notify/Builder.cpp \
  Notify/Consumer.cpp \
  Notify/ConsumerAdmin.cpp \
  Notify/CosNotify_Initializer.cpp \
  Notify/CosNotify_Service.cpp \
  Notify/Default_Factory.cpp \
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
  Notify/Notify_Constraint_Interpreter.cpp \
  Notify/Notify_Constraint_Visitors.cpp \
  Notify/Notify_Default_Collection_Factory.cpp \
  Notify/Notify_Default_CO_Factory.cpp \
  Notify/Notify_Default_EMO_Factory.cpp \
  Notify/Notify_Default_POA_Factory.cpp \
  Notify/Notify_EventChannelFactory_i.cpp \
  Notify/Notify_Extensions.cpp \
  Notify/Object.cpp \
  Notify/Peer.cpp \
  Notify/POA_Helper.cpp \
  Notify/Properties.cpp \
  Notify/PropertySeq.cpp \
  Notify/Property_Boolean.cpp \
  Notify/Proxy.cpp \
  Notify/ProxyConsumer.cpp \
  Notify/ProxySupplier.cpp \
  Notify/QoSProperties.cpp \
  Notify/Reactive_Task.cpp \
  Notify/Refcountable.cpp \
  Notify/Subscription_Change_Worker.cpp \
  Notify/Supplier.cpp \
  Notify/SupplierAdmin.cpp \
  Notify/ThreadPool_Task.cpp \
  Notify/Timer_Queue.cpp \
  Notify/Timer_Reactor.cpp \
  Notify/Worker_Task.cpp \
  Notify/Any/AnyEvent.cpp \
  Notify/Any/CosEC_ProxyPushConsumer.cpp \
  Notify/Any/CosEC_ProxyPushSupplier.cpp \
  Notify/Any/ProxyPushConsumer.cpp \
  Notify/Any/ProxyPushSupplier.cpp \
  Notify/Any/PushConsumer.cpp \
  Notify/Any/PushSupplier.cpp \
  Notify/Sequence/Batch_Buffering_Strategy.cpp \
  Notify/Sequence/SequenceProxyPushConsumer.cpp \
  Notify/Sequence/SequenceProxyPushSupplier.cpp \
  Notify/Sequence/SequencePushConsumer.cpp \
  Notify/Sequence/SequencePushSupplier.cpp \
  Notify/Structured/StructuredEvent.cpp \
  Notify/Structured/StructuredProxyPushConsumer.cpp \
  Notify/Structured/StructuredProxyPushSupplier.cpp \
  Notify/Structured/StructuredPushConsumer.cpp \
  Notify/Structured/StructuredPushSupplier.cpp

lib_LTLIBRARIES += libTAO_CosNotification.la

libTAO_CosNotification_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_NOTIFY_BUILD_DLL -DTAO_EVENT_BUILD_DLL

libTAO_CosNotification_la_SOURCES = \
  $(Notify)

libTAO_CosNotification_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_CosNotification_la_LIBADD = \
 $(top_builddir)/tao/libTAO_DynamicAny.la \
 libTAO_ETCL.la \
 libTAO_Svc_Utils.la \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  Notify/Notify_Default_POA_Factory.h \
  Notify/Default_Factory.h \
  Notify/CosNotify_Service.h \
  Notify/Notify_Default_EMO_Factory.h \
  Notify/Any/PushSupplier.h \
  Notify/Any/PushConsumer.h \
  Notify/Reactive_Task.h \
  Notify/Method_Request.h \
  CosNotifyCommC.h \
  Notify/Admin.h \
  Notify/ThreadPool_Task.h \
  Notify/Timer_Reactor.h \
  Notify/Notify_Default_CO_Factory.h \
  Notify/Notify_Constraint_Visitors.h \
  Notify/Notify_Extensions.h \
  Notify/Properties.h \
  CosNotifyCommS.h \
  Notify/Structured/StructuredProxyPushSupplier.h \
  Notify/Structured/StructuredProxyPushConsumer.h \
  Notify/AdminProperties.h \
  Notify/EventChannelFactory.h \
  Notify/ConsumerAdmin.h \
  Notify/Any/ProxyPushSupplier.h \
  Notify/Any/ProxyPushConsumer.h \
  Notify/Builder.h \
  Notify/Subscription_Change_Worker.h \
  CosEventChannelAdminC.h \
  Notify/Refcountable.h \
  Notify/Notify_EventChannelFactory_i.h \
  CosEventChannelAdminS.h \
  Notify/ProxySupplier.h \
  Notify/Supplier.h \
  Notify/ProxyConsumer.h \
  Notify/Consumer.h \
  Notify/Sequence/Batch_Buffering_Strategy.h \
  Notify/Event.h \
  CosEventCommC.h \
  Notify/Worker_Task.h \
  Notify/EventType.h \
  Notify/EventTypeSeq.h \
  CosEventCommS.h \
  Notify/Proxy.h \
  Notify/Any/CosEC_ProxyPushSupplier.h \
  Notify/Notify_Default_Collection_Factory.h \
  Notify/Any/CosEC_ProxyPushConsumer.h \
  Notify/SupplierAdmin.h \
  Notify/Buffering_Strategy.h \
  Notify/QoSProperties.h \
  Notify/Method_Request_Event.h \
  CosNotifyFilterC.h \
  Notify/Sequence/SequencePushSupplier.h \
  CosNotifyFilterS.h \
  Notify/Sequence/SequencePushConsumer.h \
  Notify/Method_Request_Dispatch.h \
  Notify/POA_Helper.h \
  Notify/Structured/StructuredEvent.h \
  Notify/PropertySeq.h \
  Notify/Notify_Constraint_Interpreter.h \
  Event_ForwarderC.h \
  CosNotifyChannelAdminC.h \
  Event_ForwarderS.h \
  Notify/FilterAdmin.h \
  CosNotificationC.h \
  Notify/Method_Request_Shutdown.h \
  CosNotifyChannelAdminS.h \
  Notify/Peer.h \
  Notify/Any/AnyEvent.h \
  Notify/CosNotify_Initializer.h \
  CosNotificationS.h \
  Notify/Structured/StructuredPushSupplier.h \
  Notify/Structured/StructuredPushConsumer.h \
  Notify/Object.h \
  NotifyExtC.h \
  Notify/ID_Factory.h \
  NotifyExtS.h \
  Notify/Event_Manager.h \
  Notify/ETCL_FilterFactory.h \
  Notify/Property_Boolean.h \
  Notify/Sequence/SequenceProxyPushSupplier.h \
  Notify/Sequence/SequenceProxyPushConsumer.h \
  Notify/EventChannel.h \
  Notify/Method_Request_Updates.h \
  Notify/ETCL_Filter.h \
  Notify/Timer_Queue.h \
  Notify/Method_Request_Lookup.h \
  Event_ForwarderS_T.h \
  CosNotifyFilterS_T.h \
  NotifyExtS_T.h \
  CosEventChannelAdminS_T.h \
  Channel_Clients_T.h \
  CosNotifyCommS_T.h \
  CosNotificationS_T.h \
  CosNotifyChannelAdminS_T.h \
  CosEventCommS_T.h \
  Notify/Default_Factory.inl \
  Notify/CosNotify_Service.inl \
  Notify/Any/PushSupplier.inl \
  Notify/Any/PushConsumer.inl \
  Notify/Reactive_Task.inl \
  Notify/Method_Request.inl \
  CosNotifyCommC.inl \
  Notify/Admin.inl \
  Notify/ThreadPool_Task.inl \
  Notify/Timer_Reactor.inl \
  Notify/Properties.inl \
  CosNotifyCommS.inl \
  Notify/Structured/StructuredProxyPushSupplier.inl \
  Notify/Structured/StructuredProxyPushConsumer.inl \
  Notify/AdminProperties.inl \
  Notify/EventChannelFactory.inl \
  Notify/ConsumerAdmin.inl \
  Notify/Any/ProxyPushSupplier.inl \
  Notify/Any/ProxyPushConsumer.inl \
  Notify/Builder.inl \
  Notify/Subscription_Change_Worker.inl \
  CosEventChannelAdminC.inl \
  Notify/Refcountable.inl \
  CosEventChannelAdminS.inl \
  Notify/ProxySupplier.inl \
  Notify/Supplier.inl \
  Notify/ProxyConsumer.inl \
  Notify/Consumer.inl \
  Notify/Sequence/Batch_Buffering_Strategy.inl \
  Notify/Event.inl \
  CosEventCommC.inl \
  Notify/Worker_Task.inl \
  Notify/EventType.inl \
  Notify/EventTypeSeq.inl \
  CosEventCommS.inl \
  Notify/Proxy.inl \
  Notify/Any/CosEC_ProxyPushSupplier.inl \
  Notify/Any/CosEC_ProxyPushConsumer.inl \
  Notify/SupplierAdmin.inl \
  Notify/Buffering_Strategy.inl \
  Notify/QoSProperties.inl \
  Notify/Method_Request_Event.inl \
  CosNotifyFilterC.inl \
  Notify/Sequence/SequencePushSupplier.inl \
  CosNotifyFilterS.inl \
  Notify/Sequence/SequencePushConsumer.inl \
  Notify/Method_Request_Dispatch.inl \
  Notify/POA_Helper.inl \
  Notify/Structured/StructuredEvent.inl \
  Notify/PropertySeq.inl \
  Event_ForwarderC.inl \
  CosNotifyChannelAdminC.inl \
  Event_ForwarderS.inl \
  Notify/FilterAdmin.inl \
  CosNotificationC.inl \
  Notify/Method_Request_Shutdown.inl \
  CosNotifyChannelAdminS.inl \
  Notify/Peer.inl \
  Notify/Any/AnyEvent.inl \
  CosNotificationS.inl \
  Notify/Structured/StructuredPushSupplier.inl \
  Notify/Structured/StructuredPushConsumer.inl \
  Notify/Object.inl \
  NotifyExtC.inl \
  Notify/ID_Factory.inl \
  NotifyExtS.inl \
  Notify/Event_Manager.inl \
  Notify/ETCL_FilterFactory.inl \
  Notify/Property_Boolean.inl \
  Notify/Sequence/SequenceProxyPushSupplier.inl \
  Notify/Sequence/SequenceProxyPushConsumer.inl \
  Notify/EventChannel.inl \
  Notify/Method_Request_Updates.inl \
  Notify/ETCL_Filter.inl \
  Notify/Timer_Queue.inl \
  Notify/Method_Request_Lookup.inl \
  Event_ForwarderS_T.inl \
  CosNotifyFilterS_T.inl \
  NotifyExtS_T.inl \
  CosEventChannelAdminS_T.inl \
  Channel_Clients_T.i \
  CosNotifyCommS_T.inl \
  CosNotificationS_T.inl \
  CosNotifyChannelAdminS_T.inl \
  CosEventCommS_T.inl \
  CosEventCommS_T.cpp \
  CosEventChannelAdminS_T.cpp \
  CosNotificationS_T.cpp \
  CosNotifyCommS_T.cpp \
  CosNotifyFilterS_T.cpp \
  CosNotifyChannelAdminS_T.cpp \
  NotifyExtS_T.cpp \
  Event_ForwarderS_T.cpp \
  Channel_Clients_T.cpp


## Makefile.CosTime.am 

BUILT_SOURCES += \
  TimeServiceS_T.cpp \
  TimeServiceC.cpp \
  TimeServiceS.cpp \
  TimeServiceC.h \
  TimeServiceS.h \
  TimeServiceS_T.h \
  TimeServiceC.inl \
  TimeServiceS.inl \
  TimeServiceS_T.inl

CLEANFILES += \
  TimeServiceS_T.cpp \
  TimeServiceC.cpp \
  TimeServiceS.cpp \
  TimeServiceC.h \
  TimeServiceS.h \
  TimeServiceS_T.h \
  TimeServiceC.inl \
  TimeServiceS.inl \
  TimeServiceS_T.inl

TimeServiceS_T.cpp TimeServiceC.cpp TimeServiceS.cpp TimeServiceC.h TimeServiceS.h TimeServiceS_T.h TimeServiceC.inl TimeServiceS.inl TimeServiceS_T.inl: TimeService.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_Time_Export -Wb,export_include=Time/time_export.h $(srcdir)/TimeService.idl

Time = \
  TimeServiceC.cpp \
  TimeServiceS.cpp \
  Time/Timer_Helper.cpp \
  Time/TAO_TIO.cpp \
  Time/TAO_UTO.cpp \
  Time/TAO_Time_Service_Clerk.cpp \
  Time/TAO_Time_Service_Server.cpp

lib_LTLIBRARIES += libTAO_CosTime.la

libTAO_CosTime_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_TIME_BUILD_DLL

libTAO_CosTime_la_SOURCES = \
  $(Time)

libTAO_CosTime_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_CosTime_la_LIBADD = \
 libTAO_CosNaming.la \
 libTAO_Svc_Utils.la \
 $(top_builddir)/tao/libTAO_Messaging.la \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO_IORTable.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  Channel_Clients_T.h \
  TimeServiceS_T.h \
  Time/TAO_Time_Service_Server.h \
  Time/TAO_UTO.h \
  TimeServiceC.h \
  Time/TAO_Time_Service_Clerk.h \
  TimeServiceS.h \
  Time/TAO_TIO.h \
  Time/Timer_Helper.h \
  Channel_Clients_T.i \
  TimeServiceS_T.inl \
  TimeServiceS_T.cpp \
  Channel_Clients_T.cpp


## Makefile.CosTrading.am 

BUILT_SOURCES += \
  CosTradingS_T.cpp \
  CosTradingC.cpp \
  CosTradingS.cpp \
  CosTradingC.h \
  CosTradingS.h \
  CosTradingS_T.h \
  CosTradingC.inl \
  CosTradingS.inl \
  CosTradingS_T.inl

CLEANFILES += \
  CosTradingS_T.cpp \
  CosTradingC.cpp \
  CosTradingS.cpp \
  CosTradingC.h \
  CosTradingS.h \
  CosTradingS_T.h \
  CosTradingC.inl \
  CosTradingS.inl \
  CosTradingS_T.inl

CosTradingS_T.cpp CosTradingC.cpp CosTradingS.cpp CosTradingC.h CosTradingS.h CosTradingS_T.h CosTradingC.inl CosTradingS.inl CosTradingS_T.inl: CosTrading.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_Trading_Export -Wb,export_include=Trader/trading_export.h $(srcdir)/CosTrading.idl

BUILT_SOURCES += \
  CosTradingReposS_T.cpp \
  CosTradingReposC.cpp \
  CosTradingReposS.cpp \
  CosTradingReposC.h \
  CosTradingReposS.h \
  CosTradingReposS_T.h \
  CosTradingReposC.inl \
  CosTradingReposS.inl \
  CosTradingReposS_T.inl

CLEANFILES += \
  CosTradingReposS_T.cpp \
  CosTradingReposC.cpp \
  CosTradingReposS.cpp \
  CosTradingReposC.h \
  CosTradingReposS.h \
  CosTradingReposS_T.h \
  CosTradingReposC.inl \
  CosTradingReposS.inl \
  CosTradingReposS_T.inl

CosTradingReposS_T.cpp CosTradingReposC.cpp CosTradingReposS.cpp CosTradingReposC.h CosTradingReposS.h CosTradingReposS_T.h CosTradingReposC.inl CosTradingReposS.inl CosTradingReposS_T.inl: CosTradingRepos.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_Trading_Export -Wb,export_include=Trader/trading_export.h $(srcdir)/CosTradingRepos.idl

BUILT_SOURCES += \
  CosTradingDynamicS_T.cpp \
  CosTradingDynamicC.cpp \
  CosTradingDynamicS.cpp \
  CosTradingDynamicC.h \
  CosTradingDynamicS.h \
  CosTradingDynamicS_T.h \
  CosTradingDynamicC.inl \
  CosTradingDynamicS.inl \
  CosTradingDynamicS_T.inl

CLEANFILES += \
  CosTradingDynamicS_T.cpp \
  CosTradingDynamicC.cpp \
  CosTradingDynamicS.cpp \
  CosTradingDynamicC.h \
  CosTradingDynamicS.h \
  CosTradingDynamicS_T.h \
  CosTradingDynamicC.inl \
  CosTradingDynamicS.inl \
  CosTradingDynamicS_T.inl

CosTradingDynamicS_T.cpp CosTradingDynamicC.cpp CosTradingDynamicS.cpp CosTradingDynamicC.h CosTradingDynamicS.h CosTradingDynamicS_T.h CosTradingDynamicC.inl CosTradingDynamicS.inl CosTradingDynamicS_T.inl: CosTradingDynamic.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_Trading_Export -Wb,export_include=Trader/trading_export.h $(srcdir)/CosTradingDynamic.idl

Trader = \
  CosTradingC.cpp \
  CosTradingS.cpp \
  CosTradingReposC.cpp \
  CosTradingReposS.cpp \
  CosTradingDynamicC.cpp \
  CosTradingDynamicS.cpp \
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
  Trader/Trader_Utils.cpp \
  Trader/Trader_Constraint_Visitors.cpp \
  Trader/Trader_Interfaces.cpp \
  Trader/Trading_Loader.cpp

lib_LTLIBRARIES += libTAO_CosTrading.la

libTAO_CosTrading_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_TRADING_BUILD_DLL

libTAO_CosTrading_la_SOURCES = \
  $(Trader)

libTAO_CosTrading_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_CosTrading_la_LIBADD = \
 libTAO_Svc_Utils.la \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO_DynamicAny.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  Trader/Trader_Constraint_Visitors.h \
  Trader/Service_Type_Repository.h \
  Trader/Offer_Database.h \
  Trader/Constraint_Nodes.h \
  Trader/Trading_Loader.h \
  Trader/Offer_Iterators.h \
  CosTradingDynamicC.h \
  CosTradingDynamicS.h \
  Trader/Constraint_Visitors.h \
  CosTradingC.h \
  CosTradingS.h \
  Trader/Trader.h \
  Trader/Trader_Utils.h \
  Trader/Interpreter_Utils.h \
  CosTradingReposC.h \
  Trader/Trader_Interfaces.h \
  CosTradingReposS.h \
  Trader/Constraint_Interpreter.h \
  Trader/Interpreter.h \
  CosTradingReposS_T.h \
  CosTradingDynamicS_T.h \
  CosTradingS_T.h \
  Channel_Clients_T.h \
  CosTradingReposS_T.inl \
  CosTradingDynamicS_T.inl \
  CosTradingS_T.inl \
  Channel_Clients_T.i \
  CosTradingS_T.cpp \
  CosTradingReposS_T.cpp \
  CosTradingDynamicS_T.cpp \
  Channel_Clients_T.cpp


## Makefile.DsLogAdmin.am 

BUILT_SOURCES += \
  DsLogAdminS_T.cpp \
  DsLogAdminC.cpp \
  DsLogAdminS.cpp \
  DsLogAdminC.h \
  DsLogAdminS.h \
  DsLogAdminS_T.h \
  DsLogAdminC.inl \
  DsLogAdminS.inl \
  DsLogAdminS_T.inl

CLEANFILES += \
  DsLogAdminS_T.cpp \
  DsLogAdminC.cpp \
  DsLogAdminS.cpp \
  DsLogAdminC.h \
  DsLogAdminS.h \
  DsLogAdminS_T.h \
  DsLogAdminC.inl \
  DsLogAdminS.inl \
  DsLogAdminS_T.inl

DsLogAdminS_T.cpp DsLogAdminC.cpp DsLogAdminS.cpp DsLogAdminC.h DsLogAdminS.h DsLogAdminS_T.h DsLogAdminC.inl DsLogAdminS.inl DsLogAdminS_T.inl: DsLogAdmin.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_Log_Export -Wb,export_include=Log/log_export.h $(srcdir)/DsLogAdmin.idl

BUILT_SOURCES += \
  DsLogNotificationS_T.cpp \
  DsLogNotificationC.cpp \
  DsLogNotificationS.cpp \
  DsLogNotificationC.h \
  DsLogNotificationS.h \
  DsLogNotificationS_T.h \
  DsLogNotificationC.inl \
  DsLogNotificationS.inl \
  DsLogNotificationS_T.inl

CLEANFILES += \
  DsLogNotificationS_T.cpp \
  DsLogNotificationC.cpp \
  DsLogNotificationS.cpp \
  DsLogNotificationC.h \
  DsLogNotificationS.h \
  DsLogNotificationS_T.h \
  DsLogNotificationC.inl \
  DsLogNotificationS.inl \
  DsLogNotificationS_T.inl

DsLogNotificationS_T.cpp DsLogNotificationC.cpp DsLogNotificationS.cpp DsLogNotificationC.h DsLogNotificationS.h DsLogNotificationS_T.h DsLogNotificationC.inl DsLogNotificationS.inl DsLogNotificationS_T.inl: DsLogNotification.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_Log_Export -Wb,export_include=Log/log_export.h $(srcdir)/DsLogNotification.idl

DsLogAdmin = \
  DsLogAdminC.cpp \
  DsLogAdminS.cpp \
  DsLogNotificationC.cpp \
  DsLogNotificationS.cpp \
  Log/PersistStore.cpp \
  Log/LogRecordStore.cpp \
  Log/BasicLogFactory_i.cpp \
  Log/Iterator_i.cpp \
  Log/BasicLog_i.cpp \
  Log/Log_i.cpp \
  Log/LogMgr_i.cpp \
  Log/Log_Constraint_Visitors.cpp \
  Log/Log_Constraint_Interpreter.cpp \
  Log/LogNotification.cpp

lib_LTLIBRARIES += libTAO_DsLogAdmin.la

libTAO_DsLogAdmin_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_LOG_BUILD_DLL

libTAO_DsLogAdmin_la_SOURCES = \
  $(DsLogAdmin)

libTAO_DsLogAdmin_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_DsLogAdmin_la_LIBADD = \
 $(top_builddir)/tao/libTAO_DynamicAny.la \
 libTAO_CosNaming.la \
 libTAO_Svc_Utils.la \
 $(top_builddir)/tao/libTAO_Messaging.la \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO_IORTable.la \
 libTAO_ETCL.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  Log/Log_Constraint_Visitors.h \
  Log/BasicLogFactory_i.h \
  Log/Iterator_i.h \
  Log/PersistStore.h \
  Log/LogRecordStore.h \
  Log/Log_Constraint_Interpreter.h \
  Log/BasicLog_i.h \
  DsLogAdminC.h \
  DsLogAdminS.h \
  Log/LogMgr_i.h \
  Log/Log_i.h \
  Log/LogNotification.h \
  DsLogNotificationC.h \
  DsLogNotificationS.h \
  DsLogNotificationS_T.h \
  DsLogAdminS_T.h \
  Channel_Clients_T.h \
  DsLogNotificationS_T.inl \
  DsLogAdminS_T.inl \
  Channel_Clients_T.i \
  DsLogAdminS_T.cpp \
  DsLogNotificationS_T.cpp \
  Channel_Clients_T.cpp


## Makefile.DsEventLogAdmin.am 

BUILT_SOURCES += \
  DsEventLogAdminS_T.cpp \
  DsEventLogAdminC.cpp \
  DsEventLogAdminS.cpp \
  DsEventLogAdminC.h \
  DsEventLogAdminS.h \
  DsEventLogAdminS_T.h \
  DsEventLogAdminC.inl \
  DsEventLogAdminS.inl \
  DsEventLogAdminS_T.inl

CLEANFILES += \
  DsEventLogAdminS_T.cpp \
  DsEventLogAdminC.cpp \
  DsEventLogAdminS.cpp \
  DsEventLogAdminC.h \
  DsEventLogAdminS.h \
  DsEventLogAdminS_T.h \
  DsEventLogAdminC.inl \
  DsEventLogAdminS.inl \
  DsEventLogAdminS_T.inl

DsEventLogAdminS_T.cpp DsEventLogAdminC.cpp DsEventLogAdminS.cpp DsEventLogAdminC.h DsEventLogAdminS.h DsEventLogAdminS_T.h DsEventLogAdminC.inl DsEventLogAdminS.inl DsEventLogAdminS_T.inl: DsEventLogAdmin.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_EventLog_Export -Wb,export_include=Log/eventlog_export.h $(srcdir)/DsEventLogAdmin.idl

DsEventLogAdmin = \
  DsEventLogAdminC.cpp \
  DsEventLogAdminS.cpp \
  Log/EventLogFactory_i.cpp \
  Log/EventLog_i.cpp \
  Log/EventLogConsumer.cpp \
  Log/EventLogNotification.cpp

lib_LTLIBRARIES += libTAO_DsEventLogAdmin.la

libTAO_DsEventLogAdmin_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_HAS_TYPED_EVENT_CHANNEL \
  -DTAO_EVENTLOG_BUILD_DLL

libTAO_DsEventLogAdmin_la_SOURCES = \
  $(DsEventLogAdmin)

libTAO_DsEventLogAdmin_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_DsEventLogAdmin_la_LIBADD = \
 libTAO_CosEvent.la \
 libTAO_CosNaming.la \
 $(top_builddir)/tao/libTAO_IORTable.la \
 $(top_builddir)/tao/libTAO_IFR_Client.la \
 $(top_builddir)/tao/libTAO_DynamicInterface.la \
 $(top_builddir)/tao/libTAO_Messaging.la \
 libTAO_DsLogAdmin.la \
 $(top_builddir)/tao/libTAO_DynamicAny.la \
 libTAO_ETCL.la \
 libTAO_Svc_Utils.la \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  Log/EventLogConsumer.h \
  DsEventLogAdminC.h \
  DsEventLogAdminS.h \
  Log/EventLogFactory_i.h \
  Log/EventLog_i.h \
  Log/EventLogNotification.h \
  Channel_Clients_T.h \
  DsEventLogAdminS_T.h \
  Channel_Clients_T.i \
  DsEventLogAdminS_T.inl \
  DsEventLogAdminS_T.cpp \
  Channel_Clients_T.cpp


## Makefile.DsNotifyLogAdmin.am 

BUILT_SOURCES += \
  DsNotifyLogAdminS_T.cpp \
  DsNotifyLogAdminC.cpp \
  DsNotifyLogAdminS.cpp \
  DsNotifyLogAdminC.h \
  DsNotifyLogAdminS.h \
  DsNotifyLogAdminS_T.h \
  DsNotifyLogAdminC.inl \
  DsNotifyLogAdminS.inl \
  DsNotifyLogAdminS_T.inl

CLEANFILES += \
  DsNotifyLogAdminS_T.cpp \
  DsNotifyLogAdminC.cpp \
  DsNotifyLogAdminS.cpp \
  DsNotifyLogAdminC.h \
  DsNotifyLogAdminS.h \
  DsNotifyLogAdminS_T.h \
  DsNotifyLogAdminC.inl \
  DsNotifyLogAdminS.inl \
  DsNotifyLogAdminS_T.inl

DsNotifyLogAdminS_T.cpp DsNotifyLogAdminC.cpp DsNotifyLogAdminS.cpp DsNotifyLogAdminC.h DsNotifyLogAdminS.h DsNotifyLogAdminS_T.h DsNotifyLogAdminC.inl DsNotifyLogAdminS.inl DsNotifyLogAdminS_T.inl: DsNotifyLogAdmin.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_NotifyLog_Export -Wb,export_include=Log/notifylog_export.h $(srcdir)/DsNotifyLogAdmin.idl

DsNotifyLogAdmin = \
  DsNotifyLogAdminC.cpp \
  DsNotifyLogAdminS.cpp \
  Log/NotifyLogFactory_i.cpp \
  Log/NotifyLog_i.cpp \
  Log/NotifyLogConsumer.cpp \
  Log/NotifyLogNotification.cpp

lib_LTLIBRARIES += libTAO_DsNotifyLogAdmin.la

libTAO_DsNotifyLogAdmin_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_HAS_TYPED_EVENT_CHANNEL \
  -DTAO_NOTIFYLOG_BUILD_DLL

libTAO_DsNotifyLogAdmin_la_SOURCES = \
  $(DsNotifyLogAdmin)

libTAO_DsNotifyLogAdmin_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_DsNotifyLogAdmin_la_LIBADD = \
 libTAO_CosNotification.la \
 libTAO_DsEventLogAdmin.la \
 libTAO_CosEvent.la \
 libTAO_CosNaming.la \
 $(top_builddir)/tao/libTAO_IORTable.la \
 $(top_builddir)/tao/libTAO_IFR_Client.la \
 $(top_builddir)/tao/libTAO_DynamicInterface.la \
 $(top_builddir)/tao/libTAO_Messaging.la \
 libTAO_DsLogAdmin.la \
 $(top_builddir)/tao/libTAO_DynamicAny.la \
 libTAO_ETCL.la \
 libTAO_Svc_Utils.la \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  Log/NotifyLog_i.h \
  DsNotifyLogAdminC.h \
  Log/NotifyLogFactory_i.h \
  DsNotifyLogAdminS.h \
  Log/NotifyLogConsumer.h \
  Log/NotifyLogNotification.h \
  DsNotifyLogAdminS_T.h \
  Channel_Clients_T.h \
  DsNotifyLogAdminS_T.inl \
  Channel_Clients_T.i \
  DsNotifyLogAdminS_T.cpp \
  Channel_Clients_T.cpp


## Makefile.FTRT_ClientORB.am 

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
  -I$(TAO_ROOT)/orbsvcs/orbsvcs/FtRtEvent/ClientORB \
  -DTAO_FTRT_BUILD_DLL

libTAO_FTRT_ClientORB_la_SOURCES = \
  $(FTRT_ClientORB)

libTAO_FTRT_ClientORB_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_FTRT_ClientORB_la_LIBADD = \
 $(top_builddir)/tao/libTAO_Messaging.la \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  FtRtEvent/ClientORB/FTRT_ClientORB_Loader.h \
  FtRtEvent/ClientORB/FTRT_ClientORB_Initializer.h \
  FtRtEvent/ClientORB/FTRT_ClientORB_Interceptor.h \
  Channel_Clients_T.h \
  Channel_Clients_T.i \
  Channel_Clients_T.cpp


## Makefile.RTEvent.am 

BUILT_SOURCES += \
  RtecDefaultEventDataS_T.cpp \
  RtecDefaultEventDataC.cpp \
  RtecDefaultEventDataS.cpp \
  RtecDefaultEventDataC.h \
  RtecDefaultEventDataS.h \
  RtecDefaultEventDataS_T.h \
  RtecDefaultEventDataC.inl \
  RtecDefaultEventDataS.inl \
  RtecDefaultEventDataS_T.inl

CLEANFILES += \
  RtecDefaultEventDataS_T.cpp \
  RtecDefaultEventDataC.cpp \
  RtecDefaultEventDataS.cpp \
  RtecDefaultEventDataC.h \
  RtecDefaultEventDataS.h \
  RtecDefaultEventDataS_T.h \
  RtecDefaultEventDataC.inl \
  RtecDefaultEventDataS.inl \
  RtecDefaultEventDataS_T.inl

RtecDefaultEventDataS_T.cpp RtecDefaultEventDataC.cpp RtecDefaultEventDataS.cpp RtecDefaultEventDataC.h RtecDefaultEventDataS.h RtecDefaultEventDataS_T.h RtecDefaultEventDataC.inl RtecDefaultEventDataS.inl RtecDefaultEventDataS_T.inl: RtecDefaultEventData.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_RTEvent_Export -Wb,export_include=Event/event_export.h $(srcdir)/RtecDefaultEventData.idl

BUILT_SOURCES += \
  RtecEventCommS_T.cpp \
  RtecEventCommC.cpp \
  RtecEventCommS.cpp \
  RtecEventCommC.h \
  RtecEventCommS.h \
  RtecEventCommS_T.h \
  RtecEventCommC.inl \
  RtecEventCommS.inl \
  RtecEventCommS_T.inl

CLEANFILES += \
  RtecEventCommS_T.cpp \
  RtecEventCommC.cpp \
  RtecEventCommS.cpp \
  RtecEventCommC.h \
  RtecEventCommS.h \
  RtecEventCommS_T.h \
  RtecEventCommC.inl \
  RtecEventCommS.inl \
  RtecEventCommS_T.inl

RtecEventCommS_T.cpp RtecEventCommC.cpp RtecEventCommS.cpp RtecEventCommC.h RtecEventCommS.h RtecEventCommS_T.h RtecEventCommC.inl RtecEventCommS.inl RtecEventCommS_T.inl: RtecEventComm.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_RTEvent_Export -Wb,export_include=Event/event_export.h $(srcdir)/RtecEventComm.idl

BUILT_SOURCES += \
  RtecEventChannelAdminS_T.cpp \
  RtecEventChannelAdminC.cpp \
  RtecEventChannelAdminS.cpp \
  RtecEventChannelAdminC.h \
  RtecEventChannelAdminS.h \
  RtecEventChannelAdminS_T.h \
  RtecEventChannelAdminC.inl \
  RtecEventChannelAdminS.inl \
  RtecEventChannelAdminS_T.inl

CLEANFILES += \
  RtecEventChannelAdminS_T.cpp \
  RtecEventChannelAdminC.cpp \
  RtecEventChannelAdminS.cpp \
  RtecEventChannelAdminC.h \
  RtecEventChannelAdminS.h \
  RtecEventChannelAdminS_T.h \
  RtecEventChannelAdminC.inl \
  RtecEventChannelAdminS.inl \
  RtecEventChannelAdminS_T.inl

RtecEventChannelAdminS_T.cpp RtecEventChannelAdminC.cpp RtecEventChannelAdminS.cpp RtecEventChannelAdminC.h RtecEventChannelAdminS.h RtecEventChannelAdminS_T.h RtecEventChannelAdminC.inl RtecEventChannelAdminS.inl RtecEventChannelAdminS_T.inl: RtecEventChannelAdmin.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_RTEvent_Export -Wb,export_include=Event/event_export.h $(srcdir)/RtecEventChannelAdmin.idl

BUILT_SOURCES += \
  RtecUDPAdminS_T.cpp \
  RtecUDPAdminC.cpp \
  RtecUDPAdminS.cpp \
  RtecUDPAdminC.h \
  RtecUDPAdminS.h \
  RtecUDPAdminS_T.h \
  RtecUDPAdminC.inl \
  RtecUDPAdminS.inl \
  RtecUDPAdminS_T.inl

CLEANFILES += \
  RtecUDPAdminS_T.cpp \
  RtecUDPAdminC.cpp \
  RtecUDPAdminS.cpp \
  RtecUDPAdminC.h \
  RtecUDPAdminS.h \
  RtecUDPAdminS_T.h \
  RtecUDPAdminC.inl \
  RtecUDPAdminS.inl \
  RtecUDPAdminS_T.inl

RtecUDPAdminS_T.cpp RtecUDPAdminC.cpp RtecUDPAdminS.cpp RtecUDPAdminC.h RtecUDPAdminS.h RtecUDPAdminS_T.h RtecUDPAdminC.inl RtecUDPAdminS.inl RtecUDPAdminS_T.inl: RtecUDPAdmin.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_RTEvent_Export -Wb,export_include=Event/event_export.h $(srcdir)/RtecUDPAdmin.idl

RTEvent = \
  RtecDefaultEventDataC.cpp \
  RtecDefaultEventDataS.cpp \
  RtecEventCommC.cpp \
  RtecEventCommS.cpp \
  RtecEventChannelAdminC.cpp \
  RtecEventChannelAdminS.cpp \
  RtecUDPAdminC.cpp \
  RtecUDPAdminS.cpp \
  Event_Utilities.cpp \
  Event/ECG_Adapters.cpp \
  Event/ECG_Address_Server_Base.cpp \
  Event/ECG_CDR_Message_Receiver.cpp \
  Event/ECG_CDR_Message_Sender.cpp \
  Event/ECG_Complex_Address_Server.cpp \
  Event/ECG_Mcast_EH.cpp \
  Event/ECG_Mcast_Gateway.cpp \
  Event/ECG_Simple_Address_Server.cpp \
  Event/ECG_Simple_Mcast_EH.cpp \
  Event/ECG_UDP_EH.cpp \
  Event/ECG_UDP_Out_Endpoint.cpp \
  Event/ECG_UDP_Receiver.cpp \
  Event/ECG_UDP_Sender.cpp \
  Event/ECG_ConsumerEC_Control.cpp \
  Event/ECG_Reactive_ConsumerEC_Control.cpp \
  Event/ECG_Reconnect_ConsumerEC_Control.cpp \
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
  Event/EC_Masked_Type_Filter.cpp \
  Event/EC_MT_Dispatching.cpp \
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
  Event/EC_UDP_Admin.cpp

lib_LTLIBRARIES += libTAO_RTEvent.la

libTAO_RTEvent_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_RTEVENT_BUILD_DLL

libTAO_RTEvent_la_SOURCES = \
  $(RTEvent)

libTAO_RTEvent_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_RTEvent_la_LIBADD = \
 $(top_builddir)/tao/libTAO_Messaging.la \
 libTAO_Svc_Utils.la \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  Event/ECG_Adapters.h \
  Event/EC_Filter_Builder.h \
  Event/EC_Masked_Type_Filter.h \
  Event/EC_Factory.h \
  Event/ECG_Simple_Mcast_EH.h \
  RtecEventChannelAdminC.h \
  Event/EC_Basic_Factory.h \
  Event/EC_MT_Dispatching.h \
  RtecEventChannelAdminS.h \
  Event/EC_Per_Supplier_Filter.h \
  Event/EC_UDP_Admin.h \
  Event/EC_Scheduling_Strategy.h \
  Event/ECG_ConsumerEC_Control.h \
  Event/EC_Timeout_Generator.h \
  Event/EC_ConsumerAdmin.h \
  Event/EC_Dispatching.h \
  Event/EC_Type_Filter.h \
  Event/EC_Bitmask_Filter.h \
  Event/EC_Group_Scheduling.h \
  Event/EC_Reactive_Dispatching.h \
  Event/EC_Event_Channel.h \
  Event/EC_Null_Scheduling.h \
  Event/EC_Event_Channel_Base.h \
  Event/EC_Reactive_Timeout_Generator.h \
  Event/EC_QOS_Info.h \
  Event/EC_ProxySupplier.h \
  Event/EC_ProxyConsumer.h \
  RtecEventCommC.h \
  Event/EC_Conjunction_Filter.h \
  Event/ECG_UDP_Sender.h \
  Event/EC_Supplier_Filter.h \
  RtecEventCommS.h \
  Event/ECG_Address_Server_Base.h \
  RtecUDPAdminC.h \
  Event/EC_Disjunction_Filter.h \
  Event/EC_Gateway_IIOP.h \
  RtecUDPAdminS.h \
  Event/EC_Lifetime_Utils_T.h \
  Event/EC_Filter.h \
  Event/ECG_UDP_Receiver.h \
  Event/ECG_Complex_Address_Server.h \
  Event/EC_Timeout_Filter.h \
  Event_Utilities.h \
  Event/EC_SupplierAdmin.h \
  Event/ECG_Simple_Address_Server.h \
  Event/EC_Prefix_Filter_Builder.h \
  Event/EC_Supplier_Filter_Builder.h \
  Event/EC_Gateway_IIOP_Factory.h \
  Event/EC_Busy_Lock.h \
  Event/EC_Default_Factory.h \
  Event/ECG_Mcast_EH.h \
  Event/EC_Default_ProxySupplier.h \
  Event/ECG_UDP_Out_Endpoint.h \
  Event/EC_Reactive_SupplierControl.h \
  Event/EC_Default_ProxyConsumer.h \
  Event/EC_ObserverStrategy.h \
  Event/ECG_Reconnect_ConsumerEC_Control.h \
  Event/EC_Null_Factory.h \
  Event/ECG_CDR_Message_Receiver.h \
  Event/EC_SupplierControl.h \
  Event/EC_Proxy_Disconnector.h \
  Event/EC_Lifetime_Utils.h \
  Event/EC_Channel_Destroyer.h \
  Event/EC_And_Filter.h \
  Event/ECG_UDP_EH.h \
  Event/ECG_Reactive_ConsumerEC_Control.h \
  Event/EC_Gateway.h \
  Event/EC_Dispatching_Task.h \
  Event/EC_Reactive_ConsumerControl.h \
  Event/EC_Negation_Filter.h \
  RtecDefaultEventDataC.h \
  Event/ECG_Mcast_Gateway.h \
  Event/EC_Trivial_Supplier_Filter.h \
  Event/ECG_CDR_Message_Sender.h \
  Event/EC_Basic_Filter_Builder.h \
  RtecDefaultEventDataS.h \
  Event/EC_ConsumerControl.h \
  RtecDefaultEventDataS_T.h \
  RtecUDPAdminS_T.h \
  RtecEventChannelAdminS_T.h \
  Channel_Clients_T.h \
  RtecEventCommS_T.h \
  Event/EC_Filter_Builder.i \
  Event/EC_Masked_Type_Filter.i \
  Event/EC_Factory.i \
  Event/ECG_Simple_Mcast_EH.i \
  RtecEventChannelAdminC.inl \
  Event/EC_Basic_Factory.i \
  Event/EC_MT_Dispatching.i \
  RtecEventChannelAdminS.inl \
  Event/EC_Per_Supplier_Filter.i \
  Event/EC_Scheduling_Strategy.i \
  Event/ECG_ConsumerEC_Control.i \
  Event/EC_Timeout_Generator.i \
  Event/EC_ConsumerAdmin.i \
  Event/EC_Dispatching.i \
  Event/EC_Type_Filter.i \
  Event/EC_Bitmask_Filter.i \
  Event/EC_Group_Scheduling.inl \
  Event/EC_Reactive_Dispatching.i \
  Event/EC_Event_Channel.i \
  Event/EC_Null_Scheduling.i \
  Event/EC_Event_Channel_Base.i \
  Event/EC_Reactive_Timeout_Generator.i \
  Event/EC_QOS_Info.i \
  Event/EC_ProxySupplier.i \
  Event/EC_ProxyConsumer.i \
  RtecEventCommC.inl \
  Event/EC_Conjunction_Filter.i \
  Event/ECG_UDP_Sender.inl \
  Event/EC_Supplier_Filter.i \
  RtecEventCommS.inl \
  Event/ECG_Address_Server_Base.i \
  RtecUDPAdminC.inl \
  Event/EC_Disjunction_Filter.i \
  RtecUDPAdminS.inl \
  Event/EC_Lifetime_Utils_T.i \
  Event/EC_Filter.i \
  Event/ECG_UDP_Receiver.i \
  Event/ECG_Complex_Address_Server.i \
  Event/EC_Timeout_Filter.i \
  Event_Utilities.i \
  Event/EC_SupplierAdmin.i \
  Event/ECG_Simple_Address_Server.i \
  Event/EC_Prefix_Filter_Builder.i \
  Event/EC_Supplier_Filter_Builder.i \
  Event/EC_Gateway_IIOP_Factory.i \
  Event/EC_Busy_Lock.i \
  Event/EC_Default_Factory.i \
  Event/ECG_Mcast_EH.i \
  Event/EC_Default_ProxySupplier.i \
  Event/ECG_UDP_Out_Endpoint.i \
  Event/EC_Reactive_SupplierControl.i \
  Event/EC_Default_ProxyConsumer.i \
  Event/EC_ObserverStrategy.i \
  Event/EC_Null_Factory.i \
  Event/ECG_CDR_Message_Receiver.i \
  Event/EC_SupplierControl.i \
  Event/EC_Lifetime_Utils.i \
  Event/EC_And_Filter.i \
  Event/ECG_UDP_EH.i \
  Event/ECG_Reactive_ConsumerEC_Control.i \
  Event/EC_Dispatching_Task.i \
  Event/EC_Reactive_ConsumerControl.i \
  Event/EC_Negation_Filter.i \
  RtecDefaultEventDataC.inl \
  Event/ECG_Mcast_Gateway.i \
  Event/EC_Trivial_Supplier_Filter.i \
  Event/ECG_CDR_Message_Sender.i \
  Event/EC_Basic_Filter_Builder.i \
  RtecDefaultEventDataS.inl \
  Event/EC_ConsumerControl.i \
  RtecDefaultEventDataS_T.inl \
  RtecUDPAdminS_T.inl \
  RtecEventChannelAdminS_T.inl \
  Channel_Clients_T.i \
  RtecEventCommS_T.inl \
  RtecDefaultEventDataS_T.cpp \
  RtecEventCommS_T.cpp \
  RtecEventChannelAdminS_T.cpp \
  RtecUDPAdminS_T.cpp \
  Channel_Clients_T.cpp


## Makefile.FtRtEvent.am 

BUILT_SOURCES += \
  FTRTS_T.cpp \
  FTRTC.cpp \
  FTRTS.cpp \
  FTRTC.h \
  FTRTS.h \
  FTRTS_T.h \
  FTRTC.inl \
  FTRTS.inl \
  FTRTS_T.inl

CLEANFILES += \
  FTRTS_T.cpp \
  FTRTC.cpp \
  FTRTS.cpp \
  FTRTC.h \
  FTRTS.h \
  FTRTS_T.h \
  FTRTC.inl \
  FTRTS.inl \
  FTRTS_T.inl

FTRTS_T.cpp FTRTC.cpp FTRTS.cpp FTRTC.h FTRTS.h FTRTS_T.h FTRTC.inl FTRTS.inl FTRTS_T.inl: FTRT.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_FtRtEvent_Export -Wb,export_include=FtRtEvent/Utils/ftrtevent_export.h -GC $(srcdir)/FTRT.idl

BUILT_SOURCES += \
  FtRtecEventCommS_T.cpp \
  FtRtecEventCommC.cpp \
  FtRtecEventCommS.cpp \
  FtRtecEventCommC.h \
  FtRtecEventCommS.h \
  FtRtecEventCommS_T.h \
  FtRtecEventCommC.inl \
  FtRtecEventCommS.inl \
  FtRtecEventCommS_T.inl

CLEANFILES += \
  FtRtecEventCommS_T.cpp \
  FtRtecEventCommC.cpp \
  FtRtecEventCommS.cpp \
  FtRtecEventCommC.h \
  FtRtecEventCommS.h \
  FtRtecEventCommS_T.h \
  FtRtecEventCommC.inl \
  FtRtecEventCommS.inl \
  FtRtecEventCommS_T.inl

FtRtecEventCommS_T.cpp FtRtecEventCommC.cpp FtRtecEventCommS.cpp FtRtecEventCommC.h FtRtecEventCommS.h FtRtecEventCommS_T.h FtRtecEventCommC.inl FtRtecEventCommS.inl FtRtecEventCommS_T.inl: FtRtecEventComm.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_FtRtEvent_Export -Wb,export_include=FtRtEvent/Utils/ftrtevent_export.h -GC $(srcdir)/FtRtecEventComm.idl

BUILT_SOURCES += \
  FTRT_GroupManagerS_T.cpp \
  FTRT_GroupManagerC.cpp \
  FTRT_GroupManagerS.cpp \
  FTRT_GroupManagerC.h \
  FTRT_GroupManagerS.h \
  FTRT_GroupManagerS_T.h \
  FTRT_GroupManagerC.inl \
  FTRT_GroupManagerS.inl \
  FTRT_GroupManagerS_T.inl

CLEANFILES += \
  FTRT_GroupManagerS_T.cpp \
  FTRT_GroupManagerC.cpp \
  FTRT_GroupManagerS.cpp \
  FTRT_GroupManagerC.h \
  FTRT_GroupManagerS.h \
  FTRT_GroupManagerS_T.h \
  FTRT_GroupManagerC.inl \
  FTRT_GroupManagerS.inl \
  FTRT_GroupManagerS_T.inl

FTRT_GroupManagerS_T.cpp FTRT_GroupManagerC.cpp FTRT_GroupManagerS.cpp FTRT_GroupManagerC.h FTRT_GroupManagerS.h FTRT_GroupManagerS_T.h FTRT_GroupManagerC.inl FTRT_GroupManagerS.inl FTRT_GroupManagerS_T.inl: FTRT_GroupManager.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_FtRtEvent_Export -Wb,export_include=FtRtEvent/Utils/ftrtevent_export.h -GC $(srcdir)/FTRT_GroupManager.idl

BUILT_SOURCES += \
  FtRtecEventChannelAdminS_T.cpp \
  FtRtecEventChannelAdminC.cpp \
  FtRtecEventChannelAdminS.cpp \
  FtRtecEventChannelAdminC.h \
  FtRtecEventChannelAdminS.h \
  FtRtecEventChannelAdminS_T.h \
  FtRtecEventChannelAdminC.inl \
  FtRtecEventChannelAdminS.inl \
  FtRtecEventChannelAdminS_T.inl

CLEANFILES += \
  FtRtecEventChannelAdminS_T.cpp \
  FtRtecEventChannelAdminC.cpp \
  FtRtecEventChannelAdminS.cpp \
  FtRtecEventChannelAdminC.h \
  FtRtecEventChannelAdminS.h \
  FtRtecEventChannelAdminS_T.h \
  FtRtecEventChannelAdminC.inl \
  FtRtecEventChannelAdminS.inl \
  FtRtecEventChannelAdminS_T.inl

FtRtecEventChannelAdminS_T.cpp FtRtecEventChannelAdminC.cpp FtRtecEventChannelAdminS.cpp FtRtecEventChannelAdminC.h FtRtecEventChannelAdminS.h FtRtecEventChannelAdminS_T.h FtRtecEventChannelAdminC.inl FtRtecEventChannelAdminS.inl FtRtecEventChannelAdminS_T.inl: FtRtecEventChannelAdmin.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_FtRtEvent_Export -Wb,export_include=FtRtEvent/Utils/ftrtevent_export.h $(srcdir)/FtRtecEventChannelAdmin.idl

FtRtEvent = \
  FTRTC.cpp \
  FTRTS.cpp \
  FtRtecEventCommC.cpp \
  FtRtecEventCommS.cpp \
  FTRT_GroupManagerC.cpp \
  FTRT_GroupManagerS.cpp \
  FtRtecEventChannelAdminC.cpp \
  FtRtecEventChannelAdminS.cpp \
  FtRtEvent/Utils/Safe_InputCDR.cpp \
  FtRtEvent/Utils/FTEC_Gateway.cpp \
  FtRtEvent/Utils/Log.cpp \
  FtRtEvent/Utils/UUID.cpp

lib_LTLIBRARIES += libTAO_FtRtEvent.la

libTAO_FtRtEvent_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_FTRTEVENT_BUILD_DLL

libTAO_FtRtEvent_la_SOURCES = \
  $(FtRtEvent)

libTAO_FtRtEvent_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_FtRtEvent_la_LIBADD = \
 libTAO_CosNaming.la \
 $(top_builddir)/tao/libTAO_IORTable.la \
 libTAO_RTEvent.la \
 libTAO_Svc_Utils.la \
 $(top_builddir)/tao/libTAO_Messaging.la \
 libTAO_FTORB_Utils.la \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO_IORManip.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  FtRtEvent/Utils/Safe_InputCDR.h \
  FTRTC.h \
  FtRtecEventCommC.h \
  FTRTS.h \
  FtRtecEventCommS.h \
  FTRT_GroupManagerC.h \
  FTRT_GroupManagerS.h \
  FtRtecEventChannelAdminC.h \
  FtRtecEventChannelAdminS.h \
  FtRtEvent/Utils/FTEC_Gateway.h \
  FtRtEvent/Utils/UUID.h \
  FtRtEvent/Utils/Log.h \
  FTRTS_T.h \
  FTRT_GroupManagerS_T.h \
  Channel_Clients_T.h \
  FtRtecEventChannelAdminS_T.h \
  FtRtecEventCommS_T.h \
  FtRtEvent/Utils/Safe_InputCDR.inl \
  FTRTC.inl \
  FtRtecEventCommC.inl \
  FTRTS.inl \
  FtRtecEventCommS.inl \
  FTRT_GroupManagerC.inl \
  FTRT_GroupManagerS.inl \
  FtRtecEventChannelAdminC.inl \
  FtRtecEventChannelAdminS.inl \
  FtRtEvent/Utils/FTEC_Gateway.inl \
  FtRtEvent/Utils/UUID.inl \
  FtRtEvent/Utils/Log.inl \
  FTRTS_T.inl \
  FTRT_GroupManagerS_T.inl \
  Channel_Clients_T.i \
  FtRtecEventChannelAdminS_T.inl \
  FtRtecEventCommS_T.inl \
  FTRTS_T.cpp \
  FTRT_GroupManagerS_T.cpp \
  FtRtecEventCommS_T.cpp \
  FtRtecEventChannelAdminS_T.cpp \
  Channel_Clients_T.cpp


## Makefile.FTRT_EventChannel.am 

FTRT_EventChannel = \
  PortableGroup_Simple_DSC.cpp \
  FtRtEvent/EventChannel/Update_Manager.cpp \
  FtRtEvent/EventChannel/AMI_Primary_Replication_Strategy.cpp \
  FtRtEvent/EventChannel/AMI_Replication_Strategy.cpp \
  FtRtEvent/EventChannel/Basic_Replication_Strategy.cpp \
  FtRtEvent/EventChannel/Dynamic_Bitset.cpp \
  FtRtEvent/EventChannel/FTEC_Factory.cpp \
  FtRtEvent/EventChannel/FTEC_Become_Primary_Listener.cpp \
  FtRtEvent/EventChannel/FTEC_ConsumerAdmin.cpp \
  FtRtEvent/EventChannel/FTEC_Event_Channel.cpp \
  FtRtEvent/EventChannel/FTEC_Event_Channel_Impl.cpp \
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
  FtRtEvent/EventChannel/ObjectGroupManagerHandler.cpp \
  FtRtEvent/EventChannel/Identification_Service.cpp \
  FtRtEvent/EventChannel/Request_Context_Repository.cpp \
  FtRtEvent/EventChannel/ProxyConsumerStateWorker.cpp \
  FtRtEvent/EventChannel/ProxySupplierStateWorker.cpp \
  FtRtEvent/EventChannel/Replication_Service.cpp \
  FtRtEvent/EventChannel/Replication_Strategy.cpp \
  FtRtEvent/EventChannel/SCTP_Fault_Detector.cpp \
  FtRtEvent/EventChannel/Set_Update_Interceptor.cpp \
  FtRtEvent/EventChannel/UpdateableHandler.cpp \
  FtRtEvent/EventChannel/create_persistent_poa.cpp

lib_LTLIBRARIES += libTAO_FTRT_EventChannel.la

libTAO_FTRT_EventChannel_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs/orbsvcs/FtRtEvent/EventChannel \
  -DTAO_FTRTEC_BUILD_DLL

libTAO_FTRT_EventChannel_la_SOURCES = \
  $(FTRT_EventChannel)

libTAO_FTRT_EventChannel_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_FTRT_EventChannel_la_LIBADD = \
 $(top_builddir)/tao/libTAO_Utils.la \
 libTAO_FtRtEvent.la \
 libTAO_CosNaming.la \
 $(top_builddir)/tao/libTAO_IORTable.la \
 libTAO_RTEvent.la \
 libTAO_Svc_Utils.la \
 $(top_builddir)/tao/libTAO_Messaging.la \
 libTAO_FTORB_Utils.la \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO_IORManip.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  FtRtEvent/EventChannel/Fault_Detector_Loader.h \
  FtRtEvent/EventChannel/create_persistent_poa.h \
  FtRtEvent/EventChannel/Request_Context_Repository.h \
  FtRtEvent/EventChannel/FTEC_ProxySupplier.h \
  FtRtEvent/EventChannel/FTEC_ProxyConsumer.h \
  FtRtEvent/EventChannel/FTEC_Group_Manager.h \
  FtRtEvent/EventChannel/FTEC_SupplierAdmin.h \
  FtRtEvent/EventChannel/Identification_Service.h \
  FtRtEvent/EventChannel/GroupInfoPublisher.h \
  FtRtEvent/EventChannel/FTEC_Become_Primary_Listener.h \
  FtRtEvent/EventChannel/Set_Update_Interceptor.h \
  FtRtEvent/EventChannel/ObjectGroupManagerHandler.h \
  FtRtEvent/EventChannel/Dynamic_Bitset.h \
  FtRtEvent/EventChannel/ProxySupplierStateWorker.h \
  FtRtEvent/EventChannel/SCTP_Fault_Detector.h \
  FtRtEvent/EventChannel/Replication_Strategy.h \
  FtRtEvent/EventChannel/IOGR_Maker.h \
  FtRtEvent/EventChannel/FTEC_ORBInitializer.h \
  FtRtEvent/EventChannel/FtEventServiceInterceptor.h \
  FtRtEvent/EventChannel/Basic_Replication_Strategy.h \
  FtRtEvent/EventChannel/FTEC_Factory.h \
  FtRtEvent/EventChannel/FTEC_Event_Channel_Impl.h \
  FtRtEvent/EventChannel/UpdateableHandler.h \
  FtRtEvent/EventChannel/ForwardCtrlServerInterceptor.h \
  FtRtEvent/EventChannel/FT_ProxyAdmin_Base.h \
  FtRtEvent/EventChannel/Update_Manager.h \
  FtRtEvent/EventChannel/FTEC_ConsumerAdmin.h \
  FtRtEvent/EventChannel/Fault_Detector.h \
  FtRtEvent/EventChannel/Replication_Service.h \
  FtRtEvent/EventChannel/AMI_Primary_Replication_Strategy.h \
  FtRtEvent/EventChannel/AMI_Replication_Strategy.h \
  FtRtEvent/EventChannel/ProxyConsumerStateWorker.h \
  FtRtEvent/EventChannel/FTEC_Event_Channel.h \
  FtRtEvent/EventChannel/ftrtec_export.h \
  FtRtEvent/EventChannel/FT_ProxyAdmin_T.h \
  FtRtEvent/EventChannel/ConnectionHandler_T.h \
  FtRtEvent/EventChannel/Fault_Detector_T.h \
  FtRtEvent/EventChannel/Dynamic_Bitset.inl \
  FtRtEvent/EventChannel/ConnectionHandler_T.cpp \
  FtRtEvent/EventChannel/Fault_Detector_T.cpp \
  FtRtEvent/EventChannel/FT_ProxyAdmin_T.cpp


## Makefile.FT_ClientORB.am 

FT_ClientORB = \
  FaultTolerance/FT_ClientRequest_Interceptor.cpp \
  FaultTolerance/FT_ClientORBInitializer.cpp \
  FaultTolerance/FT_ClientPolicyFactory.cpp \
  FaultTolerance/FT_ClientPolicy_i.cpp \
  FaultTolerance/FT_ClientService_Activate.cpp \
  FaultTolerance/FT_Endpoint_Selector_Factory.cpp \
  FaultTolerance/FT_Invocation_Endpoint_Selectors.cpp \
  PortableGroup_Simple_DSC.cpp \
  FaultTolerance/FT_Service_Callbacks.cpp

lib_LTLIBRARIES += libTAO_FT_ClientORB.la

libTAO_FT_ClientORB_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_FT_CLIENTORB_BUILD_DLL

libTAO_FT_ClientORB_la_SOURCES = \
  $(FT_ClientORB)

libTAO_FT_ClientORB_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_FT_ClientORB_la_LIBADD = \
 libTAO_PortableGroup.la \
 libTAO_CosNaming.la \
 libTAO_Svc_Utils.la \
 $(top_builddir)/tao/libTAO_IORTable.la \
 $(top_builddir)/tao/libTAO_Messaging.la \
 libTAO_FTORB_Utils.la \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO_IORManip.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  FaultTolerance/FT_ClientRequest_Interceptor.h \
  FaultTolerance/FT_ClientPolicy_i.h \
  FaultTolerance/FT_Invocation_Endpoint_Selectors.h \
  FaultTolerance/FT_ClientORBInitializer.h \
  FaultTolerance/FT_ClientPolicyFactory.h \
  FaultTolerance/FT_Service_Callbacks.h \
  FaultTolerance/FT_Endpoint_Selector_Factory.h \
  FaultTolerance/FT_ClientService_Activate.h \
  Channel_Clients_T.h \
  FaultTolerance/FT_ClientPolicy_i.inl \
  FaultTolerance/FT_Invocation_Endpoint_Selectors.inl \
  FaultTolerance/FT_Service_Callbacks.i \
  Channel_Clients_T.i \
  Channel_Clients_T.cpp


## Makefile.FT_ServerORB.am 

FT_ServerORB = \
  FaultTolerance/FT_ServerORBInitializer.cpp \
  FaultTolerance/FT_ServerPolicyFactory.cpp \
  FaultTolerance/FT_ServerPolicy_i.cpp \
  FaultTolerance/FT_ServerRequest_Interceptor.cpp \
  PortableGroup_Simple_DSC.cpp \
  FaultTolerance/FT_ServerService_Activate.cpp

lib_LTLIBRARIES += libTAO_FT_ServerORB.la

libTAO_FT_ServerORB_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_FT_SERVERORB_BUILD_DLL

libTAO_FT_ServerORB_la_SOURCES = \
  $(FT_ServerORB)

libTAO_FT_ServerORB_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_FT_ServerORB_la_LIBADD = \
 libTAO_PortableGroup.la \
 libTAO_CosNaming.la \
 libTAO_Svc_Utils.la \
 $(top_builddir)/tao/libTAO_IORTable.la \
 $(top_builddir)/tao/libTAO_Messaging.la \
 libTAO_FTORB_Utils.la \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO_IORManip.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  Channel_Clients_T.h \
  FaultTolerance/FT_ServerPolicyFactory.h \
  FaultTolerance/FT_ServerRequest_Interceptor.h \
  FaultTolerance/FT_ServerPolicy_i.h \
  FaultTolerance/FT_ServerORBInitializer.h \
  FaultTolerance/FT_ServerService_Activate.h \
  Channel_Clients_T.i \
  FaultTolerance/FT_ServerPolicy_i.inl \
  Channel_Clients_T.cpp


## Makefile.FaultTolerance.am 

BUILT_SOURCES += \
  FT_CORBAS_T.cpp \
  FT_CORBAC.cpp \
  FT_CORBAS.cpp \
  FT_CORBAC.h \
  FT_CORBAS.h \
  FT_CORBAS_T.h \
  FT_CORBAC.inl \
  FT_CORBAS.inl \
  FT_CORBAS_T.inl

CLEANFILES += \
  FT_CORBAS_T.cpp \
  FT_CORBAC.cpp \
  FT_CORBAS.cpp \
  FT_CORBAC.h \
  FT_CORBAS.h \
  FT_CORBAS_T.h \
  FT_CORBAC.inl \
  FT_CORBAS.inl \
  FT_CORBAS_T.inl

FT_CORBAS_T.cpp FT_CORBAC.cpp FT_CORBAS.cpp FT_CORBAC.h FT_CORBAS.h FT_CORBAS_T.h FT_CORBAC.inl FT_CORBAS.inl FT_CORBAS_T.inl: FT_CORBA.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_FT_Export -Wb,export_include=FaultTolerance/fault_tol_export.h -Wb,skel_export_include=tao/PortableServer/PolicyS.h $(srcdir)/FT_CORBA.idl

BUILT_SOURCES += \
  FT_ReplicaS_T.cpp \
  FT_ReplicaC.cpp \
  FT_ReplicaS.cpp \
  FT_ReplicaC.h \
  FT_ReplicaS.h \
  FT_ReplicaS_T.h \
  FT_ReplicaC.inl \
  FT_ReplicaS.inl \
  FT_ReplicaS_T.inl

CLEANFILES += \
  FT_ReplicaS_T.cpp \
  FT_ReplicaC.cpp \
  FT_ReplicaS.cpp \
  FT_ReplicaC.h \
  FT_ReplicaS.h \
  FT_ReplicaS_T.h \
  FT_ReplicaC.inl \
  FT_ReplicaS.inl \
  FT_ReplicaS_T.inl

FT_ReplicaS_T.cpp FT_ReplicaC.cpp FT_ReplicaS.cpp FT_ReplicaC.h FT_ReplicaS.h FT_ReplicaS_T.h FT_ReplicaC.inl FT_ReplicaS.inl FT_ReplicaS_T.inl: FT_Replica.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_FT_Export -Wb,export_include=FaultTolerance/fault_tol_export.h -Wb,skel_export_include=tao/PortableServer/PolicyS.h $(srcdir)/FT_Replica.idl

BUILT_SOURCES += \
  FT_NotifierS_T.cpp \
  FT_NotifierC.cpp \
  FT_NotifierS.cpp \
  FT_NotifierC.h \
  FT_NotifierS.h \
  FT_NotifierS_T.h \
  FT_NotifierC.inl \
  FT_NotifierS.inl \
  FT_NotifierS_T.inl

CLEANFILES += \
  FT_NotifierS_T.cpp \
  FT_NotifierC.cpp \
  FT_NotifierS.cpp \
  FT_NotifierC.h \
  FT_NotifierS.h \
  FT_NotifierS_T.h \
  FT_NotifierC.inl \
  FT_NotifierS.inl \
  FT_NotifierS_T.inl

FT_NotifierS_T.cpp FT_NotifierC.cpp FT_NotifierS.cpp FT_NotifierC.h FT_NotifierS.h FT_NotifierS_T.h FT_NotifierC.inl FT_NotifierS.inl FT_NotifierS_T.inl: FT_Notifier.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_FT_Export -Wb,export_include=FaultTolerance/fault_tol_export.h -Wb,skel_export_include=tao/PortableServer/PolicyS.h $(srcdir)/FT_Notifier.idl

BUILT_SOURCES += \
  FT_FaultDetectorFactoryS_T.cpp \
  FT_FaultDetectorFactoryC.cpp \
  FT_FaultDetectorFactoryS.cpp \
  FT_FaultDetectorFactoryC.h \
  FT_FaultDetectorFactoryS.h \
  FT_FaultDetectorFactoryS_T.h \
  FT_FaultDetectorFactoryC.inl \
  FT_FaultDetectorFactoryS.inl \
  FT_FaultDetectorFactoryS_T.inl

CLEANFILES += \
  FT_FaultDetectorFactoryS_T.cpp \
  FT_FaultDetectorFactoryC.cpp \
  FT_FaultDetectorFactoryS.cpp \
  FT_FaultDetectorFactoryC.h \
  FT_FaultDetectorFactoryS.h \
  FT_FaultDetectorFactoryS_T.h \
  FT_FaultDetectorFactoryC.inl \
  FT_FaultDetectorFactoryS.inl \
  FT_FaultDetectorFactoryS_T.inl

FT_FaultDetectorFactoryS_T.cpp FT_FaultDetectorFactoryC.cpp FT_FaultDetectorFactoryS.cpp FT_FaultDetectorFactoryC.h FT_FaultDetectorFactoryS.h FT_FaultDetectorFactoryS_T.h FT_FaultDetectorFactoryC.inl FT_FaultDetectorFactoryS.inl FT_FaultDetectorFactoryS_T.inl: FT_FaultDetectorFactory.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_FT_Export -Wb,export_include=FaultTolerance/fault_tol_export.h -Wb,skel_export_include=tao/PortableServer/PolicyS.h $(srcdir)/FT_FaultDetectorFactory.idl

BUILT_SOURCES += \
  FT_ReplicationManagerS_T.cpp \
  FT_ReplicationManagerC.cpp \
  FT_ReplicationManagerS.cpp \
  FT_ReplicationManagerC.h \
  FT_ReplicationManagerS.h \
  FT_ReplicationManagerS_T.h \
  FT_ReplicationManagerC.inl \
  FT_ReplicationManagerS.inl \
  FT_ReplicationManagerS_T.inl

CLEANFILES += \
  FT_ReplicationManagerS_T.cpp \
  FT_ReplicationManagerC.cpp \
  FT_ReplicationManagerS.cpp \
  FT_ReplicationManagerC.h \
  FT_ReplicationManagerS.h \
  FT_ReplicationManagerS_T.h \
  FT_ReplicationManagerC.inl \
  FT_ReplicationManagerS.inl \
  FT_ReplicationManagerS_T.inl

FT_ReplicationManagerS_T.cpp FT_ReplicationManagerC.cpp FT_ReplicationManagerS.cpp FT_ReplicationManagerC.h FT_ReplicationManagerS.h FT_ReplicationManagerS_T.h FT_ReplicationManagerC.inl FT_ReplicationManagerS.inl FT_ReplicationManagerS_T.inl: FT_ReplicationManager.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_FT_Export -Wb,export_include=FaultTolerance/fault_tol_export.h -Wb,skel_export_include=tao/PortableServer/PolicyS.h $(srcdir)/FT_ReplicationManager.idl

FaultTolerance = \
  FT_CORBAC.cpp \
  FT_CORBAS.cpp \
  FT_ReplicaS.cpp \
  FT_ReplicaC.cpp \
  FT_NotifierS.cpp \
  FT_NotifierC.cpp \
  FT_FaultDetectorFactoryS.cpp \
  FT_FaultDetectorFactoryC.cpp \
  FT_ReplicationManagerS.cpp \
  FT_ReplicationManagerC.cpp

lib_LTLIBRARIES += libTAO_FaultTolerance.la

libTAO_FaultTolerance_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_FT_BUILD_DLL

libTAO_FaultTolerance_la_SOURCES = \
  $(FaultTolerance)

libTAO_FaultTolerance_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_FaultTolerance_la_LIBADD = \
 libTAO_PortableGroup.la \
 libTAO_CosNaming.la \
 $(top_builddir)/tao/libTAO_Messaging.la \
 $(top_builddir)/tao/libTAO_IORTable.la \
 libTAO_FT_ServerORB.la \
 libTAO_FT_ClientORB.la \
 libTAO_FTORB_Utils.la \
 $(top_builddir)/tao/libTAO_IORManip.la \
 libTAO_CosNotification.la \
 libTAO_Svc_Utils.la \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO_DynamicAny.la \
 libTAO_ETCL.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  FT_FaultDetectorFactoryS_T.h \
  Channel_Clients_T.h \
  FT_ReplicationManagerS_T.h \
  FT_ReplicaS_T.h \
  FT_NotifierS_T.h \
  FT_CORBAS_T.h \
  FT_FaultDetectorFactoryS_T.inl \
  Channel_Clients_T.i \
  FT_ReplicationManagerS_T.inl \
  FT_ReplicaS_T.inl \
  FT_NotifierS_T.inl \
  FT_CORBAS_T.inl \
  FT_CORBAS_T.cpp \
  FT_ReplicaS_T.cpp \
  FT_ReplicationManagerS_T.cpp \
  FT_NotifierS_T.cpp \
  FT_FaultDetectorFactoryS_T.cpp \
  Channel_Clients_T.cpp


## Makefile.IFRService.am 

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
  IFRService/IFR_BaseS.cpp \
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
  IFRService/IFR_ComponentsS.cpp \
  IFRService/IFR_BasicS.cpp \
  IFRService/IFR_Service_Loader.cpp \
  IFRService/IFR_ExtendedS.cpp \
  IFRService/IFR_Service_Utils.cpp \
  IFRService/InterfaceAttrExtension_i.cpp \
  IFRService/IRObject_i.cpp \
  IFRService/InterfaceDef_i.cpp \
  IFRService/Options.cpp \
  IFRService/LocalInterfaceDef_i.cpp \
  IFRService/ModuleDef_i.cpp \
  IFRService/NativeDef_i.cpp \
  IFRService/OperationDef_i.cpp \
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
  -DTAO_IFRSERVICE_BUILD_DLL

libTAO_IFRService_la_SOURCES = \
  $(IFRService)

libTAO_IFRService_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_IFRService_la_LIBADD = \
 $(top_builddir)/tao/libTAO_IORTable.la \
 libTAO_Svc_Utils.la \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO_TypeCodeFactory.la \
 $(top_builddir)/tao/libTAO_IFR_Client.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  IFRService/ComponentRepository_i.h \
  IFRService/ExtAbstractInterfaceDef_i.h \
  IFRService/ComponentContainer_i.h \
  IFRService/InterfaceAttrExtension_i.h \
  IFRService/IFR_BaseS.h \
  IFRService/EventPortDef_i.h \
  IFRService/FinderDef_i.h \
  IFRService/EmitsDef_i.h \
  IFRService/Container_i.h \
  IFRService/AttributeDef_i.h \
  IFRService/ConsumesDef_i.h \
  IFRService/ProvidesDef_i.h \
  IFRService/InterfaceDef_i.h \
  IFRService/OperationDef_i.h \
  IFRService/FactoryDef_i.h \
  IFRService/EnumDef_i.h \
  IFRService/ExtAttributeDef_i.h \
  IFRService/FixedDef_i.h \
  IFRService/ExtLocalInterfaceDef_i.h \
  IFRService/AbstractInterfaceDef_i.h \
  IFRService/ExtInterfaceDef_i.h \
  IFRService/LocalInterfaceDef_i.h \
  IFRService/UnionDef_i.h \
  IFRService/AliasDef_i.h \
  IFRService/PublishesDef_i.h \
  IFRService/ComponentDef_i.h \
  IFRService/ComponentModuleDef_i.h \
  IFRService/ValueMemberDef_i.h \
  IFRService/ValueDef_i.h \
  IFRService/TypedefDef_i.h \
  IFRService/Options.h \
  IFRService/Contained_i.h \
  IFRService/PrimitiveDef_i.h \
  IFRService/EventDef_i.h \
  IFRService/ExceptionDef_i.h \
  IFRService/IFR_Service_Utils.h \
  IFRService/ArrayDef_i.h \
  IFRService/IFR_ExtendedS.h \
  IFRService/StructDef_i.h \
  IFRService/HomeDef_i.h \
  IFRService/ConstantDef_i.h \
  IFRService/IFR_ComponentsS.h \
  IFRService/IFR_BasicS.h \
  IFRService/ModuleDef_i.h \
  IFRService/WstringDef_i.h \
  IFRService/StringDef_i.h \
  IFRService/Repository_i.h \
  IFRService/SequenceDef_i.h \
  IFRService/IFR_Service_Loader.h \
  IFRService/IDLType_i.h \
  IFRService/ExtValueDef_i.h \
  IFRService/ValueBoxDef_i.h \
  IFRService/NativeDef_i.h \
  IFRService/IRObject_i.h \
  IFRService/UsesDef_i.h \
  Channel_Clients_T.h \
  IFRService/IFR_BaseS.i \
  IFRService/IFR_ExtendedS.i \
  IFRService/IFR_ComponentsS.i \
  IFRService/IFR_BasicS.i \
  Channel_Clients_T.i \
  Channel_Clients_T.cpp


## Makefile.RTCORBAEvent.am 

RTCORBAEvent = \
  Event/EC_RTCORBA_Dispatching.cpp \
  Event/EC_RTCORBA_Factory.cpp

lib_LTLIBRARIES += libTAO_RTCORBAEvent.la

libTAO_RTCORBAEvent_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_RTCORBAEVENT_BUILD_DLL

libTAO_RTCORBAEvent_la_SOURCES = \
  $(RTCORBAEvent)

libTAO_RTCORBAEvent_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_RTCORBAEvent_la_LIBADD = \
 $(top_builddir)/tao/libTAO_RTPortableServer.la \
 $(top_builddir)/tao/libTAO_RTCORBA.la \
 libTAO_CosNaming.la \
 $(top_builddir)/tao/libTAO_IORTable.la \
 libTAO_RTEvent.la \
 libTAO_Svc_Utils.la \
 $(top_builddir)/tao/libTAO_Messaging.la \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  Channel_Clients_T.h \
  Event/EC_RTCORBA_Dispatching.h \
  Event/EC_RTCORBA_Factory.h \
  Channel_Clients_T.i \
  Event/EC_RTCORBA_Dispatching.inl \
  Event/EC_RTCORBA_Factory.inl \
  Channel_Clients_T.cpp


## Makefile.RTCosScheduling.am 

BUILT_SOURCES += \
  RTCosSchedulingS_T.cpp \
  RTCosSchedulingC.cpp \
  RTCosSchedulingS.cpp \
  RTCosSchedulingC.h \
  RTCosSchedulingS.h \
  RTCosSchedulingS_T.h \
  RTCosSchedulingC.inl \
  RTCosSchedulingS.inl \
  RTCosSchedulingS_T.inl

CLEANFILES += \
  RTCosSchedulingS_T.cpp \
  RTCosSchedulingC.cpp \
  RTCosSchedulingS.cpp \
  RTCosSchedulingC.h \
  RTCosSchedulingS.h \
  RTCosSchedulingS_T.h \
  RTCosSchedulingC.inl \
  RTCosSchedulingS.inl \
  RTCosSchedulingS_T.inl

RTCosSchedulingS_T.cpp RTCosSchedulingC.cpp RTCosSchedulingS.cpp RTCosSchedulingC.h RTCosSchedulingS.h RTCosSchedulingS_T.h RTCosSchedulingC.inl RTCosSchedulingS.inl RTCosSchedulingS_T.inl: RTCosScheduling.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Gd -Wb,export_macro=TAO_RTCosScheduling_Export -Wb,export_include=RTCosScheduling/RTCosScheduling_export.h $(srcdir)/RTCosScheduling.idl

RTCosScheduling = \
  RTCosSchedulingC.cpp \
  RTCosSchedulingS.cpp \
  RTCosScheduling/RTCosScheduling_ClientScheduler_i.cpp \
  RTCosScheduling/RTCosScheduling_PCP_Manager.cpp \
  RTCosScheduling/RTCosScheduling_ServerScheduler_i.cpp

lib_LTLIBRARIES += libTAO_RTCosScheduling.la

libTAO_RTCosScheduling_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_RTCOSSCHEDULING_BUILD_DLL

libTAO_RTCosScheduling_la_SOURCES = \
  $(RTCosScheduling)

libTAO_RTCosScheduling_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_RTCosScheduling_la_LIBADD = \
 $(top_builddir)/tao/libTAO_RTPortableServer.la \
 $(top_builddir)/tao/libTAO_RTCORBA.la \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  RTCosScheduling/RTCosScheduling_ServerScheduler_i.h \
  RTCosScheduling/RTCosScheduling_ClientScheduler_i.h \
  RTCosSchedulingC.h \
  RTCosSchedulingS.h \
  RTCosScheduling/RTCosScheduling_PCP_Manager.h \
  RTCosSchedulingS_T.h \
  RTCosScheduling/RTCosScheduling_ServerScheduler_i.i \
  RTCosScheduling/RTCosScheduling_ClientScheduler_i.i \
  RTCosSchedulingC.inl \
  RTCosSchedulingS.inl \
  RTCosScheduling/RTCosScheduling_PCP_Manager.i \
  RTCosSchedulingS_T.inl \
  RTCosSchedulingS_T.cpp


## Makefile.RTSched.am 

BUILT_SOURCES += \
  RtecSchedulerS_T.cpp \
  RtecSchedulerC.cpp \
  RtecSchedulerS.cpp \
  RtecSchedulerC.h \
  RtecSchedulerS.h \
  RtecSchedulerS_T.h \
  RtecSchedulerC.inl \
  RtecSchedulerS.inl \
  RtecSchedulerS_T.inl

CLEANFILES += \
  RtecSchedulerS_T.cpp \
  RtecSchedulerC.cpp \
  RtecSchedulerS.cpp \
  RtecSchedulerC.h \
  RtecSchedulerS.h \
  RtecSchedulerS_T.h \
  RtecSchedulerC.inl \
  RtecSchedulerS.inl \
  RtecSchedulerS_T.inl

RtecSchedulerS_T.cpp RtecSchedulerC.cpp RtecSchedulerS.cpp RtecSchedulerC.h RtecSchedulerS.h RtecSchedulerS_T.h RtecSchedulerC.inl RtecSchedulerS.inl RtecSchedulerS_T.inl: RtecScheduler.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_RTSched_Export -Wb,export_include=Sched/sched_export.h $(srcdir)/RtecScheduler.idl

Sched = \
  RtecSchedulerC.cpp \
  RtecSchedulerS.cpp \
  Scheduler_Factory.cpp \
  Runtime_Scheduler.cpp \
  Scheduler_Utilities.cpp \
  Sched/Reconfig_Sched_Utils.cpp \
  Sched/Config_Scheduler.cpp \
  Sched/DynSched.cpp \
  Sched/Reconfig_Scheduler.cpp \
  Sched/SchedEntry.cpp \
  Sched/Scheduler.cpp \
  Sched/Scheduler_Generic.cpp \
  Sched/Strategy_Scheduler.cpp

lib_LTLIBRARIES += libTAO_RTSched.la

libTAO_RTSched_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_RTSCHED_BUILD_DLL

libTAO_RTSched_la_SOURCES = \
  $(Sched)

libTAO_RTSched_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_RTSched_la_LIBADD = \
 libTAO_CosNaming.la \
 libTAO_Svc_Utils.la \
 $(top_builddir)/tao/libTAO_Messaging.la \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO_IORTable.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  Runtime_Scheduler.h \
  Sched/Config_Scheduler.h \
  Sched/Reconfig_Scheduler.h \
  Scheduler_Factory.h \
  Sched/Reconfig_Sched_Utils.h \
  RtecSchedulerC.h \
  Sched/Scheduler.h \
  RtecSchedulerS.h \
  Scheduler_Utilities.h \
  Sched/Scheduler_Generic.h \
  Sched/SchedEntry.h \
  Sched/DynSched.h \
  Sched/Strategy_Scheduler.h \
  Channel_Clients_T.h \
  RtecSchedulerS_T.h \
  Runtime_Scheduler.i \
  Sched/Config_Scheduler.i \
  Sched/Reconfig_Scheduler.i \
  Scheduler_Factory.i \
  Sched/Reconfig_Sched_Utils.i \
  RtecSchedulerC.inl \
  Sched/Scheduler.i \
  RtecSchedulerS.inl \
  Scheduler_Utilities.i \
  Sched/Scheduler_Generic.i \
  Sched/SchedEntry.i \
  Sched/DynSched.i \
  Sched/Strategy_Scheduler.i \
  Channel_Clients_T.i \
  RtecSchedulerS_T.inl \
  RtecSchedulerS_T.cpp \
  Channel_Clients_T.cpp


## Makefile.RTOLDEvent.am 

RTOLDEvent = \
  Event/BCU.cpp \
  Event/Dispatching_Modules.cpp \
  Event/Event_Channel.cpp \
  Event/Event_Manip.cpp \
  Event/GPlot_File.cpp \
  Event/Local_ESTypes.cpp \
  Event/Memory_Pools.cpp \
  Event/Module_Factory.cpp \
  Event/ReactorTask.cpp \
  Event/RT_Task.cpp \
  Event/Timer_Module.cpp

lib_LTLIBRARIES += libTAO_RTOLDEvent.la

libTAO_RTOLDEvent_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_RTOLDEVENT_BUILD_DLL

libTAO_RTOLDEvent_la_SOURCES = \
  $(RTOLDEvent)

libTAO_RTOLDEvent_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_RTOLDEvent_la_LIBADD = \
 libTAO_RTSched.la \
 libTAO_CosNaming.la \
 $(top_builddir)/tao/libTAO_IORTable.la \
 libTAO_RTEvent.la \
 libTAO_Svc_Utils.la \
 $(top_builddir)/tao/libTAO_Messaging.la \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  Event/ReactorTask.h \
  Event/RT_Task.h \
  Event/BCU.h \
  Event/Dispatching_Modules.h \
  Event/Local_ESTypes.h \
  Event/Memory_Pools.h \
  Event/Module_Factory.h \
  Event/Timer_Module.h \
  Event/GPlot_File.h \
  Event/Event_Channel.h \
  Event/Event_Manip.h \
  Channel_Clients_T.h \
  Event/RT_Task.i \
  Event/Dispatching_Modules.i \
  Event/Local_ESTypes.i \
  Event/Memory_Pools.i \
  Event/Timer_Module.i \
  Event/GPlot_File.i \
  Event/Event_Channel.i \
  Event/Event_Manip.i \
  Channel_Clients_T.i \
  Channel_Clients_T.cpp


## Makefile.RTEventLogAdmin.am 

BUILT_SOURCES += \
  RTEventLogAdminS_T.cpp \
  RTEventLogAdminC.cpp \
  RTEventLogAdminS.cpp \
  RTEventLogAdminC.h \
  RTEventLogAdminS.h \
  RTEventLogAdminS_T.h \
  RTEventLogAdminC.inl \
  RTEventLogAdminS.inl \
  RTEventLogAdminS_T.inl

CLEANFILES += \
  RTEventLogAdminS_T.cpp \
  RTEventLogAdminC.cpp \
  RTEventLogAdminS.cpp \
  RTEventLogAdminC.h \
  RTEventLogAdminS.h \
  RTEventLogAdminS_T.h \
  RTEventLogAdminC.inl \
  RTEventLogAdminS.inl \
  RTEventLogAdminS_T.inl

RTEventLogAdminS_T.cpp RTEventLogAdminC.cpp RTEventLogAdminS.cpp RTEventLogAdminC.h RTEventLogAdminS.h RTEventLogAdminS_T.h RTEventLogAdminC.inl RTEventLogAdminS.inl RTEventLogAdminS_T.inl: RTEventLogAdmin.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_RTEventLog_Export -Wb,export_include=Log/rteventlog_export.h $(srcdir)/RTEventLogAdmin.idl

RTEventLog = \
  RTEventLogAdminC.cpp \
  RTEventLogAdminS.cpp \
  Log/RTEventLogFactory_i.cpp \
  Log/RTEventLog_i.cpp \
  Log/RTEventLogConsumer.cpp \
  Log/RTEventLogNotification.cpp

lib_LTLIBRARIES += libTAO_RTEventLogAdmin.la

libTAO_RTEventLogAdmin_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_RTEVENTLOG_BUILD_DLL

libTAO_RTEventLogAdmin_la_SOURCES = \
  $(RTEventLog)

libTAO_RTEventLogAdmin_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_RTEventLogAdmin_la_LIBADD = \
 libTAO_DsLogAdmin.la \
 $(top_builddir)/tao/libTAO_DynamicAny.la \
 libTAO_ETCL.la \
 libTAO_RTOLDEvent.la \
 libTAO_RTSched.la \
 libTAO_RTEvent.la \
 libTAO_CosNaming.la \
 libTAO_Svc_Utils.la \
 $(top_builddir)/tao/libTAO_Messaging.la \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO_IORTable.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  Channel_Clients_T.h \
  RTEventLogAdminS_T.h \
  Log/RTEventLogNotification.h \
  Log/RTEventLog_i.h \
  RTEventLogAdminC.h \
  RTEventLogAdminS.h \
  Log/RTEventLogConsumer.h \
  Log/RTEventLogFactory_i.h \
  Channel_Clients_T.i \
  RTEventLogAdminS_T.inl \
  RTEventLogAdminS_T.cpp \
  Channel_Clients_T.cpp


## Makefile.RTKokyuEvent.am 

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
  -I$(ACE_ROOT)/Kokyu \
  -DTAO_RTKOKYUEVENT_BUILD_DLL

libTAO_RTKokyuEvent_la_SOURCES = \
  $(RTKokyuEvent)

libTAO_RTKokyuEvent_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_RTKokyuEvent_la_LIBADD = \
 -lKokyu libTAO_RTSched.la \
 libTAO_CosNaming.la \
 $(top_builddir)/tao/libTAO_IORTable.la \
 libTAO_RTEvent.la \
 libTAO_Svc_Utils.la \
 $(top_builddir)/tao/libTAO_Messaging.la \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  Event/EC_Kokyu_Scheduling.h \
  Event/EC_Kokyu_Dispatching.h \
  Event/EC_Kokyu_Filter.h \
  Event/EC_Kokyu_Filter_Builder.h \
  Event/EC_Kokyu_Factory.h \
  Channel_Clients_T.h \
  Event/EC_Kokyu_Scheduling.i \
  Event/EC_Kokyu_Dispatching.i \
  Event/EC_Kokyu_Filter.i \
  Event/EC_Kokyu_Filter_Builder.i \
  Event/EC_Kokyu_Factory.i \
  Channel_Clients_T.i \
  Channel_Clients_T.cpp


## Makefile.RTSchedEvent.am 

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
  -DTAO_RTSCHEDEVENT_BUILD_DLL

libTAO_RTSchedEvent_la_SOURCES = \
  $(RTSchedEvent)

libTAO_RTSchedEvent_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_RTSchedEvent_la_LIBADD = \
 libTAO_RTSched.la \
 libTAO_CosNaming.la \
 $(top_builddir)/tao/libTAO_IORTable.la \
 libTAO_RTEvent.la \
 libTAO_Svc_Utils.la \
 $(top_builddir)/tao/libTAO_Messaging.la \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  Event/EC_Sched_Filter.h \
  Event/EC_Priority_Scheduling.h \
  Event/EC_Gateway_Sched.h \
  Event/EC_Priority_Dispatching.h \
  Event/EC_Sched_Factory.h \
  Event/EC_Sched_Filter_Builder.h \
  Channel_Clients_T.h \
  Event/EC_Sched_Filter.i \
  Event/EC_Priority_Scheduling.i \
  Event/EC_Priority_Dispatching.i \
  Event/EC_Sched_Factory.i \
  Event/EC_Sched_Filter_Builder.i \
  Channel_Clients_T.i \
  Channel_Clients_T.cpp


## Makefile.RT_Notification.am 

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
  -DTAO_RT_NOTIFY_BUILD_DLL

libTAO_RT_Notification_la_SOURCES = \
  $(RT_Notify)

libTAO_RT_Notification_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_RT_Notification_la_LIBADD = \
 libTAO_CosNotification.la \
 libTAO_ETCL.la \
 $(top_builddir)/tao/libTAO_RTCORBA.la \
 $(top_builddir)/tao/libTAO_DynamicAny.la \
 libTAO_Svc_Utils.la \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  Channel_Clients_T.h \
  Notify/RT_Builder.h \
  Notify/RT_POA_Helper.h \
  Notify/RT_Factory.h \
  Notify/RT_Notify_Service.h \
  Notify/RT_Properties.h \
  Notify/Structured/RT_StructuredProxyPushSupplier.h \
  Channel_Clients_T.i \
  Notify/RT_Builder.inl \
  Notify/RT_POA_Helper.inl \
  Notify/RT_Factory.inl \
  Notify/RT_Notify_Service.inl \
  Notify/RT_Properties.inl \
  Notify/Structured/RT_StructuredProxyPushSupplier.inl \
  Channel_Clients_T.cpp


## Makefile.Security.am 

BUILT_SOURCES += \
  CSIS_T.cpp \
  CSIC.cpp \
  CSIC.h \
  CSIS.h \
  CSIS_T.h \
  CSIC.inl \
  CSIS.inl \
  CSIS_T.inl

CLEANFILES += \
  CSIS_T.cpp \
  CSIC.cpp \
  CSIC.h \
  CSIS.h \
  CSIS_T.h \
  CSIC.inl \
  CSIS.inl \
  CSIS_T.inl

CSIS_T.cpp CSIC.cpp CSIC.h CSIS.h CSIS_T.h CSIC.inl CSIS.inl CSIS_T.inl: CSI.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_Security_Export -Wb,export_include=Security/security_export.h $(srcdir)/CSI.idl

BUILT_SOURCES += \
  CSIIOPS_T.cpp \
  CSIIOPC.cpp \
  CSIIOPC.h \
  CSIIOPS.h \
  CSIIOPS_T.h \
  CSIIOPC.inl \
  CSIIOPS.inl \
  CSIIOPS_T.inl

CLEANFILES += \
  CSIIOPS_T.cpp \
  CSIIOPC.cpp \
  CSIIOPC.h \
  CSIIOPS.h \
  CSIIOPS_T.h \
  CSIIOPC.inl \
  CSIIOPS.inl \
  CSIIOPS_T.inl

CSIIOPS_T.cpp CSIIOPC.cpp CSIIOPC.h CSIIOPS.h CSIIOPS_T.h CSIIOPC.inl CSIIOPS.inl CSIIOPS_T.inl: CSIIOP.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_Security_Export -Wb,export_include=Security/security_export.h $(srcdir)/CSIIOP.idl

BUILT_SOURCES += \
  GSSUPS_T.cpp \
  GSSUPC.cpp \
  GSSUPC.h \
  GSSUPS.h \
  GSSUPS_T.h \
  GSSUPC.inl \
  GSSUPS.inl \
  GSSUPS_T.inl

CLEANFILES += \
  GSSUPS_T.cpp \
  GSSUPC.cpp \
  GSSUPC.h \
  GSSUPS.h \
  GSSUPS_T.h \
  GSSUPC.inl \
  GSSUPS.inl \
  GSSUPS_T.inl

GSSUPS_T.cpp GSSUPC.cpp GSSUPC.h GSSUPS.h GSSUPS_T.h GSSUPC.inl GSSUPS.inl GSSUPS_T.inl: GSSUP.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_Security_Export -Wb,export_include=Security/security_export.h $(srcdir)/GSSUP.idl

BUILT_SOURCES += \
  SecurityS_T.cpp \
  SecurityC.cpp \
  SecurityS.cpp \
  SecurityC.h \
  SecurityS.h \
  SecurityS_T.h \
  SecurityC.inl \
  SecurityS.inl \
  SecurityS_T.inl

CLEANFILES += \
  SecurityS_T.cpp \
  SecurityC.cpp \
  SecurityS.cpp \
  SecurityC.h \
  SecurityS.h \
  SecurityS_T.h \
  SecurityC.inl \
  SecurityS.inl \
  SecurityS_T.inl

SecurityS_T.cpp SecurityC.cpp SecurityS.cpp SecurityC.h SecurityS.h SecurityS_T.h SecurityC.inl SecurityS.inl SecurityS_T.inl: Security.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_Security_Export -Wb,export_include=Security/security_export.h $(srcdir)/Security.idl

BUILT_SOURCES += \
  SecurityLevel1S_T.cpp \
  SecurityLevel1C.cpp \
  SecurityLevel1S.cpp \
  SecurityLevel1C.h \
  SecurityLevel1S.h \
  SecurityLevel1S_T.h \
  SecurityLevel1C.inl \
  SecurityLevel1S.inl \
  SecurityLevel1S_T.inl

CLEANFILES += \
  SecurityLevel1S_T.cpp \
  SecurityLevel1C.cpp \
  SecurityLevel1S.cpp \
  SecurityLevel1C.h \
  SecurityLevel1S.h \
  SecurityLevel1S_T.h \
  SecurityLevel1C.inl \
  SecurityLevel1S.inl \
  SecurityLevel1S_T.inl

SecurityLevel1S_T.cpp SecurityLevel1C.cpp SecurityLevel1S.cpp SecurityLevel1C.h SecurityLevel1S.h SecurityLevel1S_T.h SecurityLevel1C.inl SecurityLevel1S.inl SecurityLevel1S_T.inl: SecurityLevel1.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_Security_Export -Wb,export_include=Security/security_export.h $(srcdir)/SecurityLevel1.idl

BUILT_SOURCES += \
  SecurityLevel2S_T.cpp \
  SecurityLevel2C.cpp \
  SecurityLevel2S.cpp \
  SecurityLevel2C.h \
  SecurityLevel2S.h \
  SecurityLevel2S_T.h \
  SecurityLevel2C.inl \
  SecurityLevel2S.inl \
  SecurityLevel2S_T.inl

CLEANFILES += \
  SecurityLevel2S_T.cpp \
  SecurityLevel2C.cpp \
  SecurityLevel2S.cpp \
  SecurityLevel2C.h \
  SecurityLevel2S.h \
  SecurityLevel2S_T.h \
  SecurityLevel2C.inl \
  SecurityLevel2S.inl \
  SecurityLevel2S_T.inl

SecurityLevel2S_T.cpp SecurityLevel2C.cpp SecurityLevel2S.cpp SecurityLevel2C.h SecurityLevel2S.h SecurityLevel2S_T.h SecurityLevel2C.inl SecurityLevel2S.inl SecurityLevel2S_T.inl: SecurityLevel2.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_Security_Export -Wb,export_include=Security/security_export.h $(srcdir)/SecurityLevel2.idl

BUILT_SOURCES += \
  SecurityLevel3S_T.cpp \
  SecurityLevel3C.cpp \
  SecurityLevel3C.h \
  SecurityLevel3S.h \
  SecurityLevel3S_T.h \
  SecurityLevel3C.inl \
  SecurityLevel3S.inl \
  SecurityLevel3S_T.inl

CLEANFILES += \
  SecurityLevel3S_T.cpp \
  SecurityLevel3C.cpp \
  SecurityLevel3C.h \
  SecurityLevel3S.h \
  SecurityLevel3S_T.h \
  SecurityLevel3C.inl \
  SecurityLevel3S.inl \
  SecurityLevel3S_T.inl

SecurityLevel3S_T.cpp SecurityLevel3C.cpp SecurityLevel3C.h SecurityLevel3S.h SecurityLevel3S_T.h SecurityLevel3C.inl SecurityLevel3S.inl SecurityLevel3S_T.inl: SecurityLevel3.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -Wb,export_macro=TAO_Security_Export -Wb,export_include=Security/security_export.h $(srcdir)/SecurityLevel3.idl

Security = \
  CSIC.cpp \
  CSIIOPC.cpp \
  GSSUPC.cpp \
  SecurityC.cpp \
  SecurityS.cpp \
  SecurityLevel1C.cpp \
  SecurityLevel1S.cpp \
  SecurityLevel2C.cpp \
  SecurityLevel2S.cpp \
  SecurityLevel3C.cpp \
  Security/SL2_EstablishTrustPolicy.cpp \
  Security/SL2_QOPPolicy.cpp \
  Security/SL3_CredentialsAcquirerFactory.cpp \
  Security/SL3_CredentialsCurator.cpp \
  Security/Security_Current_Impl.cpp \
  Security/Security_Current.cpp \
  Security/Security_ORBInitializer.cpp \
  Security/Security_PolicyFactory.cpp \
  Security/CSI_Utils.cpp \
  Security/SL3_ContextEstablishmentPolicy.cpp \
  Security/SL3_ObjectCredentialsPolicy.cpp \
  Security/SL3_PolicyFactory.cpp \
  Security/SL3_SecurityManager.cpp \
  Security/SL3_SecurityCurrent.cpp \
  Security/SL3_SecurityCurrent_Impl.cpp

lib_LTLIBRARIES += libTAO_Security.la

libTAO_Security_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_SECURITY_BUILD_DLL

libTAO_Security_la_SOURCES = \
  $(Security)

libTAO_Security_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_Security_la_LIBADD = \
 $(top_builddir)/tao/libTAO_PortableServer.la \
 $(top_builddir)/tao/libTAO_IORInterceptor.la \
 $(top_builddir)/tao/libTAO_ObjRefTemplate.la \
 $(top_builddir)/tao/libTAO_Valuetype.la \
 $(top_builddir)/tao/libTAO.la \
 $(top_builddir)/../ace/libACE.la

noinst_HEADERS += \
  Security/SL3_ObjectCredentialsPolicy.h \
  Security/SL3_SecurityCurrent.h \
  Security/SL3_SecurityManager.h \
  SecurityC.h \
  Security/SL2_QOPPolicy.h \
  Security/SL3_CredentialsAcquirerFactory.h \
  SecurityS.h \
  Security/SL3_ContextEstablishmentPolicy.h \
  Security/SL2_EstablishTrustPolicy.h \
  Security/Security_ORBInitializer.h \
  Security/SL3_SecurityCurrent_Impl.h \
  Security/SL3_PolicyFactory.h \
  Security/Security_Current.h \
  Security/Security_PolicyFactory.h \
  CSIIOPC.h \
  CSIC.h \
  Security/Security_Current_Impl.h \
  GSSUPC.h \
  SecurityLevel1C.h \
  Security/SL3_CredentialsCurator.h \
  SecurityLevel1S.h \
  Security/CSI_Utils.h \
  SecurityLevel2C.h \
  SecurityLevel2S.h \
  SecurityLevel3C.h \
  SecurityLevel3S_T.h \
  SecurityLevel1S_T.h \
  GSSUPS_T.h \
  Channel_Clients_T.h \
  SecurityS_T.h \
  SecurityLevel2S_T.h \
  CSIS_T.h \
  CSIIOPS_T.h \
  Security/SL3_SecurityCurrent.inl \
  SecurityC.inl \
  SecurityS.inl \
  Security/Security_Current.inl \
  CSIIOPC.inl \
  CSIC.inl \
  GSSUPC.inl \
  SecurityLevel1C.inl \
  SecurityLevel1S.inl \
  SecurityLevel2C.inl \
  SecurityLevel2S.inl \
  SecurityLevel3C.inl \
  SecurityLevel3S_T.inl \
  SecurityLevel1S_T.inl \
  GSSUPS_T.inl \
  Channel_Clients_T.i \
  SecurityS_T.inl \
  SecurityLevel2S_T.inl \
  CSIS_T.inl \
  CSIIOPS_T.inl \
  CSIS_T.cpp \
  CSIIOPS_T.cpp \
  GSSUPS_T.cpp \
  SecurityS_T.cpp \
  SecurityLevel1S_T.cpp \
  SecurityLevel2S_T.cpp \
  SecurityLevel3S_T.cpp \
  Channel_Clients_T.cpp


## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -f gcctemp.c gcctemp so_locations
	-rm -rf ptrepository SunWS_cache Templates.DB
