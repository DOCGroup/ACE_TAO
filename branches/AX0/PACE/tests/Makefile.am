##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile.am for all the PACE ``one-button'' tests
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

## LDFLAGS = 
## Libtool will automatically link against the "proper" library.
## Do not change the "libACE.la."  It is not a typographical error!
##
## We override the "LIBS" variable provided by the configure script since
## we should only need to link against the ACE library for these tests.
LDADD = $(top_builddir)/pace/libPACE.la
##LIBS = $(top_builddir)/pace/libPACE.la

## Build the following test programs when a `make check' is
## issued by the user.
check_PROGRAMS = \
  Posix_SP_Test \
  Stdio_Test

Posix_SP_Test_SOURCES = Posix_SP_Test.c

Stdio_Test_SOURCES = Stdio_Test.c

## The tests we want to run are all of the test programs we are going to build.
## Do not run them if they are cross-compiled.
if PACE_CROSS_COMPILED
TESTS =
else
TESTS = $(check_PROGRAMS)
endif

## Set up the test environment
##TESTS_ENVIRONMENT = 

## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations

## Clean up `log' directory , etc.
distclean-local:
	-rm -rf log
