// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_NS_stdlib.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 *
 *  Originally in OS.h.
 */
//=============================================================================

#ifndef ACE_OS_NS_STDLIB_H
# define ACE_OS_NS_STDLIB_H

# include /**/ "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/os_stdlib.h"

# if defined (ACE_HAS_BROKEN_R_ROUTINES)
#   undef rand_r
# endif /* ACE_HAS_BROKEN_R_ROUTINES */

// We need this for MVS... as well as Linux, etc...
extern "C" {
  typedef int (*ACE_COMPARE_FUNC)(const void *, const void *);
}

namespace ACE_OS {

  /** @name Non-standard functions
   *
   *  These functions aren't in the standard.
   *
   */
  //@{


  void _exit (int status = 0);

  void abort (void);

  int atexit (ACE_EXIT_HOOK func);

  int atoi (const char *s);

# if defined (ACE_HAS_WCHAR)
  int atoi (const wchar_t *s);
# endif /* ACE_HAS_WCHAR */

  // atop not in spec
# if defined (atop)
#   undef atop
# endif /* atop */

  void *atop (const char *s);

# if defined (ACE_HAS_WCHAR)
  void *atop (const wchar_t *s);
# endif /* ACE_HAS_WCHAR */

  void *bsearch (const void *key,
                 const void *base,
                 size_t nel,
                 size_t size,
                 ACE_COMPARE_FUNC);

  void *calloc (size_t elements, size_t sizeof_elements);

  void exit (int status = 0);

  void free (void *);

  char *getenv (const char *symbol);

#   if defined (ACE_HAS_WCHAR) && defined (ACE_WIN32)
  wchar_t *getenv (const wchar_t *symbol);
#   endif /* ACE_HAS_WCHAR && ACE_WIN32 */

  // not in spec
  ACE_TCHAR *getenvstrings (void);

  // itoa not in spec
  /// Converts an integer to a string.
  char *itoa (int value, char *string, int radix);

#if defined (ACE_HAS_WCHAR)
  /// Converts an integer to a string.
  wchar_t *itoa (int value, wchar_t *string, int radix);
#endif /* ACE_HAS_WCHAR */

#if !defined (ACE_HAS_ITOA)
  /// Emulated itoa - Converts an integer to a string.
  char *itoa_emulation (int value, char *string, int radix);
#endif /* !ACE_HAS_ITOA */

#if defined (ACE_HAS_WCHAR) && defined (ACE_LACKS_ITOW)
  /// Emulated itow - Converts an integer to a string.
  wchar_t *itow_emulation (int value, wchar_t *string, int radix);
#endif /* ACE_HAS_WCHAR && ACE_LACKS_ITOW */

  void *malloc (size_t);

  ACE_HANDLE mkstemp (ACE_TCHAR *t);

  ACE_TCHAR *mktemp (ACE_TCHAR *t);

  int putenv (const ACE_TCHAR *string);

  void qsort (void *base,
              size_t nel,
              size_t width,
              ACE_COMPARE_FUNC);

  int rand (void);

  int rand_r (ACE_RANDR_TYPE &seed);

  void *realloc (void *, size_t);

  // exit_hook and set_exit_hook not in spec
  /// Function that is called by <ACE_OS::exit>, if non-null.
  extern ACE_EXIT_HOOK exit_hook_;

  /// For use by ACE_Object_Manager only, to register its exit hook..
  ACE_EXIT_HOOK set_exit_hook (ACE_EXIT_HOOK hook);

  void srand (u_int seed);

  // not in spec
  ACE_TCHAR *strenvdup (const ACE_TCHAR *str);

#if !defined (ACE_LACKS_STRTOD)
  /// Converts a string to a double value (char version).
  double strtod (const char *s, char **endptr);
#endif /* !ACE_LACKS_STRTOD */

#if defined (ACE_HAS_WCHAR) && !defined (ACE_LACKS_WCSTOD)
  /// Converts a string to a double value (wchar_t version).
  double strtod (const wchar_t *s, wchar_t **endptr);
#endif /* ACE_HAS_WCHAR && !ACE_LACKS_WCSTOD */

  /// Converts a string to a long value (char version).
  long strtol (const char *s, char **ptr, int base);

#if defined (ACE_HAS_WCHAR) && !defined (ACE_LACKS_WCSTOL)
  /// Converts a string to a long value (wchar_t version).
  long strtol (const wchar_t *s, wchar_t **ptr, int base);
#endif /* ACE_HAS_WCHAR && !ACE_LACKS_WCSTOL */

#if defined (ACE_LACKS_STRTOL)
  long strtol_emulation (const char *nptr, char **endptr, int base);
#endif /* ACE_LACKS_STRTOL */

  /// Converts a string to an unsigned long value (char version).
  unsigned long strtoul (const char *s, char **ptr, int base);

#if defined (ACE_HAS_WCHAR) && !defined (ACE_LACKS_WCSTOUL)
  /// Converts a string to an unsigned long value (wchar_t version).
  unsigned long strtoul (const wchar_t *s, wchar_t **ptr, int base);
#endif /* ACE_HAS_WCHAR && !ACE_LACKS_WCSTOUL */

#if defined (ACE_LACKS_STRTOUL)
  unsigned long strtoul_emulation (const char *nptr,
                                   char **endptr,
                                   int base);
#endif /* ACE_LACKS_STRTOUL */

  int system (const ACE_TCHAR *s);


} /* namespace ACE_OS */

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_NS_stdlib.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

# include /**/ "ace/post.h"
#endif /* ACE_OS_NS_STDLIB_H */
