##---------------------------------------------------------------------------
##       $Id$
##
##       Makefile for TAO's AVStreams Tests -- 
##                    mpeg/source/mpeg_mib directory
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

noinst_LTLIBRARIES = libmpeg_mib

libmpeg_mib_LDADD = \
	$(top_builddir)/ace/libACE.la $(top_builddir)/TAO/tao/libTAO.la

libmpeg_mib_SOURCES = \
        mibButton.cpp \
        mibDrawArea.cpp \
        mibFrame.cpp \
        mibLabel.cpp \
        mibList.cpp \
        mibMenu.cpp \
        mibRadioBox.cpp \
        mibScale.cpp \
        mibScrollBar.cpp \
        mibTextBig.cpp \
        mibTextBox.cpp \
        mibToggle.cpp \
        mibload.cpp

noinst_HEADERS = \
        mibload.h \
        mibwidgets.h
