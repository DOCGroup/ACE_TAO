##---------------------------------------------------------------------------
##       $Id$
##
##       Makefile for GPERF release
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

bin_PROGRAMS = gperf

gperf_SOURCES = \
         gperf.cpp  \
         Bool_Array.cpp  \
         Gen_Perf.cpp  \
         Hash_Table.cpp  \
         Iterator.cpp  \
         Key_List.cpp  \
         List_Node.cpp  \
         Options.cpp  \
         Vectors.cpp  \
         Version.cpp
gperf_LDADD = $(top_builddir)/ace/libACE.la

noinst_HEADERS = \
         Bool_Array.h  \
         Gen_Perf.h  \
         Hash_Table.h  \
         Iterator.h  \
         Key_List.h  \
         List_Node.h  \
         Options.h  \
         Vectors.h
