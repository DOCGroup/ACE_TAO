##---------------------------------------------------------------------------
##       $Id$
##
##       Makefile for TAO's AVStreams Tests -- 
##                    mpeg/source/mpeg_server directory
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

noinst_LTLIBRARIES = libmpeg_server

libmpeg_server_LDADD = \
	$(top_builddir)/ace/libACE.la $(top_builddir)/TAO/tao/libTAO.la

libmpeg_server_SOURCES = \
        Audio_Control_State.cpp \
        Audio_Control_i.cpp \
        Audio_Server.cpp \
        Globals.cpp \
        Video_Control_State.cpp \
        Video_Control_i.cpp \
        Video_Server.cpp \
        as.cpp \
        las.cpp \
        lvs.cpp \
        vs.cpp

noinst_HEADERS = \
        Audio_Control_State.h \
        Audio_Control_i.h \
        Audio_Server.h \
        Globals.h \
        Video_Control_State.h \
        Video_Control_i.h \
        Video_Server.h \
        server_proto.h

## Clean up some additional files/directories possibly created during
## the configure script tests.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations

