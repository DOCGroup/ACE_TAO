##---------------------------------------------------------------------------
##       $Id$
##
##       Makefile for TAO's AVStreams Tests -- server_discovery directory
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

noinst_LTLIBRARIES = libavdemo

libavdemo_LDADD = \
	$(top_builddir)/ace/libACE.la $(top_builddir)/TAO/tao/libTAO.la

libavdemo_shared_SOURCES = \
        Server_Discovery.cpp \
        Trader_Client.cpp \
        main.cpp

noinst_HEADERS = \
        Server_0005fDiscovery_0005fUtil.h \
        Trader_Client.h

IDL_SOURCES = \
        Video_Repository.idl

JAVA_SOURCES = \
        Queue.java \
        Server_Discovery.java \
        Server_Discovery_Browser.java \
        Server_Discovery_Perf.java \
        Server_Discovery_Selection.java \
        Server_Discovery_Util.java \
        Strip_Chart.java

noinst_DATA = \
        graph02.gif \
        graph09.gif \
        mmedia3.gif \
        server01.gif \
        server02.gif \
        www1.gif

EXTRA_DIST = AV_Demo README $(IDL_SOURCES) $(JAVA_SOURCES) $(noinst_DATA)

## Clean up some additional files/directories possibly created during
## the configure script tests.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations

