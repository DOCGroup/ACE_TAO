##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile.am for ACE_SSL library
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

AM_CPPFLAGS = -I$(top_builddir) -I$(top_srcdir)

lib_LTLIBRARIES = libACE_SSL.la

libACE_SSL_la_SOURCES = \
  SSL_Asynch_BIO.cpp \
  SSL_Asynch_Stream.cpp \
  SSL_Context.cpp \
  SSL_SOCK.cpp \
  SSL_SOCK_Acceptor.cpp \
  SSL_SOCK_Connector.cpp \
  SSL_SOCK_Stream.cpp

libACE_SSL_la_LIBADD = $(top_builddir)/ace/libACE.la

HEADER_FILES = \
  SSL_Asynch_BIO.h \
  SSL_Asynch_Stream.h \
  SSL_Context.h \
  SSL_Export.h \
  SSL_SOCK.h \
  SSL_SOCK_Acceptor.h \
  SSL_SOCK_Connector.h \
  SSL_SOCK_Stream.h \
  sslconf.h

INLINE_FILES = \
  SSL_SOCK.i \
  SSL_SOCK_Acceptor.i \
  SSL_SOCK_Connector.i \
  SSL_SOCK_Stream.i \
  SSL_Context.inl

pkginclude_HEADERS = \
  $(HEADER_FILES) \
  $(INLINE_FILES)
