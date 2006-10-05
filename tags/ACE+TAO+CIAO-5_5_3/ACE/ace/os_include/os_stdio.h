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

#include /**/ "ace/pre.h"

#include "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// NOTE: stdarg.h must be #included before stdio.h on LynxOS.
#include "ace/os_include/os_stdarg.h"
#include "ace/os_include/os_stddef.h"

#if !defined (ACE_LACKS_STDIO_H)
#  include /**/ <stdio.h>
#endif /* !ACE_LACKS_STDIO_H */

#if defined (ACE_VXWORKS)
// for remove(), rename()
#  include /**/ <ioLib.h>
// for remCurIdGet()
#  include /**/ <remLib.h>
#  if defined (__RTP__) && ((ACE_VXWORKS == 0x620) || (ACE_VXWORKS == 0x630))
#    define L_cuserid       _PARM_L_cuserid
#  endif
#endif /* ACE_VXWORKS */

// Undefine "functions" that may be implemented as macros.
#ifdef fgetc
#undef fgetc
#endif
#ifdef fputc
#undef fputc
#endif

// Place all additions (especially function declarations) within extern "C" {}
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

# if defined (INTEGRITY)
#   define ACE_MAX_USERID 32
# elif defined (ACE_WIN32)
#   define ACE_MAX_USERID 32
# else
#  define ACE_MAX_USERID L_cuserid
#endif /* INTEGRITY */

#if defined (BUFSIZ)
#  define ACE_STREAMBUF_SIZE BUFSIZ
#else
#  define ACE_STREAMBUF_SIZE 1024
#endif /* BUFSIZ */

#if defined (ACE_WIN32)
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

#if defined (ACE_WIN32)
  typedef OVERLAPPED ACE_OVERLAPPED;
#else
  struct ACE_OVERLAPPED
  {
    unsigned long Internal;
    unsigned long InternalHigh;
    unsigned long Offset;
    unsigned long OffsetHigh;
    ACE_HANDLE hEvent;
  };
#endif /* ACE_WIN32 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#include /**/ "ace/post.h"
#endif /* ACE_OS_INCLUDE_OS_STDIO_H */
