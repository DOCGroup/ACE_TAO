##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile.am for all the ACE_SSL ``one-button'' tests
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

AM_CPPFLAGS = @ACE_TLS_CPPFLAGS@

noinst_PROGRAMS = Thread_Pool_Reactor_SSL_Test

Thread_Pool_Reactor_SSL_Test_SOURCES = \
  Thread_Pool_Reactor_SSL_Test.h \
  Thread_Pool_Reactor_SSL_Test.cpp \
  Main.cpp

Thread_Pool_Reactor_SSL_Test_LDADD = \
  $(top_builddir)/tests/libTest_Output.la \
  $(top_builddir)/ace/SSL/libACE_SSL.la

noinst_DATA = \
  dummy.pem \
  key.pem

## Automake recognized target.  Appends these rules to the "all"
## target.
all-local:
	for p in $(noinst_DATA); do \
	  test -f $$p \
	  || $(LN_S) $(srcdir)/$$p $$p; done
	touch tests-link-stamp

## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core core.*
	-rm -rf cxx_repository ptrepository ti_files
	-rm -rf Templates.DB gcctemp.c gcctemp so_locations *.ics
	-rm -rf templateregistry SunWS_cache ir.out


## Clean up files and directories created by this Makefile.
distclean-local:
	-test -f tests-link-stamp \
	   && rm $(noinst_DATA) tests-link-stamp
	-rm -rf log
