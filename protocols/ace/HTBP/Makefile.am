##  Process this file with automake to create Makefile.in

includedir = @includedir@/ace/HTBP

ACE_BUILDDIR = $(top_builddir)
ACE_ROOT = $(top_srcdir)

## Makefile.HTBP.am 
lib_LTLIBRARIES = libACE_HTBP.la

libACE_HTBP_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -DHTBP_BUILD_DLL

libACE_HTBP_la_SOURCES = \
  HTBP_Addr.cpp \
  HTBP_Channel.cpp \
  HTBP_Environment.cpp \
  HTBP_Filter.cpp \
  HTBP_Filter_Factory.cpp \
  HTBP_ID_Requestor.cpp \
  HTBP_Inside_Squid_Filter.cpp \
  HTBP_Notifier.cpp \
  HTBP_Outside_Squid_Filter.cpp \
  HTBP_Session.cpp \
  HTBP_Stream.cpp

libACE_HTBP_la_LIBADD = \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS = \
  HTBP_Addr.h \
  HTBP_Channel.h \
  HTBP_Environment.h \
  HTBP_Export.h \
  HTBP_Filter.h \
  HTBP_Filter_Factory.h \
  HTBP_ID_Requestor.h \
  HTBP_Inside_Squid_Filter.h \
  HTBP_Macros.h \
  HTBP_Notifier.h \
  HTBP_Outside_Squid_Filter.h \
  HTBP_Session.h \
  HTBP_Stream.h \
  HTBP_Channel.inl \
  HTBP_Filter.inl \
  HTBP_Inside_Squid_Filter.inl \
  HTBP_Notifier.inl \
  HTBP_Outside_Squid_Filter.inl \
  HTBP_Session.inl \
  HTBP_Stream.inl

## Clean up template repositories, etc.
clean-local:
	-rm -f *~ *.bak *.rpo *.sym lib*.*_pure_* core core.*
	-rm -f gcctemp.c gcctemp so_locations *.ics
	-rm -rf cxx_repository ptrepository ti_files
	-rm -rf templateregistry ir.out
	-rm -rf ptrepository SunWS_cache Templates.DB
