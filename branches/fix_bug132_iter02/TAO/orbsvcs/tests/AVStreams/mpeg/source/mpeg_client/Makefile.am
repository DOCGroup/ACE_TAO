##---------------------------------------------------------------------------
##       $Id$
##
##       Makefile for TAO's AVStreams Tests -- 
##                    mpeg/source/mpeg_client directory
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

noinst_LTLIBRARIES = libmpeg_client

libmpeg_client_LDADD = \
	$(top_builddir)/ace/libACE.la $(top_builddir)/TAO/tao/libTAO.la

libmpeg_client_SOURCES = \
        24bit.cpp \
        2x2.cpp \
        Command_Handler.cpp \
        ab.cpp \
        audio.cpp \
        ctr.cpp \
        decoders.cpp \
        file.cpp \
        fs2.cpp \
        fs2fast.cpp \
        fs4.cpp \
        global.cpp \
        gray.cpp \
        hybrid.cpp \
        hybriderr.cpp \
        info.cpp \
        jrevdct.cpp \
        mb_ordered.cpp \
        mono.cpp \
        motionvector.cpp \
        ordered.cpp \
        ordered2.cpp \
        para.cpp \
        parseblock.cpp \
        prog.cpp \
        ui.cpp \
        util.cpp \
        util32.cpp \
        vb.cpp \
        vd.cpp \
        video.cpp \
        vp.cpp

noinst_HEADERS = \
        Command_Handler.h \
        ab.h \
        decoders.h \
        dither.h \
        fs2.h \
        fs4.h \
        global.h \
        mibload.h \
        newproto.h \
        proto.h \
        ui.h \
        util.h \
        vb.h \
        video.h

dist-hook:
        $(mkinstalldirs) $(distdir)/$(subdir)
        tar cf - ./interface | (cd $(distdir)/$(subdir); tar xfBp -)

## Clean up some additional files/directories possibly created during
## the configure script tests.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations

