##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile.am for ACE_QoS library
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

lib_LTLIBRARIES = libACE_QoS.la

libACE_QoS_la_SOURCES = \
  QoS_Decorator.cpp \
  QoS_Manager.cpp \
  QoS_Session_Factory.cpp \
  QoS_Session_Impl.cpp \
  SOCK_Dgram_Mcast_QoS.cpp

libACE_QoS_la_LIBADD = $(top_builddir)/ace/libACE.la

HEADER_FILES = \
  ACE_QoS_Export.h \
  QoS_Decorator.h \
  QoS_Manager.h \
  QoS_Session.h \
  QoS_Session_Factory.h \
  QoS_Session_Impl.h \
  SOCK_Dgram_Mcast_QoS.h

INLINE_FILES = \
  QoS_Session_Impl.i \
  SOCK_Dgram_Mcast_QoS.i

pkginclude_HEADERS = \
  $(HEADER_FILES) \
  $(INLINE_FILES)
