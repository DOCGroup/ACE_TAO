##---------------------------------------------------------------------------
##       $Id$
##
##       Makefile for TAO's Trading Service library
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

lib_LTLIBRARIES = libTAO_Trader.la

libTAO_Trader_la_LDADD = \
	$(top_builddir)/ace/libACE.la $(top_builddir)/TAO/tao/libTAO.la

libTAO_Trader_la_SOURCES = \
        Constraint_Interpreter.cpp \
        Constraint_Nodes.cpp \
        Constraint_Visitors.cpp \
        Constraint_l.cpp \
        Constraint_y.cpp \
        Offer_Database.cpp \
        Offer_Iterators.cpp \
        Offer_Iterators_T.cpp \
        Service_Type_Repository.cpp \
        Trader.cpp \
        Trader_Interfaces.cpp \
        Trader_T.cpp \
        Trader_Utils.cpp

noinst_HEADERS = \
        Constraint_Interpreter.h \
        Constraint_Nodes.h \
        Constraint_Tokens.h \
        Constraint_Visitors.h \
        Locking.h \
        Offer_Database.h \
        Offer_Iterators.h \
        Offer_Iterators_T.h \
        Service_Type_Repository.h \
        Trader.h \
        Trader_Interfaces.h \
        Trader_T.h \
        Trader_Utils.h

## Clean up some additional files/directories possibly created during
## the configure script tests.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations

