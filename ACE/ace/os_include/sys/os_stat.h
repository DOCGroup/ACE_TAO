// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_stat.h
 *
 *  data returned by the stat() function
 *
 *  @author Don Hinton <dhinton@dresystems.com>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_SYS_OS_STAT_H
#define ACE_OS_INCLUDE_SYS_OS_STAT_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/sys/os_types.h"

#if defined (ACE_HAS_DIRECT_H)
#  include /**/ <direct.h>
#endif /* ACE_HAS_DIRECT_H */

#if !defined (ACE_LACKS_SYS_STAT_H)
#  include /**/ <sys/stat.h>
#endif /* !ACE_LACKS_SYS_STAT_H */

// Place all additions (especially function declarations) within extern "C" {}
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#if defined (ACE_LACKS_MODE_MASKS)
// MODE MASKS

// the following macros are for POSIX conformance.

#  if !defined (ACE_HAS_USER_MODE_MASKS)
#    if !defined (S_IRWXU)
#      define S_IRWXU 00700         /* read, write, execute: owner. */
#    endif /* !S_IRWXU */
#    if !defined (S_IRUSR)
#      define S_IRUSR 00400         /* read permission: owner. */
#    endif /* !S_IRUSR */
#    if !defined (S_IWUSR)
#      define S_IWUSR 00200         /* write permission: owner. */
#    endif /* !S_IWUSR */
#    if !defined (S_IXUSR)
#      define S_IXUSR 00100         /* execute permission: owner. */
#    endif /* !S_IXUSR */
#  endif /* ACE_HAS_USER_MODE_MASKS */

#  if !defined (S_IRWXG)
#    define S_IRWXG 00070
#  endif /* S_IRWXG */
#  if !defined (S_IRGRP)
#    define S_IRGRP 00040
#  endif /* S_IRGRP */
#  if !defined (S_IWGRP)
#    define S_IWGRP 00020
#  endif /* S_IWGRP */
#  if !defined (S_IXGRP)
#    define S_IXGRP 00010
#  endif /* S_IXGRP */
#  if !defined (S_IRWXO)
#    define S_IRWXO 00007
#  endif /* S_IRWXO */
#  if !defined (S_IROTH)
#    define S_IROTH 00004
#  endif /* S_IROTH */
#  if !defined (S_IWOTH)
#    define S_IWOTH 00002
#  endif /* S_IWOTH */
#  if !defined (S_IXOTH)
#    define S_IXOTH 00001
#  endif /* S_IXOTH */
#  if !defined (S_IFLNK)
#    define S_IFLNK 0200000
#  endif /* S_IFLNK */
#endif /* ACE_LACKS_MODE_MASKS */

// Some systems (VxWorks) don't define S_ISLNK
#if !defined (S_ISLNK)
# if defined (S_IFLNK)
#   define S_ISLNK(mode) (((mode)&S_IFLNK) == S_IFLNK)
# else
#   define S_ISLNK(mode) 0
# endif /* S_IFLNK */
#endif /* S_ISLNK */

// Visual Studio doesn't define S_ISDIR
#if !defined (S_ISDIR)
# define S_ISDIR(mode)   ((mode&S_IFMT) == S_IFDIR)
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#include /**/ "ace/post.h"
#endif /* ACE_OS_INCLUDE_SYS_OS_STAT_H */
