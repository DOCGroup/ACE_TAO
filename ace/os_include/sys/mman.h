// -*- C++ -*-

//=============================================================================
/**
 *  @file    mman.h
 *
 *  memory management declarations
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@ieee.org>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_SYS_MMAN_H
#define ACE_OS_INCLUDE_SYS_MMAN_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/sys/types.h"

#if !defined (ACE_LACKS_SYS_MMAN_H)
# include /**/ <sys/mman.h>
#endif /* !ACE_LACKS_SYS_MMAN_H */

#if defined (ACE_WIN32)
// These two may be used for internal flags soon:
#   define MAP_PRIVATE 1
#   define MAP_SHARED  2
#   define MAP_FIXED   4

// MMAP flags
#   define PROT_READ PAGE_READONLY
#   define PROT_WRITE PAGE_READWRITE
#   define PROT_RDWR PAGE_READWRITE
/* If we can find suitable use for these flags, here they are:
PAGE_WRITECOPY
PAGE_EXECUTE
PAGE_EXECUTE_READ
PAGE_EXECUTE_READWRITE
PAGE_EXECUTE_WRITECOPY
PAGE_GUARD
PAGE_NOACCESS
PAGE_NOCACHE  */
#endif /* ACE_WIN32 */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_SYS_MMAN_H */
