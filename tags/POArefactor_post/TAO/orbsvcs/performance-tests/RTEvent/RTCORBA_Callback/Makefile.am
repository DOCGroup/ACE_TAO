##  Process this file with automake to create Makefile.in

ACE_ROOT = $(top_srcdir)/..
ACE_BUILDDIR = $(top_builddir)/..
TAO_ROOT = $(top_srcdir)
TAO_BUILDDIR = $(top_builddir)

TAO_IDL = ACE_ROOT=$(ACE_ROOT) TAO_ROOT=$(TAO_ROOT) $(top_builddir)/TAO_IDL/tao_idl
TAO_IDLFLAGS = -Ge 1 -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -I$(TAO_ROOT) -I$(srcdir) -g $(ACE_BUILDDIR)/apps/gperf/src/gperf

BUILT_SOURCES = \
  TestS_T.cpp \
  TestC.inl \
  TestS.inl \
  TestS_T.inl \
  TestC.cpp \
  TestS.cpp \
  TestC.h \
  TestS.h \
  TestS_T.h

CLEANFILES = \
  TestS_T.cpp \
  TestC.inl \
  TestS.inl \
  TestS_T.inl \
  TestC.cpp \
  TestS.cpp \
  TestC.h \
  TestS.h \
  TestS_T.h \
  Test-stamp

TestS_T.cpp TestC.inl TestS.inl TestS_T.inl TestC.cpp TestS.cpp TestC.h TestS.h TestS_T.h: Test-stamp

Test-stamp: $(srcdir)/Test.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) $(srcdir)/Test.idl
	@touch $@

## Makefile.RTCORBA_Callback_Client.am 

if BUILD_RT_CORBA
if !BUILD_MINIMUM_CORBA
noinst_PROGRAMS = client
endif
endif

client_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(srcdir)/../lib \
  -I../lib \
  -I$(TAO_ROOT)/orbsvcs

client_SOURCES = \
  Callback.cpp \
  TestC.cpp \
  TestS.cpp \
  client.cpp \
  Callback.h \
  TestC.h \
  TestS.h \
  TestS_T.h \
  TestC.inl \
  TestS.inl \
  TestS_T.inl

client_LDADD = \
  ../lib/libTAO_RTEC_Perf.la \
  $(TAO_BUILDDIR)/tao/libTAO_Strategies.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_RTCORBAEvent.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_RTEvent.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_Svc_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_RTPortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_RTCORBA.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORInterceptor.la \
  $(TAO_BUILDDIR)/tao/libTAO_ObjRefTemplate.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la


## Makefile.RTCORBA_Callback_Server.am 

if BUILD_RT_CORBA
if !BUILD_MINIMUM_CORBA
noinst_PROGRAMS += server
endif
endif

server_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(srcdir)/../lib \
  -I../lib \
  -I$(TAO_ROOT)/orbsvcs

server_SOURCES = \
  Session.cpp \
  Session_Factory.cpp \
  TestC.cpp \
  TestS.cpp \
  server.cpp \
  Session.h \
  Session_Factory.h \
  TestC.h \
  TestS.h \
  TestS_T.h \
  TestC.inl \
  TestS.inl \
  TestS_T.inl

server_LDADD = \
  ../lib/libTAO_RTEC_Perf.la \
  $(TAO_BUILDDIR)/tao/libTAO_Strategies.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_RTCORBAEvent.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_RTEvent.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_Svc_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_RTPortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_RTCORBA.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORInterceptor.la \
  $(TAO_BUILDDIR)/tao/libTAO_ObjRefTemplate.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la


## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -f gcctemp.c gcctemp so_locations
	-rm -rf ptrepository SunWS_cache Templates.DB
