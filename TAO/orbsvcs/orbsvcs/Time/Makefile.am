##---------------------------------------------------------------------------
##       $Id$
##
##       Makefile for TAO's Time Service library
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

lib_LTLIBRARIES = libTAO_Time.la

libTAO_Time_la_LDADD = \
	$(top_builddir)/ace/libACE.la $(top_builddir)/TAO/tao/libTAO.la

libTAO_Time_la_SOURCES = \
        TAO_TIO.cpp \
        TAO_Time_Service_Clerk.cpp \
        TAO_Time_Service_Server.cpp \
        TAO_UTO.cpp \
        Timer_Helper.cpp

noinst_HEADERS = \
        TAO_TIO.h \
        TAO_Time_Service_Clerk.h \
        TAO_Time_Service_Server.h \
        TAO_UTO.h \
        Timer_Helper.h
