// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_stdlib.h
 *
 *  standard library definitions
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@dresystems.com>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_OS_STDLIB_H
#define ACE_OS_INCLUDE_OS_STDLIB_H

#include /**/ "ace/pre.h"

#include "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/os_stddef.h"
#include "ace/os_include/sys/os_wait.h"

#if defined (ACE_HAS_ALLOCA_H)
#   include /**/ <alloca.h>
#endif /* ACE_HAS_ALLOCA_H */

#if !defined (ACE_LACKS_STDLIB_H)
#  include /**/ <stdlib.h>
#endif /* !ACE_LACKS_STDLIB_H */

#if defined (VXWORKS)
#  include /**/ <envLib.h>
#endif /* VXWORKS */

// Place all additions (especially function declarations) within extern "C" {}
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#if defined (ACE_HAS_BROKEN_RANDR)
   // The SunOS 5.4.X version of rand_r is inconsistent with the header
   // files...
   typedef u_int ACE_RANDR_TYPE;
   int rand_r (ACE_RANDR_TYPE seed);
#else
#  if defined (HPUX_10)
     // HP-UX 10.x's stdlib.h (long *) doesn't match that man page (u_int *)
     typedef long ACE_RANDR_TYPE;
#  else
     typedef u_int ACE_RANDR_TYPE;
#  endif /* HPUX_10 */
#endif /* ACE_HAS_BROKEN_RANDR */

#if defined (ACE_PSOS_SNARFS_HEADER_INFO)
   // Header information snarfed from compiler provided header files
   // that are not included because there is already an identically
   // named file provided with pSOS, which does not have this info
   // from compiler supplied header
   int putenv (char *); // stdlib.h
#endif /* ACE_PSOS_SNARFS_HEADER_INFO */

// These prototypes are chronically lacking from many versions of
// UNIX.
#if !defined (ACE_WIN32) && defined (ACE_LACKS_MKTEMP)
  char *mktemp (char *);
#endif /* !ACE_WIN32 && ACE_LACKS_MKTEMP */

#if defined (DIGITAL_UNIX)
  extern int _Prand_r (unsigned int *seedptr);
#endif /* DIGITAL_UNIX */

#if defined (__Lynx__)
  int putenv (char *);
  char *mktemp (char *);
  int mkstemp(char *);
#endif /* __Lynx__ */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#include /**/ "ace/post.h"
#endif /* ACE_OS_INCLUDE_OS_STDLIB_H */
