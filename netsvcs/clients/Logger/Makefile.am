##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile for client logging applications
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

AM_CPPFLAGS = -I$(top_builddir) -I$(top_srcdir)

## LDFLAGS = 
## Libtool will automatically link against the "proper" library.
## Do not change the "libACE.la."  It is not a typographical error!
LDADD = $(top_builddir)/ace/libACE.la

noinst_PROGRAMS = \
         direct_logging \
         indirect_logging


direct_logging_SOURCES = direct_logging.cpp

indirect_logging_SOURCES = indirect_logging.cpp
