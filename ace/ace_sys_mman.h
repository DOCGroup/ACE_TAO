/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_sys_mman.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_SYS_MMAN_H
#define ACE_ACE_SYS_MMAN_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_WIN32)
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
#endif


#   if defined (ACE_LACKS_MMAP)
#     define PROT_READ 0
#     define PROT_WRITE 0
#     define PROT_EXEC 0
#     define PROT_NONE 0
#     define PROT_RDWR 0
#     define MAP_PRIVATE 0
#     define MAP_SHARED 0
#     define MAP_FIXED 0
#   endif /* ACE_LACKS_MMAP */

// Fixes a problem with HP/UX.
#   if defined (ACE_HAS_BROKEN_MMAP_H)
extern "C"
{
#     include /**/ <sys/mman.h>
}
#   elif !defined (ACE_LACKS_MMAP)
#     include /**/ <sys/mman.h>
#   endif /* ACE_HAS_BROKEN_MMAP_H */


# if !defined (PROT_RDWR)
#   define PROT_RDWR (PROT_READ|PROT_WRITE)
# endif /* PROT_RDWR */



#endif /* ACE_ACE_SYS_MMAN_H */
