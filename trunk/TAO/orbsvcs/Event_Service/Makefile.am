##---------------------------------------------------------------------------
##       $Id$
##
##       Makefile for TAO's Event Service
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

bin_PROGRAMS = Event_Service

Event_Service_LDADD = \
	$(top_builddir)/ace/libACE.la $(top_builddir)/TAO/tao/libTAO.la

Event_Service_SOURCES = \
        Event_Service.cpp

noinst_HEADERS = \
        Event_Service.h

## EXTRA_DIST = README
