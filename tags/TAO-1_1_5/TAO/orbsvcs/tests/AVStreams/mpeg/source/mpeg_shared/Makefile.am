##---------------------------------------------------------------------------
##       $Id$
##
##       Makefile for TAO's AVStreams Tests -- 
##                    mpeg/source/mpeg_shared directory
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

noinst_LTLIBRARIES = libmpeg_shared

libmpeg_shared_LDADD = \
	$(top_builddir)/ace/libACE.la $(top_builddir)/TAO/tao/libTAO.la

libmpeg_shared_SOURCES = \
        com.cpp \
        fileio.cpp \
        filters.cpp \
        routine.cpp \
        sendpt.cpp \
        Video_ControlS.cpp \
        Video_ControlC.cpp \
        Audio_ControlS.cpp \
        Audio_ControlC.cpp \
        ReceiverS.cpp \
        ReceiverC.cpp \
        MMDevice_ExporterC.cpp \
        MMDevice_ExporterS.cpp

noinst_HEADERS = \
        com.h \
        fileio.h \
        filters.h \
        routine.h \
        sendpt.h \
        Video_ControlS.h \
        Video_ControlC.h \
        Audio_ControlS.h \
        Audio_ControlC.h \
        ReceiverS.h \
        ReceiverC.h \
        MMDevice_ExporterC.h \
        MMDevice_ExporterS.h

IDL_SOURCES = \
        Audio_Control.idl \
        MMDevice_Exporter.idl \
        Receiver.idl \
        Video_Control.idl

BUILT_SOURCES = \
        Video_ControlS.cpp \
        Video_ControlS.h \
        Video_ControlC.cpp \
        Video_ControlC.h \
        Audio_ControlS.cpp \
        Audio_ControlS.h \
        Audio_ControlC.cpp \
        Audio_ControlC.h \
        ReceiverS.cpp \
        ReceiverS.h \
        ReceiverC.cpp \
        ReceiverC.h \
        MMDevice_ExporterC.cpp \
        MMDevice_ExporterC.h \
        MMDevice_ExporterS.cpp \
        MMDevice_ExporterS.h

EXTRA_DIST = $(IDL_SOURCES)

## Clean up some additional files/directories possibly created during
## the configure script tests.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations

