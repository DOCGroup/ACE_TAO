##---------------------------------------------------------------------------
##       $Id$
##
##       Makefile for TAO's AVStreams Service -- benchmark directory
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

noinst_PROGRAMS = server client #child

server_LDADD = \
	$(top_builddir)/ace/libACE.la $(top_builddir)/TAO/tao/libTAO.la

server_SOURCES = \
        server.cpp \
        child.cpp

client_LDADD = \
	$(top_builddir)/ace/libACE.la $(top_builddir)/TAO/tao/libTAO.la

client_SOURCES = \
        client.cpp

## chile_LDADD = \
## 	$(top_builddir)/ace/libACE.la $(top_builddir)/TAO/tao/libTAO.la
##
## child_SOURCES = \
##         child.cpp

noinst_HEADERS = \
        child.h \
        client.h \
        server.h

noinst_DATA = svc.conf clnt.conf

EXTRA_DIST = $(noinst_DATA)
