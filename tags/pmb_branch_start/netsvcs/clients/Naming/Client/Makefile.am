##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile for the client programs that test the ACE network services
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

noinst_LIBRARIES = libClient_Test.a

libClient_Test_a_SOURCES = Client_Test.cpp Client_Test.h


noinst_PROGRAMS = \
         main

main_SOURCES = main.cpp
main_LDADD = $(top_builddir)/netsvcs/clients/Naming/Client/libClient_Test.a \
             $(top_builddir)/ace/libACE.la

noinst_DATA = svc.conf svc2.conf

EXTRA_DIST = $(noinst_DATA)

## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations
