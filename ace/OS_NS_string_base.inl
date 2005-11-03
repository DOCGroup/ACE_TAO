/* -*- C++ -*- */
// $Id$

#ifndef OS_NS_STRING_BASE_INL
#define OS_NS_STRING_BASE_INL

#include <string.h>

namespace ACE_OS
{

template <typename CHAR> inline
size_t strlen (const CHAR* s)
{
  const CHAR* string = s;
  for (; *string; ++string) continue;
  return (string - s);
}

template <> inline
size_t strlen (const char* s)
{
  return ::strlen (s);
}

#if !defined (ACE_LACKS_WCSLEN)
template <> inline
size_t strlen (const wchar_t* s)
{
  return ::wcslen (s);
}
#endif /* !ACE_LACKS_WCSLEN */

template <typename CHAR> inline
CHAR* strncpy (CHAR* dest, const CHAR* src, size_t len)
{
  if (len != 0)
  {
    CHAR* d = dest;
    const CHAR* s = src;
    do
    {
      if ((*d++ = *s++) == 0)
      {
        // NUL pad the remaining n-1 bytes
        while (--len != 0)
          *d++ = 0;
        break;
      }
    } while (--len != 0);
  }
  return dest;
}

template <> inline
char* strncpy (char* dest, const char* src, size_t len)
{
  return ::strncpy (dest, src, len);
}

#if !defined (ACE_LACKS_WCSNCPY)
template <> inline
wchar_t* strncpy (wchar_t* dest, const wchar_t* src, size_t len)
{
  return ::wcsncpy (dest, src, len);
}
#endif /* !ACE_LACKS_WCSNCPY */

template < typename CHAR > inline
int strcmp (const CHAR *lhs, const CHAR *rhs)
{
// we need static casts!
  while (*lhs == *rhs++)
    if (*lhs++ == 0)
      return (0);
  return (*lhs - *--rhs);
}

/// Compares two strings (char version).
template <> inline
int strcmp (const char *lhs, const char *rhs)
{
  return ::strcmp (lhs, rhs);
}

/// Compares two strings (wchar_t version).
#if !defined (ACE_LACKS_WCSCMP)
template <> inline
int strcmp (const wchar_t *lhs, const wchar_t *rhs)
{
  return ::wcscmp (lhs, rhs);
}
#endif /* !ACE_LACKS_WCSCMP */

}

#endif
