##  Process this file with automake to create Makefile.in

ACE_ROOT = $(top_srcdir)/..
ACE_BUILDDIR = $(top_builddir)/..
TAO_ROOT = $(top_srcdir)
TAO_BUILDDIR = $(top_builddir)

## Makefile.Basic_Logging_Service.am 

noinst_PROGRAMS = Basic_Logging_Service

Basic_Logging_Service_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs

Basic_Logging_Service_SOURCES = \
  Basic_Logging_Service.cpp \
  Basic_Logging_Service.h

Basic_Logging_Service_LDADD = \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_DsLogAdmin_Serv.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_ETCL.la \
  $(TAO_BUILDDIR)/tao/libTAO_DynamicAny.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_DsLogAdmin_Skel.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_DsLogAdmin.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -f gcctemp.c gcctemp so_locations
	-rm -rf ptrepository SunWS_cache Templates.DB
