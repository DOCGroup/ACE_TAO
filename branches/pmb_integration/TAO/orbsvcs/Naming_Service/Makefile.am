##  Process this file with automake to create Makefile.in

ACE_ROOT = $(top_srcdir)/..
ACE_BUILDDIR = $(top_builddir)/..
TAO_ROOT = $(top_srcdir)
TAO_BUILDDIR = $(top_builddir)

## Makefile.NT_Naming_Service.am 

bin_PROGRAMS = NT_Naming_Service

NT_Naming_Service_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs

NT_Naming_Service_SOURCES = \
  NT_Naming_Server.cpp \
  NT_Naming_Service.cpp \
  Naming_Service.cpp \
  NT_Naming_Service.h \
  Naming_Service.h

NT_Naming_Service_LDADD = \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming_Serv.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_Svc_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORTable.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming_Skel.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

## Makefile.Naming_Service.am 

bin_PROGRAMS += Naming_Service

Naming_Service_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs

Naming_Service_SOURCES = \
  Naming_Server.cpp \
  Naming_Service.cpp \
  Naming_Service.h

Naming_Service_LDADD = \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming_Serv.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_Svc_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORTable.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming_Skel.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -f gcctemp.c gcctemp so_locations
	-rm -rf ptrepository SunWS_cache Templates.DB
