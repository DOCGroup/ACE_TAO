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

## Makefile.BiDirCORBALOC_Server.am

BUILT_SOURCES = \
  TimeModuleC.inl \
  TimeModuleS.inl \
  TimeModuleS_T.inl \
  TimeModuleC.h \
  TimeModuleS.h \
  TimeModuleS_T.h \
  TimeModuleC.cpp \
  TimeModuleS.cpp \
  TimeModuleA.cpp \
  TimeModuleS_T.cpp

CLEANFILES = \
  TimeModule-stamp \
  TimeModuleC.inl \
  TimeModuleS.inl \
  TimeModuleS_T.inl \
  TimeModuleC.h \
  TimeModuleS.h \
  TimeModuleS_T.h \
  TimeModuleC.cpp \
  TimeModuleS.cpp \
  TimeModuleA.cpp \
  TimeModuleS_T.cpp

TimeModuleC.inl TimeModuleS.inl TimeModuleS_T.inl TimeModuleC.h TimeModuleS.h TimeModuleS_T.h TimeModuleC.cpp TimeModuleS.cpp TimeModuleA.cpp TimeModuleS_T.cpp: TimeModule-stamp

TimeModule-stamp: $(srcdir)/TimeModule.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs -GA $(srcdir)/TimeModule.idl
	@touch $@

noinst_PROGRAMS = TimeServer

TimeServer_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs

TimeServer_SOURCES = \
  TimeModuleA.cpp \
  TimeModuleC.cpp \
  TimeModuleS.cpp \
  TimeServer.cpp \
  TimeModuleC.h \
  TimeModuleS.h \
  TimeModuleS_T.h \
  TimeModuleC.inl \
  TimeModuleS.inl \
  TimeModuleS_T.inl

TimeServer_LDADD = \
  $(TAO_BUILDDIR)/tao/libTAO_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_BiDirGIOP.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

## Clean up template repositories, etc.
clean-local:
	-rm -f *~ *.bak *.rpo *.sym lib*.*_pure_* core core.*
	-rm -f gcctemp.c gcctemp so_locations *.ics
	-rm -rf cxx_repository ptrepository ti_files
	-rm -rf templateregistry ir.out
	-rm -rf ptrepository SunWS_cache Templates.DB
