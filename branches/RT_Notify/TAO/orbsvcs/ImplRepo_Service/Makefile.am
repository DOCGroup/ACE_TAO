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
        Adapter_Activator.cpp \
        Forwarder.cpp \
        ImplRepo.cpp \
        ImplRepo_i.cpp \
        Iterator.cpp \
        Locator.cpp \
        NT_Service.h \
        Options.cpp \
        Repository.cpp

noinst_HEADERS = \
        Adapter_Activator.h \
        Forwarder.h \
        ImplRepo_i.h \
        Iterator.h \
        NT_Service.h \
        Options.h \
        Repository.h

## EXTRA_DIST = README

## Clean up some additional files/directories possibly created during
## the configure script tests.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations

