##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile.am for the netsvcs server example
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

AM_CPPFLAGS = -I$(top_builddir) -I$(top_srcdir) -I$(top_srcdir)/netsvcs/lib

noinst_PROGRAMS = main

main_SOURCES = main.cpp
main_LDADD = $(top_builddir)/netsvcs/lib/libnetsvcs.la \
             $(top_builddir)/ace/libACE.la

noinst_DATA = svc.conf

EXTRA_DIST = $(noinst_DATA)
