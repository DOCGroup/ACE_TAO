// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_NS_wchar_t.h
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

#ifndef ACE_OS_NS_wchar_t_H
# define ACE_OS_NS_wchar_t_H

# include /**/ "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/ace_wchar.h"
#include "ace/Basic_Types.h"
#include "ace/ACE_export.h"

#include "ace/os_include/os_errno.h"

#if defined (ACE_EXPORT_MACRO)
#  undef ACE_EXPORT_MACRO
#endif
#define ACE_EXPORT_MACRO ACE_Export

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE_OS
{

  ACE_NAMESPACE_INLINE_FUNCTION
  wint_t fgetwc (FILE* fp);

#if defined (ACE_LACKS_WCSCAT)
  /// Emulated wcscat - Appends a string.
  extern ACE_Export
  wchar_t *wcscat_emulation (wchar_t *destination,
                                    const wchar_t *source);
#endif /* ACE_LACKS_WCSCAT */

#if defined (ACE_LACKS_WCSCHR)
  /// Emulated wcschr - Finds a character in a string.
  extern ACE_Export
  wchar_t *wcschr_emulation (const wchar_t *string, wint_t c);
#endif /* ACE_LACKS_WCSCHR */

#if defined (ACE_LACKS_WCSCMP)
  /// Emulated wcscmp - Compares strings.
  extern ACE_Export
  int wcscmp_emulation (const wchar_t *string1, const wchar_t *string2);
#endif /* ACE_LACKS_WCSCMP */

#if defined (ACE_LACKS_WCSCPY)
  /// Emulated wcscpy - Copies a string.
  extern ACE_Export
  wchar_t *wcscpy_emulation (wchar_t *destination,
                             const wchar_t *source);
#endif /* ACE_LACKS_WCSCPY */

#if defined (ACE_LACKS_WCSCSPN)
  /// Emulated wcscspn.
  extern ACE_Export
  size_t wcscspn_emulation (const wchar_t *string,
                            const wchar_t *reject);
#endif /* ACE_LACKS_WCSCSPN */

#if defined (ACE_LACKS_WCSICMP)
  /// Emulated wcsicmp - Performs a case insensitive comparison of strings.
  extern ACE_Export
  int wcsicmp_emulation (const wchar_t *string1,
                         const wchar_t *string2);
#endif /* ACE_LACKS_WCSICMP */

#if defined (ACE_LACKS_WCSLEN)
  /// Emulated wcslen - Returns the length of a string.
  extern ACE_Export
  size_t wcslen_emulation (const wchar_t *string);
#endif /* ACE_LACKS_WCSLEN */

#if defined (ACE_LACKS_WCSNCAT)
  /// Emulated wcscat - Appends a string.
  extern ACE_Export
  wchar_t *wcsncat_emulation (wchar_t *destination,
                                  const wchar_t *source,
                                  size_t count);
#endif /* ACE_LACKS_WCSCAT */

#if defined (ACE_LACKS_WCSNCMP)
  /// Emulated wcsncmp - Compares two arrays.
  extern ACE_Export
  int wcsncmp_emulation (const wchar_t *string1,
                         const wchar_t *string2,
                         size_t len);
#endif /* ACE_LACKS_WCSNCMP */

#if defined (ACE_LACKS_WCSNCPY)
  /// Emulated wcsncpy - Copies an array.
  extern ACE_Export
  wchar_t *wcsncpy_emulation (wchar_t *destination,
                                  const wchar_t *source,
                                  size_t len);
#endif /* ACE_LACKS_WCSNCPY */

#if defined (ACE_LACKS_WCSNICMP)
  /// Emulated wcsnicmp - Performs a case insensitive comparison of two
  /// arrays
  extern ACE_Export
  int wcsnicmp_emulation (const wchar_t *string1,
                          const wchar_t *string2,
                          size_t len);
#endif /* ACE_LACKS_WCSNICMP */

#if defined (ACE_LACKS_WCSPBRK)
  /// Emulated wcspbrk - Searches for characters in a string.
  extern ACE_Export
  wchar_t *wcspbrk_emulation (const wchar_t *string,
                              const wchar_t *charset);
#endif /* ACE_LACKS_WCSPBRK */

#if defined (ACE_LACKS_WCSRCHR)
  /// Emulated wcsrchr (wchar_t version) - Finds the last occurance of a
  /// character in a string.
  extern ACE_Export
  wchar_t *wcsrchr_emulation (wchar_t *string, wint_t c);

  /// Emulated wcsrchr (const wchar_t version) - Finds the last occurance of a
  /// character in a string.
  extern ACE_Export
  const wchar_t *wcsrchr_emulation (const wchar_t *string, wint_t c);
#endif /* ACE_LACKS_WCSRCHR */

#if defined (ACE_LACKS_WCSSPN)
  /// Emulated wcsspn.
  extern ACE_Export
  size_t wcsspn_emulation (const wchar_t *string,
                           const wchar_t *charset);
#endif /* ACE_LACKS_WCSSPN */

#if defined (ACE_LACKS_WCSSTR)
  /// Emulated wcsstr - Performs a case insensitive comparison of two strings.
  extern ACE_Export
  wchar_t *wcsstr_emulation (const wchar_t *string,
                             const wchar_t *charset);
#endif /* ACE_LACKS_WCSSTR */

  // these are named wrong.  should be wcslen, etc... dhinton
  ACE_NAMESPACE_INLINE_FUNCTION
  u_int wslen (const wchar_t *);

  ACE_NAMESPACE_INLINE_FUNCTION
  wchar_t *wscpy (wchar_t *,
                const wchar_t *);

  ACE_NAMESPACE_INLINE_FUNCTION
  int wscmp (const wchar_t *,
             const wchar_t *);

  ACE_NAMESPACE_INLINE_FUNCTION
  int wsncmp (const wchar_t *,
              const wchar_t *,
              size_t len);

  ACE_NAMESPACE_INLINE_FUNCTION
  wint_t ungetwc (wint_t c, FILE* fp);

} /* namespace ACE_OS */

ACE_END_VERSIONED_NAMESPACE_DECL

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_NS_wchar.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

# include /**/ "ace/post.h"
#endif /* ACE_OS_NS_STDIO_H */
