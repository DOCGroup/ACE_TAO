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

## Create a libtool convenience library that contains the contents of
## the POSIX-specific version of PACE.
noinst_LTLIBRARIES = libPACE_POSIX.la

libPACE_POSIX_la_SOURCES = \
  aio.c \
  assert.c \
  ctype.c \
  dirent.c \
  fcntl.c \
  grp.c \
  locale.c \
  math.c \
  mman.c \
  mqueue.c \
  pthread.c \
  pwd.c \
  sched.c \
  semaphore.c \
  setjmp.c \
  signal.c \
  stat.c \
  stdio.c \
  stdlib.c \
  string.c \
  termios.c \
  time.c \
  times.c \
  unistd.c \
  utime.c \
  utsname.c \
  wait.c

PACE_INLINE_FILES = \
  aio.inl \
  assert.inl \
  ctype.inl \
  dirent.inl \
  fcntl.inl \
  grp.inl \
  locale.inl \
  math.inl \
  mman.inl \
  mqueue.inl \
  pthread.inl \
  pwd.inl \
  sched.inl \
  semaphore.inl \
  setjmp.inl \
  signal.inl \
  stat.inl \
  stdio.inl \
  stdlib.inl \
  string.inl \
  termios.inl \
  time.inl \
  times.inl \
  unistd.inl \
  utime.inl \
  utsname.inl \
  wait.inl

PACE_HEADER_FILES = \
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
  mman.h \
  mqueue.h \
  pthread.h \
  pwd.h \
  sched.h \
  semaphore.h \
  setjmp.h \
  signal.h \
  stat.h \
  stdio.h \
  stdlib.h \
  string.h \
  termios.h \
  time.h \
  times.h \
  types.h \
  unistd.h \
  utime.h \
  utsname.h \
  wait.h

pkgincludedir = $(includedir)/$(PACKAGE)/posix
pkginclude_HEADERS = \
  $(PACE_INLINE_FILES) \
  $(PACE_HEADER_FILES)
