##  Process this file with automake to create Makefile.in

ACE_ROOT = $(top_srcdir)/..
ACE_BUILDDIR = $(top_builddir)/..
TAO_ROOT = $(top_srcdir)
TAO_BUILDDIR = $(top_builddir)

## Makefile.LifeCycle_Service.am 

if !BUILD_MINIMUM_CORBA

bin_PROGRAMS = LifeCycle_Service

LifeCycle_Service_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs

LifeCycle_Service_SOURCES = \
  Criteria_Evaluator.cpp \
  Factory_Trader.cpp \
  LifeCycle_Service.cpp \
  LifeCycle_Service_i.cpp \
  Criteria_Evaluator.h \
  Factory_Trader.h \
  LifeCycle_Service.h \
  LifeCycle_Service_i.h

LifeCycle_Service_LDADD = \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosTrading_Serv.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_Svc_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_DynamicAny.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosTrading_Skel.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosTrading.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosLifeCycle.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

endif !BUILD_MINIMUM_CORBA

## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -f gcctemp.c gcctemp so_locations
	-rm -rf ptrepository SunWS_cache Templates.DB
