# $Id$
# Copyright (C) 1989, 1992, 1993 Free Software Foundation, Inc.
# written by Douglas C. Schmidt (schmidt@ics.uci.edu)
# 
# This file is part of GNU GPERF.
# 
# GNU GPERF is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 1, or (at your option)
# any later version.
# 
# GNU GPERF is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with GNU GPERF; see the file COPYING.  If not, write to the Free
# Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111, USA. 

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

srcdir = .
GPERF = ../src/gperf

all:
	@echo "performing some tests of the perfect hash generator"   
	$(COMPILE.cc) $(srcdir)/test.cpp
	$(GPERF) -a -p -c -l -S1 -o $(srcdir)/c.gperf > cinset.cpp
	$(CXX) $(CCFLAGS) -o cout cinset.cpp test.o $(LDFLAGS) $(VLDLIBS)
	./cout -v < $(srcdir)/c.gperf > c.out
	-diff -b $(srcdir)/c.exp c.out
	$(GPERF) -a -k1,4,'$$' $(srcdir)/ada.gperf > adainset.cpp
# double '$$' is only there since make gets confused; programn wants only 1 '$'
	$(CXX) $(CCFLAGS) -o aout adainset.cpp test.o $(LDFLAGS) $(VLDLIBS)
	./aout -v < $(srcdir)/ada.gperf > ada-res.out
	-diff -b $(srcdir)/ada-res.exp ada-res.out
	$(GPERF) -a -p -D -k1,'$$' -s 2 -o $(srcdir)/adadefs.gperf > preinset.cpp
	$(CXX) $(CCFLAGS) -o preout preinset.cpp test.o  $(LDFLAGS) $(VLDLIBS)
	./preout -v < $(srcdir)/adadefs.gperf > ada-pred.out
	-diff -b $(srcdir)/ada-pred.exp ada-pred.out
	$(GPERF) -a -k1,2,'$$' -o $(srcdir)/modula3.gperf > m3inset.cpp
	$(CXX) $(CCFLAGS) -o m3out m3inset.cpp test.o  $(LDFLAGS) $(VLDLIBS)
	./m3out -v < $(srcdir)/modula3.gperf > modula.out
	-diff -b $(srcdir)/modula.exp modula.out
	$(GPERF) -a -o -S2 -p < $(srcdir)/pascal.gperf > pinset.cpp
	$(CXX) $(CCFLAGS) -o pout pinset.cpp test.o $(LDFLAGS) $(VLDLIBS)
	./pout -v < $(srcdir)/pascal.gperf > pascal.out
	-diff -b $(srcdir)/pascal.exp pascal.out
	$(GPERF) -m -M -J -c -C -D -E -T -a -o -p -L C++ < $(srcdir)/idl.gperf > iinset.cpp
	$(CXX) $(CCFLAGS) -o iout iinset.cpp test.o $(LDFLAGS) $(VLDLIBS)
	./iout -v < $(srcdir)/idl.gperf > idl.out
	-diff -b $(srcdir)/idl.exp idl.out
# these next 5 are demos that show off the generated code
	$(GPERF) -p -j1 -o -t -N is_reserved_word -k1,3,'$$' < $(srcdir)/c-parse.gperf | egrep -v 'ing time is ' > test-1.out
	-diff -b $(srcdir)/test-1.exp test-1.out
	$(GPERF) -n -k1-8 -l <$(srcdir)/modula2.gperf | egrep -v 'ing time is ' > test-2.out
	-diff -b $(srcdir)/test-2.exp test-2.out
	$(GPERF) -p -j 1 -o -a -C -g -t -k1,4,$$ < $(srcdir)/gplus.gperf | egrep -v 'ing time is ' > test-3.out
	-diff -b $(srcdir)/test-3.exp test-3.out
	$(GPERF) -D -p -t < $(srcdir)/c-parse.gperf | egrep -v 'ing time is ' > test-4.out
	-diff -b $(srcdir)/test-4.exp test-4.out
	$(GPERF) -g -o -j1 -t -p -N is_reserved_word < $(srcdir)/gpc.gperf | egrep -v 'ing time is ' > test-5.out
	-diff -b $(srcdir)/test-5.exp test-5.out
# prints out the help message
	-$(GPERF) -a -h > test-6.out 2>&1 || [ a = a ]
	-diff -b $(srcdir)/test-6.exp test-6.out
	./aout -v < $(srcdir)/c.gperf > test-7.out
	-diff -b $(srcdir)/test-7.exp test-7.out

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
