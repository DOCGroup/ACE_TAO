##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile.am for the ACE documentation directory
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

## Build man3 before html directory!
SUBDIRS = man3 html

EXTRA_DIST = acewindex.html


acewindex.html:
	$(top_srcdir)/generate_html_windex $(top_srcdir)/man/man3/ACE*.3
