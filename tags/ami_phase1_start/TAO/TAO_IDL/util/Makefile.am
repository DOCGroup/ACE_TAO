##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile.am for the TAO IDL compiler --- 'util' sub-directory
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

## SUBDIRS = utl_tmpl

INCLUDES = -I$(top_builddir) -I$(top_srcdir) \
           -I$(top_srcdir)/TAO_IDL/include

noinst_LIBRARIES = libutil.a

libutil_a_SOURCES = \
         utl_decllist.cpp  \
         utl_error.cpp  \
         utl_exceptlist.cpp  \
         utl_exprlist.cpp  \
         utl_global.cpp  \
         utl_identifier.cpp  \
         utl_idlist.cpp  \
         utl_indenter.cpp  \
         utl_labellist.cpp  \
         utl_list.cpp  \
         utl_namelist.cpp  \
         utl_scope.cpp  \
         utl_stack.cpp  \
         utl_string.cpp  \
         utl_strlist.cpp

## Clean up some additional files/directories possibly created during
## the configure script tests.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations

