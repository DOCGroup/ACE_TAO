// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_Memory.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt <schmidt@cs.wustl.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 */
//=============================================================================

#ifndef ACE_OS_MEMORY_H
#define ACE_OS_MEMORY_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/OS_Export.h"
#if defined (ACE_HAS_PACE)
# include /**/ "pace/stdlib.h"
#endif /* ACE_HAS_PACE */
#include /**/ <stddef.h>

# if !defined (ACE_MALLOC_ALIGN)
#   define ACE_MALLOC_ALIGN ((int) sizeof (long))
# endif /* ACE_MALLOC_ALIGN */

# if !defined (ACE_HAS_POSITION_INDEPENDENT_POINTERS)
#   define ACE_HAS_POSITION_INDEPENDENT_POINTERS 1
# endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS */

// Allow an installation to replace the lowest-level allocation
// functions without changing the source of ACE.
//
// To do this, simple #define ACE_*_FUNC macros in config.h to
// the names of the site-specific functions, e.g.,
//
//   #define ACE_MALLOC_FUNC  dlmalloc
//   #define ACE_CALLOC_FUNC  dlcalloc
//   #define ACE_FREE_FUNC    dlfree
//   #define ACE_REALLOC_FUNC dlrealloc
//
// For completeness' sake, you should probably put
//   #define ACE_HAS_STRDUP_EMULATION
// too, so that you guarantee that strdup() calls your desired mallocator
// and not the system mallocator.
//
# if defined (ACE_HAS_PACE)
#  if !defined (ACE_MALLOC_FUNC)
#    define ACE_MALLOC_FUNC pace_malloc
#  endif
#  if !defined (ACE_CALLOC_FUNC)
#    define ACE_CALLOC_FUNC pace_calloc
#  endif
#  if !defined (ACE_FREE_FUNC)
#    define ACE_FREE_FUNC pace_free
#  endif
#  if !defined (ACE_REALLOC_FUNC)
#    define ACE_REALLOC_FUNC pace_realloc
#  endif

#  if defined (ACE_HAS_OLD_MALLOC)
typedef char *ACE_MALLOC_T;
#  else
typedef void *ACE_MALLOC_T;
#  endif /* ACE_HAS_OLD_MALLOC */

# else

#  if !defined (ACE_MALLOC_FUNC)
#    define ACE_MALLOC_FUNC ::malloc
#  endif
#  if !defined (ACE_CALLOC_FUNC)
#    define ACE_CALLOC_FUNC ::calloc
#  endif
#  if !defined (ACE_FREE_FUNC)
#    define ACE_FREE_FUNC ::free
#  endif
#  if !defined (ACE_REALLOC_FUNC)
#    define ACE_REALLOC_FUNC ::realloc
#  endif

#  if defined (ACE_HAS_OLD_MALLOC)
typedef char *ACE_MALLOC_T;
#  else
typedef void *ACE_MALLOC_T;
#  endif /* ACE_HAS_OLD_MALLOC */
#endif /* ACE_HAS_PACE */

/**
 * @class ACE_OS_Memory
 *
 * @brief This class is a wrapper for dynamic memory operations.
 *
 */
class ACE_OS_Export ACE_OS_Memory
{
public:
  // = A set of wrappers for memory managment.
  static void *sbrk (int brk);
  static void *calloc (size_t elements, size_t sizeof_elements);
  static void *malloc (size_t);
  static void *realloc (void *, size_t);
  static void free (void *);
};

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_Memory.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

#include "ace/post.h"
#endif /* ACE_OS_MEMORY_H */
