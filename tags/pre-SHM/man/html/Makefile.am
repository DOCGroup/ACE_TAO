##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile.am for the ACE `html' man page directory
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

## The HTML man page listing (html_DATA) is inserted by the
## `bootstrap' script.  Don't bother editing this list since it is
## automatically generated.

htmldir = $(pkgdatadir)/html
html_DATA =

MAINTAINERCLEANFILES = $(html_DATA)

## Only add rule to create ACE.html since it more efficient to make
## man2html create all of the html man pages in one shot.
$(html_DATA):
	@ACE_ROOT=$(top_srcdir) $(top_srcdir)/bin/man2html \
	  $(top_srcdir)/man/man3/`echo $@ | sed -e 's/\.html$$/\.3/'` \
	  > /dev/null

## For some reason Automake couldn't automatically copy the HTML man
## pages to the distribution directory so do it manually.
dist-hook:
	(tar cf - ./ACE*.html) | (cd $(distdir); tar xfBp -)
