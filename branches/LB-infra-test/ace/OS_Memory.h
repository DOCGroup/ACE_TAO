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

#include "ace/OS_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/OS_Errno.h"
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
// too, so that you guarantee that strdup() calls your desired mallocator
// and not the system mallocator.
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

#if defined (ACE_HAS_OLD_MALLOC)
typedef char *ACE_MALLOC_T;
#else
typedef void *ACE_MALLOC_T;
#endif /* ACE_HAS_OLD_MALLOC */

// ============================================================================
// ACE_NEW macros
//
// A useful abstraction for expressions involving operator new since
// we can change memory allocation error handling policies (e.g.,
// depending on whether ANSI/ISO exception handling semantics are
// being used).
// ============================================================================

#if defined (ACE_NEW_THROWS_EXCEPTIONS)

// Since new() throws exceptions, we need a way to avoid passing
// exceptions past the call to new because ACE counts on having a 0
// return value for a failed allocation. Some compilers offer the
// new (nothrow) version, which does exactly what we want. Others
// do not. For those that do not, this sets up what exception is thrown,
// and then below we'll do a try/catch around the new to catch it and
// return a 0 pointer instead.

#  if defined (__HP_aCC)
      // I know this works for HP aC++... if <stdexcept> is used, it
      // introduces other stuff that breaks things, like <memory>, which
      // screws up auto_ptr.
#    include /**/ <new>
    // _HP_aCC was first defined at aC++ 03.13 on HP-UX 11. Prior to that
    // (03.10 and before) a failed new threw bad_alloc. After that (03.13
    // and above) the exception thrown is dependent on the below settings.
#    if (HPUX_VERS >= 1100)
#      if ((__HP_aCC < 32500 && !defined (RWSTD_NO_NAMESPACE)) || \
           defined (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB))
#        define ACE_bad_alloc std::bad_alloc
#        define ACE_nothrow   std::nothrow
#        define ACE_nothrow_t std::nothrow_t
#      else
#        define ACE_bad_alloc bad_alloc
#        define ACE_nothrow   nothrow
#        define ACE_nothrow_t nothrow_t
#      endif /* __HP_aCC */
#    elif ((__HP_aCC <  12500 && !defined (RWSTD_NO_NAMESPACE)) || \
           defined (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB))
#      define ACE_bad_alloc std::bad_alloc
#      define ACE_nothrow   std::nothrow
#      define ACE_nothrow_t std::nothrow_t
#    else
#      define ACE_bad_alloc bad_alloc
#      define ACE_nothrow   nothrow
#      define ACE_nothrow_t nothrow_t
#    endif /* HPUX_VERS < 1100 */
#    define ACE_throw_bad_alloc throw ACE_bad_alloc ()
#  elif defined (__SUNPRO_CC)
#      if (__SUNPRO_CC < 0x500) || (__SUNPRO_CC_COMPAT == 4)
#        include /**/ <exception.h>
         // Note: we catch ::xalloc rather than just xalloc because of
         // a name clash with unsafe_ios::xalloc()
#        define ACE_bad_alloc ::xalloc
#        define ACE_throw_bad_alloc throw ACE_bad_alloc ("no more memory")
#      else
#        include /**/ <new>
#        define ACE_bad_alloc std::bad_alloc
#        define ACE_throw_bad_alloc throw ACE_bad_alloc ()
#      endif /* __SUNPRO_CC < 0x500 */
#  elif defined (__BORLANDC__) || defined (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB)
#    include /**/ <new>
#    define ACE_bad_alloc std::bad_alloc
#    define ACE_throw_bad_alloc throw ACE_bad_alloc ()
#  else
#    include /**/ <new>
#    define ACE_bad_alloc bad_alloc
#    define ACE_throw_bad_alloc throw ACE_bad_alloc ()
#  endif /* __HP_aCC */

#  if defined (ACE_HAS_NEW_NOTHROW)
#    define ACE_NEW_RETURN(POINTER,CONSTRUCTOR,RET_VAL) \
   do { POINTER = new (ACE_nothrow) CONSTRUCTOR; \
     if (POINTER == 0) { errno = ENOMEM; return RET_VAL; } \
   } while (0)
#    define ACE_NEW(POINTER,CONSTRUCTOR) \
   do { POINTER = new(ACE_nothrow) CONSTRUCTOR; \
     if (POINTER == 0) { errno = ENOMEM; return; } \
   } while (0)
#    define ACE_NEW_NORETURN(POINTER,CONSTRUCTOR) \
   do { POINTER = new(ACE_nothrow) CONSTRUCTOR; \
     if (POINTER == 0) { errno = ENOMEM; } \
   } while (0)

#  else

#    define ACE_NEW_RETURN(POINTER,CONSTRUCTOR,RET_VAL) \
   do { try { POINTER = new CONSTRUCTOR; } \
        catch (ACE_bad_alloc) { errno = ENOMEM; POINTER = 0; return RET_VAL; } \
   } while (0)

#    define ACE_NEW(POINTER,CONSTRUCTOR) \
   do { try { POINTER = new CONSTRUCTOR; } \
        catch (ACE_bad_alloc) { errno = ENOMEM; POINTER = 0; return; } \
   } while (0)

#    define ACE_NEW_NORETURN(POINTER,CONSTRUCTOR) \
   do { try { POINTER = new CONSTRUCTOR; } \
        catch (ACE_bad_alloc) { errno = ENOMEM; POINTER = 0; } \
   } while (0)
#  endif /* ACE_HAS_NEW_NOTHROW */

#else /* ACE_NEW_THROWS_EXCEPTIONS */

# define ACE_NEW_RETURN(POINTER,CONSTRUCTOR,RET_VAL) \
   do { POINTER = new CONSTRUCTOR; \
     if (POINTER == 0) { errno = ENOMEM; return RET_VAL; } \
   } while (0)
# define ACE_NEW(POINTER,CONSTRUCTOR) \
   do { POINTER = new CONSTRUCTOR; \
     if (POINTER == 0) { errno = ENOMEM; return; } \
   } while (0)
# define ACE_NEW_NORETURN(POINTER,CONSTRUCTOR) \
   do { POINTER = new CONSTRUCTOR; \
     if (POINTER == 0) { errno = ENOMEM; } \
   } while (0)

# define ACE_throw_bad_alloc \
  void* gcc_will_complain_if_literal_0_is_returned = 0; \
  return gcc_will_complain_if_literal_0_is_returned

#endif /* ACE_NEW_THROWS_EXCEPTIONS */

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
 * Notice that there is a multiple of <alignment> in the range
 * [<value>,<value> + T1], also notice that if
 *
 * X = ( <value> + T1 ) & T2
 *
 * then
 *
 * <value> <= X <= <value> + T1
 *
 * because the & operator only changes the last bits, and since X is a
 * multiple of <alignment> (its last bits are zero) we have found the
 * multiple we wanted.
 */
/// Return the next integer aligned to a required boundary
/**
 * @param ptr the base pointer
 * @param alignment the required alignment
 */
#define ACE_align_binary(ptr, alignment) \
    ((ptr + ((ptrdiff_t)((alignment)-1))) & (~((ptrdiff_t)((alignment)-1))))

/// Return the next address aligned to a required boundary
#define ACE_ptr_align_binary(ptr, alignment) \
        ((char *) ACE_align_binary (((ptrdiff_t) (ptr)), (alignment)))
//@}

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
