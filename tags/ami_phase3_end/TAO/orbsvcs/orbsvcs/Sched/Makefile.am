##---------------------------------------------------------------------------
##       $Id$
##
##       Makefile for TAO's Scheduling Service library
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

lib_LTLIBRARIES = libTAO_Sched.la

libTAO_Sched_la_LDADD = \
	$(top_builddir)/ace/libACE.la $(top_builddir)/TAO/tao/libTAO.la

libTAO_Sched_la_SOURCES = \
        Config_Scheduler.cpp \
        DynSched.cpp \
        SchedEntry.cpp \
        Scheduler.cpp \
        Scheduler_Generic.cpp \
        Strategy_Scheduler.cpp

noinst_HEADERS = \
        Config_Scheduler.h \
        DynSched.h \
        SchedEntry.h \
        Scheduler.h \
        Scheduler_Generic.h \
        Strategy_Scheduler.h

## Clean up some additional files/directories possibly created during
## the configure script tests.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations

