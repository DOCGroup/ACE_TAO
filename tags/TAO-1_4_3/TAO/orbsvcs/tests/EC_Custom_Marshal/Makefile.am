##  Process this file with automake to create Makefile.in

ACE_ROOT = $(top_srcdir)/..
ACE_BUILDDIR = $(top_builddir)/..
TAO_ROOT = $(top_srcdir)
TAO_BUILDDIR = $(top_builddir)

TAO_IDL = ACE_ROOT=$(ACE_ROOT) TAO_ROOT=$(TAO_ROOT) $(top_builddir)/TAO_IDL/tao_idl
TAO_IDLFLAGS = -Ge 1 -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -I$(TAO_ROOT) -I$(srcdir) -g $(ACE_BUILDDIR)/apps/gperf/src/gperf

BUILT_SOURCES = \
  dataS_T.cpp \
  dataC.inl \
  dataS.inl \
  dataS_T.inl \
  dataC.cpp \
  dataS.cpp \
  dataC.h \
  dataS.h \
  dataS_T.h

CLEANFILES = \
  dataS_T.cpp \
  dataC.inl \
  dataS.inl \
  dataS_T.inl \
  dataC.cpp \
  dataS.cpp \
  dataC.h \
  dataS.h \
  dataS_T.h \
  data-stamp

dataS_T.cpp dataC.inl dataS.inl dataS_T.inl dataC.cpp dataS.cpp dataC.h dataS.h dataS_T.h: data-stamp

data-stamp: $(srcdir)/data.idl
	$(TAO_IDL) $(TAO_IDLFLAGS) -I$(TAO_ROOT)/orbsvcs $(srcdir)/data.idl
	@touch $@

## Makefile.ECM_Consumer.am 

noinst_PROGRAMS = ECM_Consumer

ECM_Consumer_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs

ECM_Consumer_SOURCES = \
  ECM_Consumer.cpp \
  ECM_Data.cpp \
  dataC.cpp \
  dataS.cpp \
  ECM_Consumer.h \
  ECM_Data.h \
  dataC.h \
  dataS.h \
  dataS_T.h \
  dataC.inl \
  dataS.inl \
  dataS_T.inl

ECM_Consumer_LDADD = \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_RTSched.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORTable.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_RTEvent.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_Svc_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORInterceptor.la \
  $(TAO_BUILDDIR)/tao/libTAO_ObjRefTemplate.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

## Makefile.ECM_Supplier.am 

noinst_PROGRAMS += ECM_Supplier

ECM_Supplier_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs

ECM_Supplier_SOURCES = \
  ECM_Data.cpp \
  ECM_Supplier.cpp \
  dataC.cpp \
  dataS.cpp \
  ECM_Data.h \
  ECM_Supplier.h \
  dataC.h \
  dataS.h \
  dataS_T.h \
  dataC.inl \
  dataS.inl \
  dataS_T.inl

ECM_Supplier_LDADD = \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_RTSched.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORTable.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_RTEvent.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_Svc_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
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
