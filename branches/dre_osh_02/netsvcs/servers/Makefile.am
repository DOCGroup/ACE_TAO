##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile.am for the netsvcs server example
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir) -I$(top_srcdir)/netsvcs/lib

noinst_PROGRAMS = main

main_SOURCES = main.cpp
main_LDADD = $(top_builddir)/netsvcs/lib/libnetsvcs.la \
             $(top_builddir)/ace/libACE.la

noinst_DATA = svc.conf

EXTRA_DIST = $(noinst_DATA)

