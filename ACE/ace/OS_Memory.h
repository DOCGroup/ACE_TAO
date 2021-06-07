// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_Memory.h
 *
 *  @author Doug Schmidt <d.schmidt@vanderbilt.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 */
//=============================================================================

#ifndef ACE_OS_MEMORY_H
#define ACE_OS_MEMORY_H
#include /**/ "ace/pre.h"

#include /**/ "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/OS_Errno.h"
#include "ace/Basic_Types.h"
#include "ace/os_include/os_stddef.h"

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
//   #define ACE_HAS_WCSDUP_EMULATION
// too, so that you guarantee that strdup() and wcsdup() call your
// desired mallocator and not the system mallocator.
//
#if !defined (ACE_MALLOC_FUNC)
#  define ACE_MALLOC_FUNC ::malloc
#endif
#if !defined (ACE_CALLOC_FUNC)
#  define ACE_CALLOC_FUNC ::calloc
#endif
#if !defined (ACE_FREE_FUNC)
#  define ACE_FREE_FUNC ::free
#endif
#if !defined (ACE_REALLOC_FUNC)
#  define ACE_REALLOC_FUNC ::realloc
#endif

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

typedef void * ACE_MALLOC_T;

ACE_END_VERSIONED_NAMESPACE_DECL

// For backwards compatibility, we except all compilers to support these
#include /**/ <new>
#define ACE_bad_alloc std::bad_alloc
#define ACE_nothrow   std::nothrow
#define ACE_nothrow_t std::nothrow_t
#define ACE_del_bad_alloc
#define ACE_throw_bad_alloc throw std::bad_alloc ()

// ACE_NEW macros
#define ACE_NEW_RETURN(POINTER,CONSTRUCTOR,RET_VAL) \
   do { POINTER = new (std::nothrow) CONSTRUCTOR; \
     if (POINTER == nullptr) { errno = ENOMEM; return RET_VAL; } \
   } while (0)
#define ACE_NEW(POINTER,CONSTRUCTOR) \
   do { POINTER = new(std::nothrow) CONSTRUCTOR; \
     if (POINTER == nullptr) { errno = ENOMEM; return; } \
   } while (0)
#define ACE_NEW_NORETURN(POINTER,CONSTRUCTOR) \
   do { POINTER = new(std::nothrow) CONSTRUCTOR; \
     if (POINTER == nullptr) { errno = ENOMEM; } \
   } while (0)

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
//@{
/**
 * @name Efficiently compute aligned pointers to powers of 2 boundaries.
 */

/**
 * Efficiently align "value" up to "alignment", knowing that all such
 * boundaries are binary powers and that we're using two's complement
 * arithmetic.
 *
 * Since the alignment is a power of two its binary representation is:
 *
 * alignment      = 0...010...0
 *
 * hence
 *
 * alignment - 1  = 0...001...1 = T1
 *
 * so the complement is:
 *
 * ~(alignment - 1) = 1...110...0 = T2
 *
 * Notice that there is a multiple of @a alignment in the range
 * [<value>,<value> + T1], also notice that if
 *
 * X = ( <value> + T1 ) & T2
 *
 * then
 *
 * <value> <= X <= <value> + T1
 *
 * because the & operator only changes the last bits, and since X is a
 * multiple of @a alignment (its last bits are zero) we have found the
 * multiple we wanted.
 */
/// Return the next integer aligned to a required boundary
/**
 * @param ptr the base pointer
 * @param alignment the required alignment
 */
#if defined (ACE_OPENVMS) && (!defined (__INITIAL_POINTER_SIZE) || (__INITIAL_POINTER_SIZE < 64))
inline unsigned int
ACE_align_binary (unsigned int ptr, unsigned int alignment)
{
  unsigned int const tmp = alignment - 1;
  return (ptr + tmp) & (~tmp);
}
#else
inline uintptr_t
ACE_align_binary (uintptr_t ptr, uintptr_t alignment)
{
  uintptr_t const tmp = alignment - 1;
  return (ptr + tmp) & (~tmp);
}
#endif

#if defined (ACE_OPENVMS) && (!defined (__INITIAL_POINTER_SIZE) || (__INITIAL_POINTER_SIZE < 64))
/// Return the next address aligned to a required boundary
inline char *
ACE_ptr_align_binary (char const * ptr, unsigned int alignment)
{
  return
    reinterpret_cast<char *> (
      ACE_align_binary (reinterpret_cast<unsigned int> (ptr), alignment));
}

/// Return the next address aligned to a required boundary
inline char *
ACE_ptr_align_binary (unsigned char const * ptr, unsigned int alignment)
{
  return
    ACE_ptr_align_binary (reinterpret_cast<char const *> (ptr), alignment);
}
#else
/// Return the next address aligned to a required boundary
inline char *
ACE_ptr_align_binary (char const * ptr, uintptr_t alignment)
{
  return
    reinterpret_cast<char *> (
      ACE_align_binary (reinterpret_cast<uintptr_t> (ptr), alignment));
}

/// Return the next address aligned to a required boundary
inline char *
ACE_ptr_align_binary (unsigned char const * ptr, uintptr_t alignment)
{
  return
    ACE_ptr_align_binary (reinterpret_cast<char const *> (ptr), alignment);
}
#endif  /* ACE_OPENVMS && __INITIAL_POINTER_SIZE < 64 */
//@}
ACE_END_VERSIONED_NAMESPACE_DECL

#include "ace/OS_NS_stdlib.h"

#include /**/ "ace/post.h"
#endif /* ACE_OS_MEMORY_H */
