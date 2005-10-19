/* -*- C++ -*- */
// $Id$
#ifndef OS_NS_STRING_BASE_H
#define OS_NS_STRING_BASE_H

#include "ace/config-lite.h"

#include <string.h>
// Standard library includes for wide functions declared in ace_wchar.h
// Do not include any ACE header files because this file is included from
// ace_wchar.h which is included at the config.h level.

namespace ACE_OS
{

/// Finds the length of a string (emulated version).
template <typename CHAR> inline
size_t strlen (const CHAR* s);

/// Finds the length of a string (emulated version).
template <> inline
size_t strlen (const char* s);

/// Finds the length of a string (wchar_t version).
#if !defined (ACE_LACKS_WCSLEN)
template <> inline
size_t strlen (const wchar_t* s);
#endif /* !ACE_LACKS_WCSLEN */

/// Copies an array (emulated version)
/// This method intentionally specifies
/// a uniform type for source and destination
/// Use ACE_OS::string_copy if the types may
/// not be uniform.
template <typename CHAR> inline
CHAR* strncpy (CHAR* dest, const CHAR* src, size_t len);

/// Copies an array (char version)
template <> inline
char* strncpy (char* dest, const char* src, size_t len);

/// Copies an array (wchar_t version)
#if !defined (ACE_LACKS_WCSNCPY)
template <> inline
wchar_t* strncpy (wchar_t* dest, const wchar_t* src, size_t len);
#endif /* !ACE_LACKS_WCSNCPY */

/// Compares two strings (emulated version).
/// This method intentionally specifies
/// a uniform type for source and destination.
/// The user has to specify the conversion is they differ.
template < typename CHAR > inline
int strcmp (const CHAR *lhs, const CHAR *rhs);

/// Compares two strings (char version).
template <> inline
int strcmp (const char *lhs, const char *rhs);

  /// Compares two strings (wchar_t version).
#if !defined (ACE_LACKS_WCSCMP)
template <> inline
int strcmp (const wchar_t *lhs, const wchar_t *rhs);
#endif /* !ACE_LACKS_WCSCMP */

#if !defined (ACE_LACKS_DEPRECATED_MACROS)
// Emulation
#if defined (ACE_LACKS_WCSLEN)
  /// Emulated wcslen - Returns the length of a string.
  size_t
  wcslen_emulation (const wchar_t* string)
    { return ACE_OS::strlen( string ); }
#endif /* ACE_LACKS_WCSLEN */

#if defined (ACE_LACKS_WCSNCPY)
  /// Emulated wcsncpy - Copies an array.
  wchar_t *
  wcsncpy_emulation (wchar_t *destination,
                     const wchar_t *source,
                     size_t len)
    { return ACE_OS::strncpy( destination, source, len ); }
#endif /* ACE_LACKS_WCSNCPY */

#endif

}

#include "ace/OS_NS_string_base.inl"

#endif
