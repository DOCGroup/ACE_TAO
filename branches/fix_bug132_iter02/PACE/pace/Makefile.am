##---------------------------------------------------------------------------
##       $Id$
##
##       Makefile for the PACE C library
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

SUBDIRS = \
  @SUBDIRS@

DESTDIRS = \
  config \
  emulation \
  man \
  posix \
  sys \
  vxworks \
  win32

PACE_LIB = @PACE_LIB@

lib_LTLIBRARIES = libPACE.la
libPACE_la_LIBADD = $(PACE_LIB)

pkginclude_HEADERS = \
  aio.h \
  assert.h \
  ctype.h \
  dirent.h \
  errno.h \
  fcntl.h \
  grp.h \
  limits.h \
  locale.h \
  math.h \
  mqueue.h \
  pthread.h \
  pwd.h \
  sched.h \
  semaphore.h \
  setjmp.h \
  signal.h \
  stddef.h \
  stdio.h \
  stdlib.h \
  string.h \
  termios.h \
  time.h \
  unistd.h \
  utime.h

## Clean up some additional files/directories possibly created during
## the configure script tests.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations
