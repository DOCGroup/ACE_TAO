##---------------------------------------------------------------------------
##       $Id$
##
##       Makefile for TAO's Implementation Repository Service
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

bin_PROGRAMS = ImplRepo_Service

ImplRepo_Service_LDADD = \
	$(top_builddir)/ace/libACE.la $(top_builddir)/TAO/tao/libTAO.la

ImplRepo_Service_SOURCES = \
        ImplRepo.cpp \
        ImplRepo_i.cpp \
        Repository.cpp

noinst_HEADERS = \
        ImplRepo_i.h \
        Repository.h

## EXTRA_DIST = README
