##  Process this file with automake to create Makefile.in

ACE_BUILDDIR = $(top_builddir)
ACE_ROOT = $(top_srcdir)

noinst_PROGRAMS =

## Makefile.Send_Msg_Receiver.am 

if BUILD_EXCEPTIONS

noinst_PROGRAMS += receiver

receiver_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(ACE_ROOT)/protocols

receiver_SOURCES = \
  Receiver.cpp \
  Protocol.h

receiver_LDADD = \
  ${ACE_BUILDDIR}/protocols/ace/RMCast/libACE_RMCast.la \
  ${ACE_BUILDDIR}/ace/libACE.la

endif BUILD_EXCEPTIONS

## Makefile.Send_Msg_Sender.am 

if BUILD_EXCEPTIONS

noinst_PROGRAMS += sender

sender_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(ACE_ROOT)/protocols

sender_SOURCES = \
  Sender.cpp \
  Protocol.h

sender_LDADD = \
  ${ACE_BUILDDIR}/protocols/ace/RMCast/libACE_RMCast.la \
  ${ACE_BUILDDIR}/ace/libACE.la

endif BUILD_EXCEPTIONS

## Clean up template repositories, etc.
clean-local:
	-rm -f *~ *.bak *.rpo *.sym lib*.*_pure_* core core.*
	-rm -f gcctemp.c gcctemp so_locations *.ics
	-rm -rf cxx_repository ptrepository ti_files
	-rm -rf templateregistry ir.out
	-rm -rf ptrepository SunWS_cache Templates.DB
