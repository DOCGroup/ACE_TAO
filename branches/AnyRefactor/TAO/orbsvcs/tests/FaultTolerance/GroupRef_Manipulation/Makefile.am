##  Process this file with automake to create Makefile.in

ACE_ROOT = $(top_srcdir)/..
ACE_BUILDDIR = $(top_builddir)/..
TAO_ROOT = $(top_srcdir)
TAO_BUILDDIR = $(top_builddir)

TAO_IDL = ACE_ROOT=$(ACE_ROOT) TAO_ROOT=$(TAO_ROOT) $(top_builddir)/TAO_IDL/tao_idl
TAO_IDLFLAGS = -Ge 1 -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -I$(TAO_ROOT) -I$(srcdir) -g $(ACE_BUILDDIR)/apps/gperf/src/gperf

## Makefile.GroupRef_Manipulation_Client.am 

if BUILD_ACE_UUID
if BUILD_INTERCEPTORS
if !BUILD_MINIMUM_CORBA

BUILT_SOURCES = \
  testS_T.cpp \
  testC.inl \
  testS.inl \
  testS_T.inl \
  testC.cpp \
  testC.h \
  testS.h \
  testS_T.h

CLEANFILES = \
  testS_T.cpp \
  testC.inl \
  testS.inl \
  testS_T.inl \
  testC.cpp \
  testC.h \
  testS.h \
  testS_T.h \
  test-stamp

testS_T.cpp testC.inl testS.inl testS_T.inl testC.cpp testC.h testS.h testS_T.h: test-stamp

test-stamp: $(srcdir)/test.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -DCORBA3 -I$(TAO_ROOT) $(srcdir)/test.idl
	@touch $@

noinst_PROGRAMS = client

client_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs

client_SOURCES = \
  client.cpp \
  testC.cpp \
  testC.h \
  testS_T.h \
  testC.inl \
  testS_T.inl

client_LDADD = \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_FT_ServerORB.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_FT_ClientORB.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_FTORB_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORInterceptor.la \
  $(TAO_BUILDDIR)/tao/libTAO_ObjRefTemplate.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORManip.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

endif !BUILD_MINIMUM_CORBA
endif BUILD_INTERCEPTORS
endif BUILD_ACE_UUID

## Makefile.GroupRef_Manipulation_Server.am 

if BUILD_ACE_UUID
if BUILD_INTERCEPTORS
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
  Server_ORBInitializer.cpp \
  Server_Request_Interceptor.cpp \
  server.cpp \
  testC.cpp \
  testS.cpp \
  test_i.cpp \
  Server_ORBInitializer.h \
  Server_Request_Interceptor.h \
  testC.h \
  testS.h \
  testS_T.h \
  test_i.h \
  testC.inl \
  testS.inl \
  testS_T.inl

server_LDADD = \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_FT_ServerORB.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_FT_ClientORB.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_FTORB_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORManip.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORInterceptor.la \
  $(TAO_BUILDDIR)/tao/libTAO_ObjRefTemplate.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

endif !BUILD_MINIMUM_CORBA
endif BUILD_INTERCEPTORS
endif BUILD_ACE_UUID

## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -f gcctemp.c gcctemp so_locations
	-rm -rf ptrepository SunWS_cache Templates.DB
