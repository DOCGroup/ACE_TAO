##  Process this file with automake to create Makefile.in
##
## $Id$
##

ACE_BUILDDIR = $(top_builddir)/..
ACE_ROOT = $(top_srcdir)/..
TAO_BUILDDIR = $(top_builddir)
TAO_ROOT = $(top_srcdir)

## Makefile.AVS_Simple_Three_Stage_Dist.am

noinst_PROGRAMS = distributer

distributer_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs

distributer_SOURCES = \
  distributer.cpp \
  distributer.h

distributer_LDADD = \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_AV.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosProperty.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

## Makefile.AVS_Simple_Three_Stage_Recv.am

noinst_PROGRAMS += receiver

receiver_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs

receiver_SOURCES = \
  receiver.cpp \
  receiver.h

receiver_LDADD = \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_AV.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosProperty.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

## Makefile.AVS_Simple_Three_Stage_Send.am

noinst_PROGRAMS += sender

sender_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs

sender_SOURCES = \
  sender.cpp \
  sender.h

sender_LDADD = \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_AV.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosProperty.la \
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
