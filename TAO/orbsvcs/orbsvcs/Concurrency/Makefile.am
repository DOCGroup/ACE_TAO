##---------------------------------------------------------------------------
##       $Id$
##
##       Makefile for TAO's Concurrency Service library
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

lib_LTLIBRARIES = libTAO_Concurrency.la

libTAO_Concurrency_la_LDADD = \
	$(top_builddir)/ace/libACE.la $(top_builddir)/TAO/tao/libTAO.la

libTAO_Concurrency_la_SOURCES = \
        CC_Lock.cpp \
        CC_LockSet.cpp \
        CC_LockSetFactory.cpp \
        Concurrency_Utils.cpp

noinst_HEADERS = \
        CC_Lock.h \
        CC_LockSet.h \
        CC_LockSetFactory.h \
        Concurrency_Utils.h

