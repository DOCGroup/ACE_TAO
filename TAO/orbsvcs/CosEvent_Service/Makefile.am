##---------------------------------------------------------------------------
##       $Id$
##
##       Makefile for TAO's CosEvent Service
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

bin_PROGRAMS = CosEvent_Service

CosEvent_Service_LDADD = \
	$(top_builddir)/ace/libACE.la $(top_builddir)/TAO/tao/libTAO.la

CosEvent_Service_SOURCES = \
        CosEvent_Service.cpp

noinst_HEADERS = \
        CosEvent_Service.h

EXTRA_DIST = README
