##----------------------------------------------------------------------------
##       $Id$
##
##       Top-level Makefile.am for the TAO IDL compiler
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

SUBDIRS = driver fe narrow be be_include include ast util

INCLUDES = -I$(top_srcdir)/TAO_IDL/include \
           -I$(top_srcdir)/TAO_IDL/be_include -I$(top_srcdir)/TAO_IDL/fe

man_MANS = tao_idl.1

bin_PROGRAMS = tao_idl

tao_idl_SOURCES = \
        tao_idl.cpp

tao_idl_LDADD = \
        $(top_builddir)/TAO_IDL/driver/libdriver.la  \
        $(top_builddir)/TAO_IDL/fe/libfe.la  \
        $(top_builddir)/TAO_IDL/be/libbe.la  \
        $(top_builddir)/TAO_IDL/ast/libast.la  \
        $(top_builddir)/TAO_IDL/util/libutil.la  \
        $(top_builddir)/TAO_IDL/narrow/libnarrow.la

IDL_CFE_VERSION = \"1.3.0\"

## Additional C++ preprocessor flags to define for the TAO IDL compiler.
AM_CPPFLAGS = \
  -DIDL_CFE_VERSION=$(IDL_CFE_VERSION)

EXTRA_DIST = $(man_MANS)

tags:
	rm -f TAGS
	find . -name "*.h" -print | etags -a --c++ -
	find . -name "*.cpp" -print | etags -a -

## Clean up some additional files/directories possibly created during
## the configure script tests.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations

