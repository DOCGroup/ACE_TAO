##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile for repeating token client application
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

AM_CPPFLAGS = -I$(top_builddir) -I$(top_srcdir)

noinst_PROGRAMS = \
         manual

manual_SOURCES = manual.cpp
manual_LDADD = \
              $(top_builddir)/netsvcs/lib/libnetsvcs.la \
              $(top_builddir)/ace/libACE.la

## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations
