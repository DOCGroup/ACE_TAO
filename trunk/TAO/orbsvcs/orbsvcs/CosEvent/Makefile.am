##---------------------------------------------------------------------------
##       $Id$
##
##       Makefile for TAO's CosEvent Service library
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

lib_LTLIBRARIES = libTAO_CosEvent.la

libTAO_CosEvent_la_LDADD = \
	$(top_builddir)/ace/libACE.la $(top_builddir)/TAO/tao/libTAO.la

libTAO_CosEvent_la_SOURCES = \
        ConsumerAdmin_i.cpp \
        EventChannel_i.cpp \
        ProxyPushConsumer_i.cpp \
        ProxyPushSupplier_i.cpp \
        SupplierAdmin_i.cpp

noinst_HEADERS = \
        ConsumerAdmin_i.h \
        EventChannel_i.h \
        ProxyPushConsumer_i.h \
        ProxyPushSupplier_i.h \
        SupplierAdmin_i.h
