##---------------------------------------------------------------------------
##       $Id$
##
##       Makefile for TAO's Naming Service library
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

lib_LTLIBRARIES = libTAO_Naming.la

libTAO_Naming_la_LDADD = \
	$(top_builddir)/ace/libACE.la $(top_builddir)/TAO/tao/libTAO.la

libTAO_Naming_la_SOURCES = \
        Entries.cpp \
        Hash_Naming_Context.cpp \
        Naming_Context.cpp \
        Naming_Utils.cpp

noinst_HEADERS = \
        Entries.h \
        Hash_Naming_Context.h \
        Naming_Context.h \
        Naming_Utils.h

EXTRA_DIST = README

## Clean up some additional files/directories possibly created during
## the configure script tests.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations

