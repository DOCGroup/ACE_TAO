##  Process this file with automake to create Makefile.in

ACE_ROOT = $(top_srcdir)/..
ACE_BUILDDIR = $(top_builddir)/..
TAO_ROOT = $(top_srcdir)
TAO_BUILDDIR = $(top_builddir)

TAO_IDL = ACE_ROOT=$(ACE_ROOT) TAO_ROOT=$(TAO_ROOT) $(top_builddir)/TAO_IDL/tao_idl
TAO_IDLFLAGS = -Ge 1 -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -I$(TAO_ROOT) -I$(srcdir) -g $(ACE_BUILDDIR)/apps/gperf/src/gperf

## Makefile.FaultTolerance_IOGR_Manager.am 

if BUILD_ACE_UUID
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
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs $(srcdir)/test.idl
	@touch $@

noinst_PROGRAMS = Manager

Manager_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs

Manager_SOURCES = \
  Manager.cpp \
  testC.cpp \
  Manager.h \
  testC.h \
  testS_T.h \
  testC.inl \
  testS_T.inl

Manager_LDADD = \
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
endif BUILD_ACE_UUID

## Makefile.FaultTolerance_IOGR_Server.am 

if BUILD_ACE_UUID
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
  server.cpp \
  testC.cpp \
  testS.cpp \
  test_i.cpp \
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
endif BUILD_ACE_UUID

## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -f gcctemp.c gcctemp so_locations
	-rm -rf ptrepository SunWS_cache Templates.DB
