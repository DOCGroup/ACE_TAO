##---------------------------------------------------------------------------
##       $Id$
##
##       Makefile for TAO's AVStreams Tests -- mpeg/source directory
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

SUBDIRS = \
        client \
        mpeg_client \
        mpeg_mib \
        mpeg_server \
        mpeg_shared \
        server

EXTRA_DIST = TODO

dist-hook:
        $(mkinstalldirs) $(distdir)/$(subdir)
        tar cf - ./include | (cd $(distdir)/$(subdir); tar xfBp -)
