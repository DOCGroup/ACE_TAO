// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_mman.h
 *
 *  memory management declarations
 *
 *  @author Don Hinton <dhinton@dresystems.com>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_SYS_OS_MMAN_H
#define ACE_OS_INCLUDE_SYS_OS_MMAN_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/sys/os_types.h"

#if defined (ACE_LACKS_MMAP)
#  define ACE_LACKS_SYS_MMAN_H
#endif /* ACE_LACKS_MMAP */

#if !defined (ACE_LACKS_SYS_MMAN_H)
#  include /**/ <sys/mman.h>
#endif /* ACE_LACKS_SYS_MMAN_H */

// Place all additions (especially function declarations) within extern "C" {}
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#if defined (ACE_LACKS_SYS_MMAN_H) && !defined (ACE_WIN32)
#  define PROT_READ 0
#  define PROT_WRITE 0
#  define PROT_EXEC 0
#  define PROT_NONE 0
#  define PROT_RDWR 0
#  define MAP_PRIVATE 0
#  define MAP_SHARED 0
#  define MAP_FIXED 0
#elif defined (ACE_WIN32)
   // These two may be used for internal flags soon:
#  if !defined (MAP_PRIVATE)
#    define MAP_PRIVATE 1
#  endif
#  if !defined (MAP_SHARED)
#    define MAP_SHARED  2
#  endif
#  if !defined (MAP_FIXED)
#    define MAP_FIXED   4
#  endif
   // MMAP flags
#  if !defined (PROT_READ)
#    define PROT_READ PAGE_READONLY
#  endif
#  if !defined (PROT_WRITE)
#    define PROT_WRITE PAGE_READWRITE
#  endif
#  if !defined (PROT_RDWR)
#    define PROT_RDWR PAGE_READWRITE
#  endif
#endif /* !ACE_LACKS_SYS_MMAN_H && !ACE_WIN32*/

# if !defined (ACE_MAP_PRIVATE)
#   define ACE_MAP_PRIVATE MAP_PRIVATE
# endif /* ! ACE_MAP_PRIVATE */

# if !defined (ACE_MAP_SHARED)
#   define ACE_MAP_SHARED MAP_SHARED
# endif /* ! ACE_MAP_SHARED */

# if !defined (ACE_MAP_FIXED)
#   define ACE_MAP_FIXED MAP_FIXED
# endif /* ! ACE_MAP_FIXED */

# if !defined (MAP_FAILED)
#   define MAP_FAILED ((void *) -1)
# endif /* !MAP_FAILED */

#if !defined (PROT_RDWR)
#  define PROT_RDWR (PROT_READ|PROT_WRITE)
#endif /* PROT_RDWR */

# if defined (ACE_WIN32)
    // Needed to map calls to NT transparently.
#   define MS_ASYNC 0
#   define MS_INVALIDATE 0
# endif /* ACE_WIN32 */

# if !defined (MS_SYNC)
#   define MS_SYNC 0x0
# endif /* !MS_SYNC */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#include /**/ "ace/post.h"
#endif /* ACE_OS_INCLUDE_SYS_OS_MMAN_H */
