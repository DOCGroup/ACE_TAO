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

srcdir = .
GPERF = ../src/gperf

PROG_SRCS = \
	test.cpp \
	cinset.cpp \
	c++inset.cpp \
	iinset.cpp \
	iinset2.cpp \
	tinset.cpp \
	taoinset.cpp \
	pinset.cpp \
	m3inset.cpp \
	adainset.cpp \
	preinset.cpp

SRC = $(PROG_SRCS)

COUT_OBJS = test.o \
	    cinset.o

AOUT_OBJS = test.o \
	    adainset.o

CPPOUT_OBJS = test.o \
	      c++inset.o

PREOUT_OBJS = test.o \
	      preinset.o

M3OUT_OBJS = test.o \
	     m3inset.o

POUT_OBJS = test.o \
	    pinset.o

IOUT_OBJS = test.o \
	    iinset.o

IOUT2_OBJS = test.o \
	     iinset2.o

TOUT_OBJS = test.o \
	    tinset.o

TAOOUT_OBJS = test.o \
	      taoinset.o

BIN  =  cout \
	aout \
	c++out \
	preout \
	m3out \
	pout \
	iout \
	iout2 \
	tout \
	taoout

BUILD = $(BIN)
BUILD+=runtests
VLDLIBS = $(LDLIBS:%=%$(VAR))
VBIN = $(BIN:%=%$(VAR))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------
#rules to make the gperf generated source files..

all: cout \
	aout \
	c++out \
	preout \
	m3out \
	pout \
	iout \
	iout2 \
	tout \
	runtests

cinset.cpp: $(srcdir)/c.gperf $(GPERF)
	$(GPERF) -a -p -c -l -S1 -o $(srcdir)/c.gperf > $@

adainset.cpp: $(srcdir)/ada.gperf $(GPERF)
	$(GPERF)  -a -k1,4,'$$' $(srcdir)/ada.gperf > $@

c++inset.cpp: $(srcdir)/c++.gperf $(GPERF)
	$(GPERF) -a -D $(srcdir)/c++.gperf > $@

preinset.cpp: $(srcdir)/adadefs.gperf $(GPERF)
	$(GPERF)  -a -p -D -k1,'$$' -s 2 -o $(srcdir)/adadefs.gperf > $@

m3inset.cpp: $(srcdir)/modula3.gperf $(GPERF)
	     $(GPERF) -a -k1,2,'$$' -o $(srcdir)/modula3.gperf > $@

pinset.cpp: $(srcdir)/pascal.gperf $(GPERF)
	$(GPERF) -a -o -S2 -p < $(srcdir)/pascal.gperf > $@

iinset.cpp: $(srcdir)/idl.gperf $(GPERF)
	$(GPERF) -m -M -c -C -D -S1 -E -T -a -o -p < $(srcdir)/idl.gperf > $@

# no S1 flag in this case..
iinset2.cpp: $(srcdir)/idl.gperf $(GPERF)
	$(GPERF) -m -M -c -C -D -E -T -a -o -p $(srcdir)/idl.gperf > $@

tinset.cpp:  $(srcdir)/corba.gperf $(GPERF)
	$(GPERF) -a -o < $(srcdir)/corba.gperf > $@

taoinset.cpp:  $(srcdir)/tao.gperf $(GPERF)
	$(GPERF) -c -C -D -E -f 0 -a -o < $(srcdir)/tao.gperf > $@

#rules to make the test executables

cout:$(addprefix $(VDIR),$(COUT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

aout:$(addprefix $(VDIR),$(AOUT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

c++out:$(addprefix $(VDIR),$(CPPOUT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

preout:$(addprefix $(VDIR),$(PREOUT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

m3out:$(addprefix $(VDIR),$(M3OUT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

pout:$(addprefix $(VDIR),$(POUT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

iout:$(addprefix $(VDIR),$(IOUT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

iout2:$(addprefix $(VDIR),$(IOUT2_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

tout:$(addprefix $(VDIR),$(TOUT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

taoout:$(addprefix $(VDIR),$(TAOOUT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

runtests: cout aout c++out preout m3out pout iout iout2 tout taoout
	 @echo "performing some tests of the perfect hash generator"
	 # test cout
	 ./cout -v < $(srcdir)/c.gperf > c.out
	 -diff -b $(srcdir)/c.exp c.out
	 # test aout
	 ./aout -v < $(srcdir)/ada.gperf > ada-res.out
	 -diff -b $(srcdir)/ada-res.exp ada-res.out
	 # test c++out
	 ./c++out -v < $(srcdir)/c++.gperf > c++-res.out
	 -diff -b $(srcdir)/c++-res.exp c++-res.out
	 # test preout
	 ./preout -v < $(srcdir)/adadefs.gperf > ada-pred.out
	 -diff -b $(srcdir)/ada-pred.exp ada-pred.out
	 # test m3out
	 ./m3out -v < $(srcdir)/modula3.gperf > modula.out
	 -diff -b $(srcdir)/modula.exp modula.out
	 # test pout
	 ./pout -v < $(srcdir)/pascal.gperf > pascal.out
	 -diff -b $(srcdir)/pascal.exp pascal.out
	 # test iout
	 ./iout -v < $(srcdir)/idl.gperf > idl.out
	 -diff -b $(srcdir)/idl.exp idl.out
	 # test iout2
	 ./iout2 -v < $(srcdir)/idl.gperf > idl.out
	 -diff -b $(srcdir)/idl.exp idl.out
	 #test tout
	 ./tout -v < $(srcdir)/corba.gperf > corba.out
	 -diff -b $(srcdir)/corba.exp corba.out
	 #test taoout
	 ./taoout -v < $(srcdir)/tao.gperf > tao.out
	 -diff -b $(srcdir)/tao.exp tao.out
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

realclean:
	$(RM) -rf *.out cinset.cpp c++inset.cpp iinset.cpp iinset2.cpp \
	tinset.cpp taoinset.cpp pinset.cpp m3inset.cpp adainset.cpp preinset.cpp
	$(RM) -rf c.out aout preout m3out pout cout c++out iout iout2 tout taoout

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/test.o .obj/test.so .shobj/test.o .shobj/test.so: test.cpp \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 $(ACE_ROOT)/ace/OS_Errno.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/Default_Constants.h \
 $(ACE_ROOT)/ace/Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Trace.h

.obj/cinset.o .obj/cinset.so .shobj/cinset.o .shobj/cinset.so: cinset.cpp

.obj/c++inset.o .obj/c++inset.so .shobj/c++inset.o .shobj/c++inset.so: c++inset.cpp

.obj/iinset.o .obj/iinset.so .shobj/iinset.o .shobj/iinset.so: iinset.cpp

.obj/iinset2.o .obj/iinset2.so .shobj/iinset2.o .shobj/iinset2.so: iinset2.cpp

.obj/tinset.o .obj/tinset.so .shobj/tinset.o .shobj/tinset.so: tinset.cpp

.obj/taoinset.o .obj/taoinset.so .shobj/taoinset.o .shobj/taoinset.so: taoinset.cpp

.obj/pinset.o .obj/pinset.so .shobj/pinset.o .shobj/pinset.so: pinset.cpp

.obj/m3inset.o .obj/m3inset.so .shobj/m3inset.o .shobj/m3inset.so: m3inset.cpp

.obj/adainset.o .obj/adainset.so .shobj/adainset.o .shobj/adainset.so: adainset.cpp

.obj/preinset.o .obj/preinset.so .shobj/preinset.o .shobj/preinset.so: preinset.cpp

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
