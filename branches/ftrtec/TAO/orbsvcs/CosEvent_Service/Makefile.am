##---------------------------------------------------------------------------
##       $Id$
##
##       Makefile for TAO's CosEvent Service
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

bin_PROGRAMS = CosEvent_Service

CosEvent_Service_LDADD = \
	$(top_builddir)/ace/libACE.la \
	$(top_builddir)/TAO/tao/libTAO.la \
	$(top_builddir)/TAO/tao/libTAO_CosEvent.la \
	$(top_builddir)/TAO/tao/libTAO_Svc_Utils.la

CosEvent_Service_SOURCES = \
        CosEvent_Service.cpp

EXTRA_DIST = README

## Clean up some additional files/directories possibly created during
## the configure script tests.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations

