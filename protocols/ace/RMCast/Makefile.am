##  Process this file with automake to create Makefile.in
##
## $Id$
##

includedir = @includedir@/ace/RMCast
pkgconfigdir = @libdir@/pkgconfig

ACE_BUILDDIR = $(top_builddir)
ACE_ROOT = $(top_srcdir)


## Makefile.RMCast.am

if BUILD_THREADS

lib_LTLIBRARIES = libACE_RMCast.la

libACE_RMCast_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -DACE_RMCAST_BUILD_DLL

libACE_RMCast_la_SOURCES = \
  Acknowledge.cpp \
  Link.cpp \
  Protocol.cpp \
  Retransmit.cpp \
  Simulator.cpp \
  Socket.cpp \
  Stack.cpp

libACE_RMCast_la_LDFLAGS = \
 -version-number @ACE_MAJOR@:@ACE_MINOR@:@ACE_BETA@

libACE_RMCast_la_LIBADD = \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS = \
  Acknowledge.h \
  Bits.h \
  Link.h \
  Protocol.h \
  RMCast_Export.h \
  Retransmit.h \
  Simulator.h \
  Socket.h \
  Stack.h

pkgconfig_DATA = ACE_RMCast.pc

ACE_RMCast.pc: ${top_builddir}/config.status ${srcdir}/ACE_RMCast.pc.in
	${top_builddir}/config.status --file $@:${srcdir}/ACE_RMCast.pc.in

endif BUILD_THREADS

EXTRA_DIST = \
  ACE_RMCast.pc.in


## Clean up template repositories, etc.
clean-local:
	-rm -f *~ *.bak *.rpo *.sym lib*.*_pure_* core core.*
	-rm -f gcctemp.c gcctemp so_locations *.ics
	-rm -rf cxx_repository ptrepository ti_files
	-rm -rf templateregistry ir.out
	-rm -rf ptrepository SunWS_cache Templates.DB
