##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile.am for the TAO IDL compiler --- 'fe' sub-directory
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir) \
           -I$(top_srcdir)/TAO_IDL/include

noinst_LTLIBRARIES = libfe.la

libfe_la_SOURCES = \
  fe_declarator.cpp \
  fe_extern.cpp \
  fe_init.cpp \
  fe_interface_header.cpp \
  fe_lookup.cpp \
  fe_private.cpp \
  lex.yy.cpp \
  tmplinst.cpp \
  y.tab.cpp


noinst_HEADERS = y.tab.h

##BUILT_SOURCES = fe_lookup.cpp lex.yy.cpp y.tab.cpp y.tab.h

fe_lookup.cpp: keywords.dat
	rm -f fe_lookup.cpp
	echo "// \$$Id\$$" > fe_lookup.cpp
	echo "#include	\"idl.h\"" >> fe_lookup.cpp
	echo "#include	\"idl_extern.h\"" >> fe_lookup.cpp
	echo "#include	\"fe_private.h\"" >> fe_lookup.cpp
	$(GPERF) -M -J -c -C -D -E -T -F 0 -f 0 -a -o -t -p -K keyword_  \
			-L C++ -Z TAO_IDL_CPP_Keyword_Table -N lookup -k1,2,$$ keywords.dat \
			 >> fe_lookup.cpp

lex.yy.cpp: idl.ll
	@echo "WARNING: Potentially outdated fe/lex.yy.cpp"
	@echo "         either touch(1) the file or explicitly"
	@echo "         generate it using:"
	@echo "    make lex.yy.cpp.target"

.PHONY: lex.yy.cpp.target

lex.yy.cpp.target:
	$(LEX) -t idl.ll | \
	sed -e "s/yy/tao_yy/g" \
            -e "s/YY/TAO_YY/g" \
            -e "s@#include <unistd\.h>@#include \"ace/OS.h\"@" \
            -e "/#include </d" \
            -e "s@ECHO@TAO_YY_ECHO@" \
            -e 's/\$$Hea''der.*\$$/$$I''d$$/' > lex.yy.cpp
	patch < lex.yy.cpp.diff
	echo timestamp > $@

y.tab.cpp: idl.yy
	@echo "WARNING: Potentially outdated fe/y.tab.cpp"
	@echo "         either touch(1) the file or explicitly"
	@echo "         generate it using:"
	@echo "    make y.tab"

.PHONY: y.tab

y.tab: idl.yy
	$(YACC) -d idl.yy
	sed -e "s/char \*getenv/char *ace_foo/g" \
            -e "s/= getenv/= ACE_OS::getenv/g" \
            -e "s/yynewerror://g" \
            -e "s/yy/tao_yy/g" \
            -e "s/YY/TAO_YY/g" \
	    -e "s/y\.tab\.c/y.tab.cpp/g" < y.tab.c > /tmp/$(USER).Y
	sed -e "s/yy/tao_yy/g" \
            -e "s/YY/TAO_YY/g" < y.tab.h > /tmp/$(USER).YH
	cp /tmp/$(USER).Y y.tab.cpp
	cp /tmp/$(USER).YH y.tab.h
	rm -f /tmp/$(USER).Y /tmp/$(USER).YH y.tab.c y.tab.h
	patch < y.tab.cpp.diff
	echo timestamp > $@

noinst_DATA = keywords.dat

EXTRA_DIST = idl.ll idl.yy keywords.dat lex.yy.cpp.diff y.tab.cpp.diff

## Clean up some additional files/directories possibly created during
## the configure script tests.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations
