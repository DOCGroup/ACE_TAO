// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_string.h
 *
 *  string operations
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@dresystems.com>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_OS_STRING_H
#define ACE_OS_INCLUDE_OS_STRING_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/os_stddef.h"

// Matthew Stevens 7-10-95 Fix GNU GCC 2.7 for memchr() problem.
#if defined (ACE_HAS_GNU_CSTRING_H)
// Define this file to keep /usr/include/memory.h from being included.
# include /**/ <cstring>
#else
# if !defined (ACE_LACKS_MEMORY_H)
#   include /**/ <memory.h>
# endif /* !ACE_LACKS_MEMORY_H */
# if !defined (ACE_LACKS_STRING_H)
#   include /**/ <string.h>
# endif /* !ACE_LACKS_STRING_H */
#endif /* ACE_HAS_GNU_CSTRING_H */

// Place all additions (especially function declarations) within extern "C" {}
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

  // this looks fishy... dhinton
#if !defined (ACE_HAS_STRERROR)
# if defined (ACE_HAS_SYS_ERRLIST)
    extern char *sys_errlist[];
#   define strerror(err) sys_errlist[err]
# else
#   define strerror(err) "strerror is unsupported"
# endif /* ACE_HAS_SYS_ERRLIST */
#endif /* !ACE_HAS_STERROR */

#if defined (ACE_LACKS_STRTOK_R_PROTOTYPE) && !defined (_POSIX_SOURCE)
  char *strtok_r (char *s, const char *delim, char **save_ptr);
#endif  /* ACE_LACKS_STRTOK_R_PROTOTYPE */

#if defined (__BORLANDC__)
#  define _stricmp stricmp
#  define _strnicmp strnicmp
#endif /* __BORLANDC__ */

#if defined (ACE_PSOS_SNARFS_HEADER_INFO)
   // Header information snarfed from compiler provided header files
   // that are not included because there is already an identically
   // named file provided with pSOS, which does not have this info
   // from compiler supplied header
   char *strdup (const char *);  // string.h
#endif /* ACE_PSOS_SNARFS_HEADER_INFO */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#include /**/ "ace/post.h"
#endif /* ACE_OS_INCLUDE_OS_STRING_H */
