##  Process this file with automake to create Makefile.in
##
## $Id$
##

ACE_BUILDDIR = $(top_builddir)/..
ACE_ROOT = $(top_srcdir)/..
TAO_BUILDDIR = $(top_builddir)
TAO_ROOT = $(top_srcdir)

bin_PROGRAMS =

## Makefile.Time_Service_Clerk.am

if BUILD_CORBA_MESSAGING

bin_PROGRAMS += Time_Service_Clerk

Time_Service_Clerk_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs

Time_Service_Clerk_SOURCES = \
  Clerk_i.cpp \
  Time_Service_Clerk.cpp \
  Clerk_i.h

Time_Service_Clerk_LDADD = \
  $(TAO_BUILDDIR)/tao/libTAO_Utils.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming_Serv.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_Svc_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORTable.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming_Skel.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosTime.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

endif BUILD_CORBA_MESSAGING

## Makefile.Time_Service_Server.am

if BUILD_CORBA_MESSAGING

bin_PROGRAMS += Time_Service_Server

Time_Service_Server_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs

Time_Service_Server_SOURCES = \
  Server_i.cpp \
  Time_Service_Server.cpp \
  Server_i.h

Time_Service_Server_LDADD = \
  $(TAO_BUILDDIR)/tao/libTAO_Utils.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming_Serv.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_Svc_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_Messaging.la \
  $(TAO_BUILDDIR)/tao/libTAO_Valuetype.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORTable.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming_Skel.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosTime.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

endif BUILD_CORBA_MESSAGING

## Clean up template repositories, etc.
clean-local:
	-rm -f *~ *.bak *.rpo *.sym lib*.*_pure_* core core.*
	-rm -f gcctemp.c gcctemp so_locations *.ics
	-rm -rf cxx_repository ptrepository ti_files
	-rm -rf templateregistry ir.out
	-rm -rf ptrepository SunWS_cache Templates.DB
