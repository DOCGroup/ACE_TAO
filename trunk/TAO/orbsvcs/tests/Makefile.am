##---------------------------------------------------------------------------
##       $Id$
##
##       Top-level Makefile for TAO's ORB Services Tests
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

SUBDIRS = \
        AVStreams \
        Concurrency \
        CosEC_Basic \
        CosEC_Multiple \
        EC_Basic \
        EC_Custom_Marshal \
        EC_Mcast \
        EC_Multiple \
        EC_Throughput \
        Event_Latency \
        ImplRepo \
        Logger \
        Property \
        Sched \
        Sched_Conf \
        Simple_Naming \
        Simulator \
        Time \
        Trading

noinst_SCRIPTS = start_services

EXTRA_DIST = $(noinst_SCRIPTS)
