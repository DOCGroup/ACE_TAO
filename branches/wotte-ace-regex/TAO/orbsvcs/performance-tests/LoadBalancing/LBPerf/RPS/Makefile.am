##  Process this file with automake to create Makefile.in

ACE_ROOT = $(top_srcdir)/..
ACE_BUILDDIR = $(top_builddir)/..
TAO_ROOT = $(top_srcdir)
TAO_BUILDDIR = $(top_builddir)

TAO_IDL = ACE_ROOT=$(ACE_ROOT) TAO_ROOT=$(TAO_ROOT) $(top_builddir)/TAO_IDL/tao_idl
TAO_IDLFLAGS = -Ge 1 -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -I$(TAO_ROOT) -I$(srcdir) -g $(ACE_BUILDDIR)/apps/gperf/src/gperf

if BUILD_AMI
if !BUILD_MINIMUM_CORBA

BUILT_SOURCES = \
  TestS_T.cpp \
  TestC.inl \
  TestS.inl \
  TestS_T.inl \
  TestC.cpp \
  TestC.h \
  TestS.h \
  TestS_T.h

CLEANFILES = \
  TestS_T.cpp \
  TestC.inl \
  TestS.inl \
  TestS_T.inl \
  TestC.cpp \
  TestC.h \
  TestS.h \
  TestS_T.h \
  Test-stamp

TestS_T.cpp TestC.inl TestS.inl TestS_T.inl TestC.cpp TestC.h TestS.h TestS_T.h: Test-stamp

Test-stamp: $(srcdir)/Test.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) $(srcdir)/Test.idl
	@touch $@

endif
endif

## Makefile.LoadBalancing_Performance_Client.am 

if BUILD_AMI
if !BUILD_MINIMUM_CORBA

noinst_PROGRAMS = client

client_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs

client_SOURCES = \
  TestC.cpp \
  client.cpp \
  TestC.h \
  TestS_T.h \
  TestC.inl \
  TestS_T.inl

client_LDADD = \
  $(TAO_BUILDDIR)/tao/libTAO_Strategies.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosLoadBalancing.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_PortableGroup.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORManip.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_Svc_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORTable.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORInterceptor.la \
  $(TAO_BUILDDIR)/tao/libTAO_ObjRefTemplate.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

endif !BUILD_MINIMUM_CORBA
endif BUILD_AMI

## Makefile.LoadBalancing_Performance_Server.am 

if BUILD_AMI
if !BUILD_MINIMUM_CORBA

noinst_PROGRAMS += server

server_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs

server_SOURCES = \
  ORBInitializer.cpp \
  RPS_Monitor.cpp \
  Roundtrip.cpp \
  ServerRequestInterceptor.cpp \
  TestC.cpp \
  TestS.cpp \
  server.cpp \
  ORBInitializer.h \
  RPS_Monitor.h \
  Roundtrip.h \
  ServerRequestInterceptor.h \
  TestC.h \
  TestS.h \
  TestS_T.h \
  TestC.inl \
  TestS.inl \
  TestS_T.inl

server_LDADD = \
  $(TAO_BUILDDIR)/tao/libTAO_Strategies.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosLoadBalancing.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_PortableGroup.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORManip.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_Svc_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORTable.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORInterceptor.la \
  $(TAO_BUILDDIR)/tao/libTAO_ObjRefTemplate.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

endif !BUILD_MINIMUM_CORBA
endif BUILD_AMI

## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -f gcctemp.c gcctemp so_locations
	-rm -rf ptrepository SunWS_cache Templates.DB
