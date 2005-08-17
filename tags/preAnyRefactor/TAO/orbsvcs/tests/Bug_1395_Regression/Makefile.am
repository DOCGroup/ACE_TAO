##  Process this file with automake to create Makefile.in

ACE_ROOT = $(top_srcdir)/..
ACE_BUILDDIR = $(top_builddir)/..
TAO_ROOT = $(top_srcdir)
TAO_BUILDDIR = $(top_builddir)

TAO_IDL = ACE_ROOT=$(ACE_ROOT) TAO_ROOT=$(TAO_ROOT) $(top_builddir)/TAO_IDL/tao_idl
TAO_IDLFLAGS = -Ge 1 -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -I$(TAO_ROOT) -I$(srcdir) -g $(ACE_BUILDDIR)/apps/gperf/src/gperf

BUILT_SOURCES = \
  TestS_T.cpp \
  TestC.cpp \
  TestC.h \
  TestS.h \
  TestS_T.h \
  TestC.inl \
  TestS.inl \
  TestS_T.inl

CLEANFILES = \
  TestS_T.cpp \
  TestC.cpp \
  TestC.h \
  TestS.h \
  TestS_T.h \
  TestC.inl \
  TestS.inl \
  TestS_T.inl \
  Test-stamp

TestS_T.cpp TestC.cpp TestC.h TestS.h TestS_T.h TestC.inl TestS.inl TestS_T.inl: Test-stamp

Test-stamp: $(srcdir)/Test.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) $(srcdir)/Test.idl
	@touch $@

## Makefile.Bug_1395_Regression_Client.am

noinst_PROGRAMS = client

client_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR)

client_SOURCES = \
  TestC.cpp \
  client.cpp \
  TestC.h \
  TestS_T.h \
  TestC.inl \
  TestS_T.inl

client_LDADD = \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la


## Makefile.Bug_1395_Regression_Server.am

noinst_PROGRAMS += server

server_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR)

server_SOURCES = \
  TestC.cpp \
  TestS.cpp \
  Test_i.cpp \
  server.cpp \
  TestC.h \
  TestS.h \
  TestS_T.h \
  Test_i.h \
  TestC.inl \
  TestS.inl \
  TestS_T.inl

server_LDADD = \
  $(TAO_BUILDDIR)/tao/libTAO_ImR_Client.la \
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
