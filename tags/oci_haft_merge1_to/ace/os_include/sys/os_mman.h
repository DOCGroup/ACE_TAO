// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_mman.h
 *
 *  memory management declarations
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@dresystems.com>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_SYS_OS_MMAN_H
#define ACE_OS_INCLUDE_SYS_OS_MMAN_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/sys/os_types.h"

#if defined (ACE_LACKS_MMAP)
#  define ACE_LACKS_SYS_MMAN_H
#endif /* ACE_LACKS_MMAP */

#if !defined (ACE_LACKS_SYS_MMAN_H)
   // Fixes a problem with HP/UX.
#  if defined (ACE_HAS_BROKEN_MMAP_H)
     extern "C" {
#  endif /* ACE_HAS_BROKEN_MMAP_H */
#  include /**/ <sys/mman.h>
#  if defined (ACE_HAS_BROKEN_MMAP_H)
     }
#  endif /* ACE_HAS_BROKEN_MMAP_H */
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
#  define MAP_PRIVATE 1
#  define MAP_SHARED  2
#  define MAP_FIXED   4
   // MMAP flags
#  define PROT_READ PAGE_READONLY
#  define PROT_WRITE PAGE_READWRITE
#  define PROT_RDWR PAGE_READWRITE
/* If we can find suitable use for these flags, here they are:
PAGE_WRITECOPY
PAGE_EXECUTE
PAGE_EXECUTE_READ
PAGE_EXECUTE_READWRITE
PAGE_EXECUTE_WRITECOPY
PAGE_GUARD
PAGE_NOACCESS
PAGE_NOCACHE  */
#endif /* !ACE_LACKS_SYS_MMAN_H && !ACE_WIN32*/

#if !defined (PROT_RDWR)
#  define PROT_RDWR (PROT_READ|PROT_WRITE)
#endif /* PROT_RDWR */

# if !defined (MS_SYNC)
#   define MS_SYNC 0x0
# endif /* !MS_SYNC */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_SYS_OS_MMAN_H */
