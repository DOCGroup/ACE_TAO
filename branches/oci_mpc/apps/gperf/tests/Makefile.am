##---------------------------------------------------------------------------
##       $Id$
##
## Copyright (C) 1989, 1992, 1993 Free Software Foundation, Inc.
## Original makefile written by Douglas C. Schmidt (schmidt@ics.uci.edu)
## This Makefile.am written by Ossama Othman (othman@astrosun.tn.cornell.edu)
## 
## This file is part of GNU GPERF.
## 
## GNU GPERF is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 1, or (at your option)
## any later version.
## 
## GNU GPERF is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with GNU GPERF; see the file COPYING.  If not, write to the Free
## Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111, USA. 
##
##       Makefile for GPERF tests
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4


noinst_PROGRAMS = cout aout c++out preout m3out pout iout iout2 tout

cout_SOURCES = cinset.cpp test.cpp

aout_SOURCES = adainset.cpp test.cpp

c++out_SOURCES = c++inset.cpp test.cpp

preout_SOURCES = preinset.cpp test.cpp

m3out_SOURCES = m3inset.cpp test.cpp

pout_SOURCES = pinset.cpp test.cpp

iout_SOURCES = iinset.cpp test.cpp

iout_SOURCES = iinset2.cpp test.cpp

tout_SOURCES = tinset.cpp test.cpp

BUILT_SOURCES = \
        cinset.cpp \
        adainset.cpp \
        c++inset.cpp \
        preinset.cpp \
        m3inset.cpp \
        pinset.cpp \
        iinset.cpp \
        iinset2.cpp \
        tinset.cpp

EXTRA_DIST = \
        gpc.gperf \
        gplus.gperf \
        irc.gperf \
        makeinfo.gperf \
        modula2.gperf \
        ada-pred.exp \
        ada-res.exp \
        c++-res.exp \
        c.exp \
        corba.exp \
        idl.exp \
        modula.exp \
        pascal.exp \
        test-1.exp \
        test-2.exp \
        test-3.exp \
        test-4.exp \
        test-5.exp \
        test-6.exp \
        test-7.exp

cinset.cpp: c.gperf
        $(GPERF) -a -p -c -l -S1 -o $(srcdir)/c.gperf > cinset.cpp

adainset.cpp: ada.gperf
        $(GPERF) -a -k1,4,'$$' $(srcdir)/ada.gperf > adainset.cpp

c++inset.cpp: c++.gperf
        $(GPERF) -a -D $(srcdir)/c++.gperf > c++inset.cpp

preinset.cpp: adadefs.gperf
        $(GPERF) -a -p -D -k1,'$$' -s 2 -o $(srcdir)/adadefs.gperf > preinset.cpp

m3inset.cpp: modula3.gperf
        $(GPERF) -a -k1,2,'$$' -o $(srcdir)/modula3.gperf > m3inset.cpp

pinset.cpp: pascal.gperf
        $(GPERF) -a -o -S2 -p < $(srcdir)/pascal.gperf > pinset.cpp

iinset.cpp: idl.gperf
        $(GPERF) -m -M -c -C -D -S1 -E -T -a -o -p < $(srcdir)/idl.gperf > iinset.cpp

iinset2.cpp: idl.gperf
        $(GPERF) -m -M -c -C -D -E -T -a -o -p < $(srcdir)/idl.gperf > iinset.cpp

tinset.cpp: corba.gperf
        $(GPERF) -a -o < $(srcdir)/corba.gperf > tinset.cpp

