##---------------------------------------------------------------------------
##       $Id$
##
##       Makefile for TAO's Audio/Visual (AV) Service library
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

lib_LTLIBRARIES = libTAO_AV.la

libTAO_AV_la_LDADD = \
	$(top_builddir)/ace/libACE.la $(top_builddir)/TAO/tao/libTAO.la

libTAO_AV_la_SOURCES = \
        AVStreams_i.cpp \
        Endpoint_Strategy.cpp \
        Endpoint_Strategy_T.cpp \
        sfp.cpp

noinst_HEADERS = \
        AVStreams_i.h \
        Endpoint_Strategy.h \
        Endpoint_Strategy_T.h \
        sfp.h

EXTRA_DIST = README

## Clean up some additional files/directories possibly created during
## the configure script tests.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations

