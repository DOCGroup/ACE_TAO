##  Process this file with automake to create Makefile.in

ACE_ROOT = $(top_srcdir)/..
ACE_BUILDDIR = $(top_builddir)/..
TAO_ROOT = $(top_srcdir)
TAO_BUILDDIR = $(top_builddir)

## Makefile.catior.am 

if BUILD_RT_CORBA

bin_PROGRAMS = catior

catior_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR)

catior_SOURCES = \
  catior.cpp

catior_LDADD = \
  $(TAO_BUILDDIR)/tao/libTAO_RTCORBA.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

endif BUILD_RT_CORBA

## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -f gcctemp.c gcctemp so_locations
	-rm -rf ptrepository SunWS_cache Templates.DB
