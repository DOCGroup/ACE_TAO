// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_stdio.h
 *
 *  standard buffered input/output
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@dresystems.com>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_OS_STDIO_H
#define ACE_OS_INCLUDE_OS_STDIO_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// NOTE: stdarg.h must be #included before stdio.h on LynxOS.
#include "ace/os_include/stdarg.h"
#include "ace/os_include/stddef.h"

#if !defined (ACE_LACKS_STDIO_H)
#  include /**/ <stdio.h>
#endif /* !ACE_LACKS_STDIO_H */

#if !defined (ACE_WIN32) && !defined (INTEGRITY)
#  define ACE_MAX_USERID L_cuserid
#endif /*!ACE_WIN32*/

// this is a nasty hack to get around problems with the
// pSOS definition of BUFSIZ as the config table entry
// (which is valued using the LC_BUFSIZ value anyway)
#if defined (ACE_PSOS)
#  if defined (BUFSIZ)
#    undef BUFSIZ
#  endif /* defined (BUFSIZ) */
#  define BUFSIZ LC_BUFSIZ
#endif /* defined (ACE_PSOS) */

#if defined (BUFSIZ)
#  define ACE_STREAMBUF_SIZE BUFSIZ
#else
#  define ACE_STREAMBUF_SIZE 1024
#endif /* BUFSIZ */

#if defined (ACE_WIN32) && !defined (ACE_PSOS)
// The following are #defines and #includes that are specific to
// WIN32.
#  if defined (ACE_HAS_WINCE)
#    define ACE_STDIN  _fileno (stdin)
#    define ACE_STDOUT _fileno (stdout)
#    define ACE_STDERR _fileno (stderr)
#  else
#    define ACE_STDIN GetStdHandle (STD_INPUT_HANDLE)
#    define ACE_STDOUT GetStdHandle (STD_OUTPUT_HANDLE)
#    define ACE_STDERR GetStdHandle (STD_ERROR_HANDLE)
#  endif  // ACE_HAS_WINCE
// The following are #defines and #includes that are specific to UNIX.
#else /* !ACE_WIN32 */
#  define ACE_STDIN 0
#  define ACE_STDOUT 1
#  define ACE_STDERR 2
#endif /* ACE_WIN32 */

#if defined (ACE_PSOS_SNARFS_HEADER_INFO)
   // Header information snarfed from compiler provided header files
   // that are not included because there is already an identically
   // named file provided with pSOS, which does not have this info
   // from compiler supplied stdio.h
   extern FILE *fdopen(int, const char *);
   extern char *tempnam(const char *, const char *);
   extern "C" int fileno(FILE *);
#endif /* ACE_PSOS_SNARFS_HEADER_INFO */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_OS_STDIO_H */
