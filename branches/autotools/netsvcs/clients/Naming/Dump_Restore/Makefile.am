##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile for the ACE Dump-Restore Name_Server utility
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

AM_CPPFLAGS = -I$(top_builddir) -I$(top_srcdir)

noinst_LIBRARIES = \
         libDump_Restore.a

libDump_Restore_a_SOURCES = Dump_Restore.cpp Dump_Restore.h


noinst_PROGRAMS = \
         main

main_SOURCES = main.cpp
main_LDADD = \
      $(top_builddir)/netsvcs/clients/Naming/Dump_Restore/libDump_Restore.a \
      $(top_builddir)/ace/libACE.la

## ACE currently doesn't build this so we don't build it with automake either.
EXTRA_DIST = createfile.cpp

