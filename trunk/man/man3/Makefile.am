##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile.am for the ACE `man3' man page directory
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

## Exclude non-ACE man pages
## The distribution must be first be bootstrapped by running
## `bin/bootstrap' from the top-level source directory before the
## following will work.
man_MANS := $(shell echo ACE*.3)

MAINTAINERCLEANFILES = $(man_MANS)

EXTRA_DIST = $(man_MANS)


