##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile.am for the TAO orbsvcs library
##            orbsvcs subdirectory
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

## # Uncomment out the following line (or invoke make with
## # TAO_LACKS_EVENT_CHANNEL_ANY=1)
## # if you don't want support for anys in the Event Channel.
## #### TAO_LACKS_EVENT_CHANNEL_ANY = 1
## ifneq ($(TAO_LACKS_EVENT_CHANNEL_ANY),)
##   override TAO_LACKS_EVENT_CHANNEL_ANY = -DTAO_LACKS_EVENT_CHANNEL_ANY
## endif

# TODO: This should be a configure script option
# Uncomment out the following line (or invoke make with
# if you don't want support for anys in the Event Channel.
#TAO_LACKS_EVENT_CHANNEL_ANY = -DTAO_LACKS_EVENT_CHANNEL_ANY

TAO_IDLFLAGS = \
        -Wb,export_macro=TAO_ORBSVCS_Export \
        -Wb,export_include=orbsvcs_export.h \
        $(TAO_LACKS_EVENT_CHANNEL_ANY)

# Build a libtool library, liborbsvcs.la for installation in libdir.
lib_LTLIBRARIES = liborbsvcs.la
liborbsvcs_la_SOURCES = \
        CosConcurrencyControlS.h \
        CosConcurrencyControlS.cpp \
        CosConcurrencyControlC.h \
        CosConcurrencyControlC.cpp \
        CosNamingS.h \
        CosNamingS.cpp \
        CosNamingC.h \
        CosNamingC.cpp \
        CosLifeCycleS.h \
        CosLifeCycleS.cpp \
        CosLifeCycleC.h \
        CosLifeCycleC.cpp \
        CosPropertyServiceS.h \
        CosPropertyServiceS.cpp \
        CosPropertyServiceC.h \
        CosPropertyServiceC.cpp \
        CosTimeBaseS.h \
        CosTimeBaseS.cpp \
        CosTimeBaseC.h \
        CosTimeBaseC.cpp \
        LoggerS.h \
        LoggerS.cpp \
        LoggerC.h \
        LoggerC.cpp \
        RtecEventCommS.h \
        RtecEventCommS.cpp \
        RtecEventCommC.h \
        RtecEventCommC.cpp \
        RtecSchedulerS.h \
        RtecSchedulerS.cpp \
        RtecSchedulerC.h \
        RtecSchedulerC.cpp \
        RtecEventChannelAdminS.h \
        RtecEventChannelAdminS.cpp \
        RtecEventChannelAdminC.h \
        RtecEventChannelAdminC.cpp \
        RtecUDPAdminS.h \
        RtecUDPAdminS.cpp \
        RtecUDPAdminC.h \
        RtecUDPAdminC.cpp \
        LifeCycleServiceS.h \
        LifeCycleServiceS.cpp \
        LifeCycleServiceC.h \
        LifeCycleServiceC.cpp \
        CosTradingS.h \
        CosTradingS.cpp \
        CosTradingC.h \
        CosTradingC.cpp \
        AVStreamsS.h \
        AVStreamsS.cpp \
        AVStreamsC.h \
        AVStreamsC.cpp \
        Null_MediaCtrlS.h \
        Null_MediaCtrlS.cpp \
        Null_MediaCtrlC.h \
        Null_MediaCtrlC.cpp \
        \
        

VPATH=.:Naming:Time:Log:Concurrency:Property:Trader:Sched:Event:CosEvent:AV:

## Clean up some additional files/directories possibly created during
## the configure script tests.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations

