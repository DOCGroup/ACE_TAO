##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile.am for the TAO IDL compiler --- 'driver' sub-directory
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir) \
           -I$(top_srcdir)/TAO_IDL/include \
           -I$(top_srcdir)/TAO_IDL/be_include

noinst_LIBRARIES = libdriver.a

libdriver_a_SOURCES = \
         drv_args.cpp  \
         drv_fork.cpp  \
         drv_init.cpp  \
         drv_link.cpp  \
         drv_preproc.cpp  \
         drv_private.cpp

## FIXME: What should CPP_LOCATION be?
##        Is the "+=" a portable make feature?
CXXFLAGS += \
           -DCPP_LOCATION=\"$(CXX)\"
