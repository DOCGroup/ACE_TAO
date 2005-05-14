##  Process this file with automake to create Makefile.in

ACE_ROOT = $(top_srcdir)/..
ACE_BUILDDIR = $(top_builddir)/..
TAO_ROOT = $(top_srcdir)
TAO_BUILDDIR = $(top_builddir)

## Makefile.ECT_Consumer.am

noinst_PROGRAMS = ECT_Consumer

ECT_Consumer_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs

ECT_Consumer_SOURCES = \
  ECT_Consumer.cpp \
  ECT_Consumer_Driver.cpp \
  ECT_Driver.cpp \
  ECT_Consumer.h \
  ECT_Consumer_Driver.h \
  ECT_Driver.h

ECT_Consumer_LDADD = \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_RTOLDEvent.la \
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

## Makefile.ECT_Supplier.am

noinst_PROGRAMS += ECT_Supplier

ECT_Supplier_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs

ECT_Supplier_SOURCES = \
  ECT_Driver.cpp \
  ECT_Supplier.cpp \
  ECT_Supplier_Driver.cpp \
  ECT_Driver.h \
  ECT_Supplier.h \
  ECT_Supplier_Driver.h \
  ECT_Driver.i

ECT_Supplier_LDADD = \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_RTOLDEvent.la \
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

## Makefile.ECT_Throughput.am

noinst_PROGRAMS += ECT_Throughput

ECT_Throughput_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs

ECT_Throughput_SOURCES = \
  ECT_Consumer.cpp \
  ECT_Driver.cpp \
  ECT_Supplier.cpp \
  ECT_Throughput.cpp \
  ECT_Consumer.h \
  ECT_Driver.h \
  ECT_Supplier.h \
  ECT_Throughput.h \
  ECT_Driver.i

ECT_Throughput_LDADD = \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_RTOLDEvent.la \
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
