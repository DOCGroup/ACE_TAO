##---------------------------------------------------------------------------
##       $Id$
##
##       Makefile for TAO's AVStreams Tests -- mpeg/source/server directory
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

noinst_PROGRAMS = augmented_server server vs as

augmented_server_LDADD = \
	$(top_builddir)/ace/libACE.la $(top_builddir)/TAO/tao/libTAO.la

augmented_server_SOURCES = \
        Video_RepositoryC.cpp \
        augmented_server.cpp \
        Property_Exporter.cpp \
        Machine_Properties.cpp \
        Video_Repository.cpp

server_LDADD = \
	$(top_builddir)/ace/libACE.la $(top_builddir)/TAO/tao/libTAO.la

server_SOURCES = \
        server.cpp

vs_LDADD = \
	$(top_builddir)/ace/libACE.la $(top_builddir)/TAO/tao/libTAO.la

vs_SOURCES = \
        vs.cpp

as_LDADD = \
	$(top_builddir)/ace/libACE.la $(top_builddir)/TAO/tao/libTAO.la

as_SOURCES = \
        as.cpp

noinst_HEADERS = \
        Machine_Properties.h \
        Property_Exporter.h \
        Video_Repository.h \
        as.h \
        augmented_server.h \
        server.h \
        vs.h

IDL_SOURCES = \
        Video_Repository.idl

BUILT_SOURCES = \
        Video_RepositoryS.cpp
        Video_RepositoryS.h
        Video_RepositoryC.cpp
        Video_RepositoryC.h

EXTRA_DIST = $(IDL_SOURCES)

## Clean up some additional files/directories possibly created during
## the configure script tests.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations

