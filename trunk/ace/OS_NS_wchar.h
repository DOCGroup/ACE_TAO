// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_NS_wchar.h
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

#ifndef ACE_OS_NS_WCHAR_H
# define ACE_OS_NS_WCHAR_H

# include /**/ "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/os_wchar.h"
#include "ace/Basic_Types.h"

namespace ACE_OS {

//#if defined (ACE_HAS_WCHAR)
  typedef ACE_WCHAR_T WChar;

  wint_t fgetwc (FILE* fp);

#if defined (ACE_HAS_WCHAR) && defined (ACE_LACKS_WCSCAT)
  /// Emulated wcscat - Appends a string.
  wchar_t *wcscat_emulation (wchar_t *destination,
                                    const wchar_t *source);
#endif /* ACE_HAS_WCHAR && ACE_LACKS_WCSCAT */

#if defined (ACE_HAS_WCHAR) && defined (ACE_LACKS_WCSCHR)
  /// Emulated wcschr - Finds a character in a string.
  wchar_t *wcschr_emulation (const wchar_t *string, wint_t c);
#endif /* ACE_HAS_WCHAR && ACE_LACKS_WCSCHR */

#if !defined (ACE_HAS_WCHAR) || defined (ACE_LACKS_WCSCMP)
  /// Emulated wcscmp - Compares strings.
  int wcscmp_emulation (const ACE_WCHAR_T *string1, const ACE_WCHAR_T *string2);
#endif /* !ACE_HAS_WCHAR || ACE_LACKS_WCSCMP */

#if defined (ACE_HAS_WCHAR) && defined (ACE_LACKS_WCSCPY)
  /// Emulated wcscpy - Copies a string.
  wchar_t *wcscpy_emulation (wchar_t *destination,
                                    const wchar_t *source);
#endif /* ACE_HAS_WCHAR && ACE_LACKS_WCSCPY */

#if defined (ACE_HAS_WCHAR) && defined (ACE_LACKS_WCSCSPN)
  /// Emulated wcscspn.
  size_t wcscspn_emulation (const wchar_t *string,
                                   const wchar_t *reject);
#endif /* ACE_HAS_WCHAR && ACE_LACKS_WCSCSPN */

#if defined (ACE_HAS_WCHAR) && defined (ACE_LACKS_WCSICMP)
  /// Emulated wcsicmp - Performs a case insensitive comparison of strings.
  int wcsicmp_emulation (const wchar_t *string1,
                                const wchar_t *string2);
#endif /* ACE_HAS_WCHAR && ACE_LACKS_WCSICMP */

#if !defined (ACE_HAS_WCHAR) || defined (ACE_LACKS_WCSLEN)
  /// Emulated wcslen - Returns the length of a string.
  size_t wcslen_emulation (const ACE_WCHAR_T *string);
#endif /* !ACE_HAS_WCHAR || ACE_LACKS_WCSLEN */

#if !defined (ACE_HAS_WCHAR) || defined (ACE_LACKS_WCSNCAT)
  /// Emulated wcscat - Appends a string.
  ACE_WCHAR_T *wcsncat_emulation (ACE_WCHAR_T *destination,
                                         const ACE_WCHAR_T *source,
                                         size_t count);
#endif /* !ACE_HAS_WCHAR || ACE_LACKS_WCSCAT */

#if !defined (ACE_HAS_WCHAR) || defined (ACE_LACKS_WCSNCMP)
  /// Emulated wcsncmp - Compares two arrays.
  int wcsncmp_emulation (const ACE_WCHAR_T *string1,
                                const ACE_WCHAR_T *string2,
                                size_t len);
#endif /* !ACE_HAS_WCHAR || ACE_LACKS_WCSNCMP */

#if !defined (ACE_HAS_WCHAR) || defined (ACE_LACKS_WCSNCPY)
  /// Emulated wcsncpy - Copies an array.
  ACE_WCHAR_T *wcsncpy_emulation (ACE_WCHAR_T *destination,
                                         const ACE_WCHAR_T *source,
                                         size_t len);
#endif /* !ACE_HAS_WCHAR || ACE_LACKS_WCSNCPY */

#if defined (ACE_HAS_WCHAR) && defined (ACE_LACKS_WCSNICMP)
  /// Emulated wcsnicmp - Performs a case insensitive comparison of two
  /// arrays
  int wcsnicmp_emulation (const wchar_t *string1,
                                 const wchar_t *string2,
                                 size_t len);
#endif /* ACE_HAS_WCHAR && ACE_LACKS_WCSNICMP */

#if defined (ACE_HAS_WCHAR) && defined (ACE_LACKS_WCSPBRK)
  /// Emulated wcspbrk - Searches for characters in a string.
  wchar_t *wcspbrk_emulation (const wchar_t *string,
                                     const wchar_t *charset);
#endif /* ACE_HAS_WCHAR && ACE_LACKS_WCSPBRK */

#if defined (ACE_HAS_WCHAR) && defined (ACE_LACKS_WCSRCHR)
  /// Emulated wcsrchr (wchar_t version) - Finds the last occurance of a
  /// character in a string.
  wchar_t *wcsrchr_emulation (wchar_t *string, wint_t c);

  /// Emulated wcsrchr (const wchar_t version) - Finds the last occurance of a
  /// character in a string.
  const wchar_t *wcsrchr_emulation (const wchar_t *string, wint_t c);
#endif /* ACE_HAS_WCHAR && ACE_LACKS_WCSRCHR */

#if defined (ACE_HAS_WCHAR) && defined (ACE_LACKS_WCSSPN)
  /// Emulated wcsspn.
  size_t wcsspn_emulation (const wchar_t *string,
                                  const wchar_t *charset);
#endif /* ACE_HAS_WCHAR && ACE_LACKS_WCSSPN */

#if defined (ACE_HAS_WCHAR) && defined (ACE_LACKS_WCSSTR)
  /// Emulated wcsstr - Performs a case insensitive comparison of two strings.
  wchar_t *wcsstr_emulation (const wchar_t *string,
                                    const wchar_t *charset);
#endif /* ACE_HAS_WCHAR && ACE_LACKS_WCSSTR */

  // these are named wrong.  should be wcslen, etc... dhinton
  u_int wslen (const WChar *);

  WChar *wscpy (WChar *,
                const WChar *);

  int wscmp (const WChar *,
             const WChar *);

  int wsncmp (const WChar *,
              const WChar *,
              size_t len);

  wint_t ungetwc (wint_t c, FILE* fp);

} /* namespace ACE_OS */

//#endif /* ACE_HAS_WCHAR */

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_NS_wchar.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

# include /**/ "ace/post.h"
#endif /* ACE_OS_NS_STDIO_H */
