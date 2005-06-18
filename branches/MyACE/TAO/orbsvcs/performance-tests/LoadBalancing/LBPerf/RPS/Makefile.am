##  Process this file with automake to create Makefile.in
##
## $Id$
##

ACE_BUILDDIR = $(top_builddir)/..
ACE_ROOT = $(top_srcdir)/..
TAO_BUILDDIR = $(top_builddir)
TAO_ROOT = $(top_srcdir)

TAO_IDL = ACE_ROOT=$(ACE_ROOT) TAO_ROOT=$(TAO_ROOT) $(TAO_BUILDDIR)/TAO_IDL/tao_idl
TAO_IDLFLAGS = -Ge 1 -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -I$(TAO_ROOT) -I$(srcdir) -g $(ACE_BUILDDIR)/apps/gperf/src/gperf

noinst_PROGRAMS =
CLEANFILES =
BUILT_SOURCES =

## Makefile.LoadBalancing_Performance_IDL.am

if BUILD_AMI
if BUILD_CORBA_MESSAGING
if !BUILD_MINIMUM_CORBA

BUILT_SOURCES += \
  TestC.inl \
  TestS.inl \
  TestS_T.inl \
  TestC.h \
  TestS.h \
  TestS_T.h \
  TestC.cpp \
  TestS.cpp \
  TestS_T.cpp

CLEANFILES += \
  Test-stamp \
  TestC.inl \
  TestS.inl \
  TestS_T.inl \
  TestC.h \
  TestS.h \
  TestS_T.h \
  TestC.cpp \
  TestS.cpp \
  TestS_T.cpp

TestC.inl TestS.inl TestS_T.inl TestC.h TestS.h TestS_T.h TestC.cpp TestS.cpp TestS_T.cpp: Test-stamp

Test-stamp: $(srcdir)/Test.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs $(srcdir)/Test.idl
	@touch $@


endif !BUILD_MINIMUM_CORBA
endif BUILD_CORBA_MESSAGING
endif BUILD_AMI

## Makefile.LoadBalancing_Performance_Client.am

if BUILD_AMI
if BUILD_CORBA_MESSAGING
if !BUILD_MINIMUM_CORBA

noinst_PROGRAMS += client

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
  ORBInitializer.h \
  RPS_Monitor.h \
  Roundtrip.h \
  ServerRequestInterceptor.h

client_LDADD = \
  $(TAO_BUILDDIR)/tao/libTAO_Strategies.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosLoadBalancing.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORInterceptor.la \
  $(TAO_BUILDDIR)/tao/libTAO_ObjRefTemplate.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_PortableGroup.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORManip.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

endif !BUILD_MINIMUM_CORBA
endif BUILD_CORBA_MESSAGING
endif BUILD_AMI

## Makefile.LoadBalancing_Performance_Server.am

if BUILD_AMI
if BUILD_CORBA_MESSAGING
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
  ServerRequestInterceptor.h

server_LDADD = \
  $(TAO_BUILDDIR)/tao/libTAO_Strategies.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosLoadBalancing.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORInterceptor.la \
  $(TAO_BUILDDIR)/tao/libTAO_ObjRefTemplate.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_PortableGroup.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORManip.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

endif !BUILD_MINIMUM_CORBA
endif BUILD_CORBA_MESSAGING
endif BUILD_AMI

## Clean up template repositories, etc.
clean-local:
	-rm -f *~ *.bak *.rpo *.sym lib*.*_pure_* core core.*
	-rm -f gcctemp.c gcctemp so_locations *.ics
	-rm -rf cxx_repository ptrepository ti_files
	-rm -rf templateregistry ir.out
	-rm -rf ptrepository SunWS_cache Templates.DB
