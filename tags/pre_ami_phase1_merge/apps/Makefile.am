##---------------------------------------------------------------------------
##       $Id$
##
##       Makefile for the apps directory
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

## The following directories aren't compiled by default since haven't
## finished integrating it into ACE...
##
## Orbix-Examples

if COMPILE_GPERF
GPERF_DIR = gperf
else
GPERF_DIR = 
endif

SUBDIRS = $(GPERF_DIR)
## SUBDIRS = Gateway \
##           JAWS \
##           Orbix-Examples \
##           gperf

## EXTRA_DIST = README
