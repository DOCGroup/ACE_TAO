##---------------------------------------------------------------------------
##       $Id$
##
##       Makefile for TAO's Property Service library
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

lib_LTLIBRARIES = libTAO_Property.la

libTAO_Property_la_LDADD = \
	$(top_builddir)/ace/libACE.la $(top_builddir)/TAO/tao/libTAO.la

libTAO_Property_la_SOURCES = \
        CosPropertyService_i.cpp

noinst_HEADERS = \
        CosPropertyService_i.h
