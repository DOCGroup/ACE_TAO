##---------------------------------------------------------------------------
##       $Id$
##
##       Makefile for TAO's Dump Schedule Service
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

bin_PROGRAMS = Dump_Schedule

Dump_Schedule_LDADD = \
	$(top_builddir)/ace/libACE.la $(top_builddir)/TAO/tao/libTAO.la

Dump_Schedule_SOURCES = \
        Dump_Schedul.cpp

## noinst_HEADERS = 

## EXTRA_DIST = README
