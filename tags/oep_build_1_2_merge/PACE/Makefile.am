##---------------------------------------------------------------------------
##       $Id$
##
##       Top-level Makefile for the PACE toolkit libraries, tests, and
##       applications
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4 #check-news

SUBDIRS = \
  pace \
  tests

DESTDIRS = \
  docs \
  examples \
  pace++

## Clean up some additional files/directories possibly created during
## the configure script tests.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations
