// -*- C++ -*-
//=============================================================================
/**
 *  @file   OS_String.inl
 *
 *  $Id$
 *
 *  @brief  Contains inline definitions for class ACE_OS_String.
 */
//=============================================================================

// Matthew Stevens 7-10-95 Fix GNU GCC 2.7 for memchr() problem.
#if defined (ACE_HAS_GNU_CSTRING_H)
// Define this file to keep /usr/include/memory.h from being included.
# include /**/ <cstring>
#else
# if !defined (ACE_LACKS_MEMORY_H)
#   include /**/ <memory.h>
# endif /* !ACE_LACKS_MEMORY_H */
#endif /* ACE_HAS_GNU_CSTRING_H */

#include "ace/os_include/os_stdlib.h"
#include "ace/os_include/os_string.h"
#include "ace/os_include/os_strings.h"
#include "ace/os_include/os_ctype.h"

ACE_INLINE const void *
ACE_OS_String::memchr (const void *s, int c, size_t len)
{
#if defined (ACE_HAS_MEMCHR)
  return ::memchr (s, c, len);
#else /* ACE_HAS_MEMCHR */
  return ACE_OS_String::memchr_emulation (s, c, len);
#endif /* ACE_HAS_MEMCHR */
}

ACE_INLINE void *
ACE_OS_String::memchr (void *s, int c, size_t len)
{
  return ACE_const_cast (void *,
    ACE_OS_String::memchr (ACE_static_cast (const void *, s), c, len));
}


ACE_INLINE int
ACE_OS_String::memcmp (const void *t, const void *s, size_t len)
{
  return ::memcmp (t, s, len);
}


ACE_INLINE void *
ACE_OS_String::memcpy (void *t, const void *s, size_t len)
{
  return ::memcpy (t, s, len);
}

ACE_INLINE void *
ACE_OS_String::memmove (void *t, const void *s, size_t len)
{
  return ::memmove (t, s, len);
}

ACE_INLINE void *
ACE_OS_String::memset (void *s, int c, size_t len)
{
  return ::memset (s, c, len);
}

ACE_INLINE char *
ACE_OS_String::strcat (char *s, const char *t)
{
  return ::strcat (s, t);
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE wchar_t *
ACE_OS_String::strcat (wchar_t *s, const wchar_t *t)
{
#  if defined (ACE_LACKS_WCSCAT)
  return ACE_OS_String::wcscat_emulation (s, t);
#  else /* ACE_LACKS_WCSCAT */
  return ::wcscat (s, t);
#  endif /* ACE_LACKS_WCSCAT */
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE const char *
ACE_OS_String::strchr (const char *s, int c)
{
#if defined (ACE_LACKS_STRCHR)
  return ACE_OS_String::strchr_emulation (s, c);
#else  /* ! ACE_LACKS_STRCHR */
  return (const char *) ::strchr (s, c);
#endif /* ACE_LACKS_STRCHR */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE const wchar_t *
ACE_OS_String::strchr (const wchar_t *s, wint_t c)
{
#  if defined (ACE_LACKS_WCSCHR)
  return ACE_OS_String::wcschr_emulation (s, c);
#  else /* ACE_LACKS_WCSCHR */
  return ::wcschr (s, c);
#  endif /* ACE_LACKS_WCSCHR */
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE char *
ACE_OS_String::strchr (char *s, int c)
{
#if defined (ACE_LACKS_STRCHR)
  return ACE_OS_String::strchr_emulation (s, c);
#else  /* ! ACE_LACKS_STRCHR */
  return ::strchr (s, c);
#endif /* ACE_LACKS_STRCHR */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE wchar_t *
ACE_OS_String::strchr (wchar_t *s, wint_t c)
{
  return ACE_const_cast (wchar_t *,
    ACE_OS_String::strchr (ACE_static_cast (const wchar_t *, s), c));
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE int
ACE_OS_String::strcmp (const char *s, const char *t)
{
  return ::strcmp (s, t);
}

ACE_INLINE int
ACE_OS_String::strcmp (const ACE_WCHAR_T *s, const ACE_WCHAR_T *t)
{
#  if !defined (ACE_HAS_WCHAR) || defined (ACE_LACKS_WCSCMP)
  return ACE_OS_String::wcscmp_emulation (s, t);
#  else /* !ACE_HAS_WCHAR || ACE_LACKS_WCSCMP */
  return ::wcscmp (s, t);
#  endif /* !ACE_HAS_WCHAR || ACE_LACKS_WCSCMP */
}

ACE_INLINE char *
ACE_OS_String::strcpy (char *s, const char *t)
{
  return ::strcpy (s, t);
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE wchar_t *
ACE_OS_String::strcpy (wchar_t *s, const wchar_t *t)
{
#  if defined (ACE_LACKS_WCSCPY)
  return ACE_OS_String::wcscpy_emulation (s, t);
#  else /* ACE_LACKS_WCSCPY */
  return ::wcscpy (s, t);
#  endif /* ACE_LACKS_WCSCPY */
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE size_t
ACE_OS_String::strcspn (const char *s, const char *reject)
{
#if defined (ACE_LACKS_STRCSPN)
  return ACE_OS_String::strcspn_emulation (s, reject);
#else  /* ACE_LACKS_STRCSPN */
  return ::strcspn (s, reject);
#endif /* ACE_LACKS_STRCSPN */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE size_t
ACE_OS_String::strcspn (const wchar_t *s, const wchar_t *reject)
{
#  if defined (ACE_LACKS_WCSCSPN)
  return ACE_OS_String::wcscspn_emulation (s, reject);
#  else /* ACE_LACKS_WCSCSPN */
  return ::wcscspn (s, reject);
#  endif /* ACE_LACKS_WCSCSPN */
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE char *
ACE_OS_String::strerror (int errnum)
{
#if defined (ACE_LACKS_STRERROR)
  return ACE_OS_String::strerror_emulation (errnum);
#else /* ACE_LACKS_STRERROR */
  return ::strerror (errnum);
#endif /* ACE_LACKS_STRERROR */
}

ACE_INLINE size_t
ACE_OS_String::strlen (const char *s)
{
  return ::strlen (s);
}

ACE_INLINE size_t
ACE_OS_String::strlen (const ACE_WCHAR_T *s)
{
# if !defined (ACE_HAS_WCHAR) || defined (ACE_LACKS_WCSLEN)
  return ACE_OS_String::wcslen_emulation (s);
# else  /* !ACE_HAS_WCHAR || ACE_LACKS_WCSLEN */
  return ::wcslen (s);
# endif /* !ACE_HAS_WCHAR || ACE_LACKS_WCSLEN */
}

ACE_INLINE size_t
ACE_OS_String::strnlen (const char *s, size_t maxlen)
{
#if defined (ACE_HAS_STRNLEN)
  return ::strnlen (s, maxlen);
#else /* ACE_HAS_STRNLEN */
  size_t i;
  for (i = 0; i < maxlen; ++i)
    if (s[i] == '\0')
      break;
  return i;
#endif /* ACE_HAS_STRNLEN */
}

ACE_INLINE size_t
ACE_OS_String::strnlen (const ACE_WCHAR_T *s, size_t maxlen)
{
#if defined (ACE_HAS_WCHAR) && defined (ACE_HAS_WCSNLEN)
  return wcsnlen (s, maxlen);
#else /* ACE_HAS_WCSNLEN */
  size_t i;
  for (i = 0; i < maxlen; ++i)
    if (s[i] == '\0')
      break;
  return i;
#endif /* ACE_HAS_WCSNLEN */
}

ACE_INLINE char *
ACE_OS_String::strncat (char *s, const char *t, size_t len)
{
  return ::strncat (s, t, len);
}

ACE_INLINE ACE_WCHAR_T *
ACE_OS_String::strncat (ACE_WCHAR_T *s, const ACE_WCHAR_T *t, size_t len)
{
#  if !defined (ACE_HAS_WCHAR) || defined (ACE_LACKS_WCSNCAT)
  return ACE_OS_String::wcsncat_emulation (s, t, len);
#  else /* !ACE_HAS_WCHAR || ACE_LACKS_WCSNCAT */
  return ::wcsncat (s, t, len);
#  endif /* !ACE_HAS_WCHAR || ACE_LACKS_WCSNCAT */
}

ACE_INLINE int
ACE_OS_String::strncmp (const char *s, const char *t, size_t len)
{
  return ::strncmp (s, t, len);
}

ACE_INLINE int
ACE_OS_String::strncmp (const ACE_WCHAR_T *s, const ACE_WCHAR_T *t, size_t len)
{
#  if !defined (ACE_HAS_WCHAR) || defined (ACE_LACKS_WCSNCMP)
  return ACE_OS_String::wcsncmp_emulation (s, t, len);
#  else /* !ACE_HAS_WCHAR || ACE_LACKS_WCSNCMP */
  return ::wcsncmp (s, t, len);
#  endif /* !ACE_HAS_WCHAR || ACE_LACKS_WCSNCMP */
}

ACE_INLINE char *
ACE_OS_String::strncpy (char *s, const char *t, size_t len)
{
  return ::strncpy (s, t, len);
}

ACE_INLINE ACE_WCHAR_T *
ACE_OS_String::strncpy (ACE_WCHAR_T *s, const ACE_WCHAR_T *t, size_t len)
{
#  if !defined (ACE_HAS_WCHAR) || defined (ACE_LACKS_WCSNCPY)
  return ACE_OS_String::wcsncpy_emulation (s, t, len);
#  else /* !ACE_HAS_WCHAR || ACE_LACKS_WCSNCPY */
  return ::wcsncpy (s, t, len);
#  endif /* !ACE_HAS_WCHAR || ACE_LACKS_WCSNCPY */
}

ACE_INLINE const char *
ACE_OS_String::strpbrk (const char *s1, const char *s2)
{
#if defined (ACE_LACKS_STRPBRK)
  return ACE_OS_String::strpbrk_emulation (s1, s2);
#else  /* ACE_LACKS_STRPBRK */
  return (const char *) ::strpbrk (s1, s2);
#endif /* ACE_LACKS_STRPBRK */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE const wchar_t *
ACE_OS_String::strpbrk (const wchar_t *s, const wchar_t *t)
{
#  if defined (ACE_LACKS_WCSPBRK)
  return ACE_OS_String::wcspbrk_emulation (s, t);
#  else /* ACE_LACKS_WCSPBRK */
  return ::wcspbrk (s, t);
#  endif /* ACE_LACKS_WCSPBRK */
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE char *
ACE_OS_String::strpbrk (char *s1, const char *s2)
{
#if defined (ACE_LACKS_STRPBRK)
  return ACE_OS_String::strpbrk_emulation (s1, s2);
#else /* ACE_LACKS_STRPBRK */
  return ::strpbrk (s1, s2);
#endif /* ACE_LACKS_STRPBRK */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE wchar_t *
ACE_OS_String::strpbrk (wchar_t *s, const wchar_t *t)
{
  return ACE_const_cast (wchar_t *,
    ACE_OS_String::strpbrk (ACE_static_cast (const wchar_t *, s), t));
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE const char *
ACE_OS_String::strrchr (const char *s, int c)
{
#if defined (ACE_LACKS_STRRCHR)
  return ACE_OS_String::strrchr_emulation (s, c);
#else  /* ! ACE_LACKS_STRRCHR */
  return (const char *) ::strrchr (s, c);
#endif /* ! ACE_LACKS_STRRCHR */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE const wchar_t *
ACE_OS_String::strrchr (const wchar_t *s, wint_t c)
{
#if defined (ACE_LACKS_WCSRCHR)
  return ACE_OS_String::wcsrchr_emulation (s, c);
#else /* ! ACE_LACKS_WCSRCHR */
  return (const wchar_t *) ::wcsrchr (s, c);
#endif /* ! ACE_LACKS_WCSRCHR */
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE char *
ACE_OS_String::strrchr (char *s, int c)
{
#if defined (ACE_LACKS_STRRCHR)
  return ACE_OS_String::strrchr_emulation (s, c);
#else  /* ! ACE_LACKS_STRRCHR */
  return ::strrchr (s, c);
#endif /* ! ACE_LACKS_STRRCHR */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE wchar_t *
ACE_OS_String::strrchr (wchar_t *s, wint_t c)
{
  return ACE_const_cast (wchar_t *,
    ACE_OS_String::strrchr (ACE_static_cast (const wchar_t *, s), c));
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE size_t
ACE_OS_String::strspn (const char *s, const char *t)
{
#if defined (ACE_LACKS_STRSPN)
  return ACE_OS_String::strspn_emulation (s, t);
#else /* ACE_LACKS_STRSPN */
  return ::strspn (s, t);
#endif /* ACE_LACKS_STRSPN */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE size_t
ACE_OS_String::strspn (const wchar_t *s, const wchar_t *t)
{
#  if defined (ACE_LACKS_WCSSPN)
  return ACE_OS_String::wcsspn_emulation (s, t);
#  else /* ACE_LACKS_WCSSPN */
  return ::wcsspn (s, t);
#  endif /* ACE_LACKS_WCSSPN */
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE const char *
ACE_OS_String::strstr (const char *s, const char *t)
{
  return (const char *) ::strstr (s, t);
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE const wchar_t *
ACE_OS_String::strstr (const wchar_t *s, const wchar_t *t)
{
#  if defined (ACE_LACKS_WCSSTR)
  return ACE_OS_String::wcsstr_emulation (s, t);
#  elif defined (HPUX)
  return (const wchar_t *) ::wcswcs (s, t);
#  else /* ACE_LACKS_WCSSTR */
  return (const wchar_t *) ::wcsstr (s, t);
#  endif /* ACE_LACKS_WCSSTR */
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE char *
ACE_OS_String::strstr (char *s, const char *t)
{
  return ::strstr (s, t);
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE wchar_t *
ACE_OS_String::strstr (wchar_t *s, const wchar_t *t)
{
#  if defined (ACE_LACKS_WCSSTR)
  return ACE_OS_String::wcsstr_emulation (s, t);
#  elif defined (HPUX)
  return ::wcswcs (s, t);
#  else /* ACE_LACKS_WCSSTR */
  return ::wcsstr (s, t);
#  endif /* ACE_LACKS_WCSSTR */
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE char *
ACE_OS_String::strtok (char *s, const char *tokens)
{
  return ::strtok (s, tokens);
}

#if defined (ACE_HAS_WCHAR) && !defined (ACE_LACKS_WCSTOK)
ACE_INLINE wchar_t *
ACE_OS_String::strtok (wchar_t *s, const wchar_t *tokens)
{
#if defined (ACE_HAS_3_PARAM_WCSTOK)
  static wchar_t *lasts;
  return ::wcstok (s, tokens, &lasts);
#else
  return ::wcstok (s, tokens);
#endif /* ACE_HAS_3_PARAM_WCSTOK */
}
#endif /* ACE_HAS_WCHAR && !ACE_LACKS_WCSTOK */

ACE_INLINE int
ACE_OS_String::ace_isprint (const ACE_TCHAR c)
{
#if defined (ACE_USES_WCHAR)
  return iswprint (c);
#else /* ACE_USES_WCHAR */
  return isprint ((unsigned char) c);
#endif /* ACE_USES_WCHAR */
}

ACE_INLINE int
ACE_OS_String::ace_isspace (const ACE_TCHAR c)
{
#if defined (ACE_USES_WCHAR)
  return iswspace (c);
#else /* ACE_USES_WCHAR */
  return isspace ((unsigned char) c);
#endif /* ACE_USES_WCHAR */
}

ACE_INLINE int
ACE_OS_String::to_lower (int c)
{
  return tolower (c);
}

#if defined (ACE_HAS_WCHAR) && !defined (ACE_LACKS_TOWLOWER)
ACE_INLINE wint_t
ACE_OS_String::to_lower (wint_t c)
{
  return towlower (c);
}
#endif /* ACE_HAS_WCHAR && !ACE_LACKS_TOWLOWER */

ACE_INLINE char *
ACE_OS_String::itoa (int value, char *string, int radix)
{
#if !defined (ACE_HAS_ITOA)
  return ACE_OS_String::itoa_emulation (value, string, radix);
#elif defined (ACE_ITOA_EQUIVALENT)
  return ACE_ITOA_EQUIVALENT (value, string, radix);
#else /* !ACE_HAS_ITOA */
  return ::itoa (value, string, radix);
#endif /* !ACE_HAS_ITOA */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE wchar_t *
ACE_OS_String::itoa (int value, wchar_t *string, int radix)
{
#if defined (ACE_LACKS_ITOW)
  return ACE_OS_String::itow_emulation (value, string, radix);
#else /* ACE_LACKS_ITOW */
  return ::_itow (value, string, radix);
#endif /* ACE_LACKS_ITOW */
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE int
ACE_OS_String::strcasecmp (const char *s, const char *t)
{
#if defined (ACE_LACKS_STRCASECMP)
  return ACE_OS_String::strcasecmp_emulation (s, t);
#elif defined (ACE_STRCASECMP_EQUIVALENT)
  return ACE_STRCASECMP_EQUIVALENT (s, t);
#else /* ACE_LACKS_STRCASECMP */
  return ::strcasecmp (s, t);
#endif /* ACE_LACKS_STRCASECMP */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE int
ACE_OS_String::strcasecmp (const wchar_t *s, const wchar_t *t)
{
#  if defined (ACE_LACKS_WCSICMP)
  return ACE_OS_String::wcsicmp_emulation (s, t);
#  else  /* ACE_LACKS_WCSICMP */
  return ::_wcsicmp (s, t);
#  endif /* ACE_LACKS_WCSICMP */
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE char *
ACE_OS_String::strnchr (char *s, int c, size_t len)
{
#if defined ACE_PSOS_DIAB_PPC  /* Compiler problem Diab 4.2b */
  const char *const_char_s = s;
  return ACE_const_cast (char *,
                         ACE_OS_String::strnchr (const_char_s, c, len));
#else
  return ACE_const_cast (char *,
    ACE_OS_String::strnchr (ACE_static_cast (const char *, s), c, len));
#endif
}

ACE_INLINE ACE_WCHAR_T *
ACE_OS_String::strnchr (ACE_WCHAR_T *s, ACE_WINT_T c, size_t len)
{
  return ACE_const_cast (ACE_WCHAR_T *,
    ACE_OS_String::strnchr (ACE_static_cast (const ACE_WCHAR_T *, s), c, len));
}

ACE_INLINE int
ACE_OS_String::strncasecmp (const char *s, const char *t, size_t len)
{
#if defined (ACE_LACKS_STRCASECMP)
  return ACE_OS_String::strncasecmp_emulation (s, t, len);
#elif defined (ACE_STRNCASECMP_EQUIVALENT)
  return ACE_STRNCASECMP_EQUIVALENT (s, t, len);
#else /* ACE_LACKS_STRCASECMP */
  return ::strncasecmp (s, t, len);
#endif /* ACE_LACKS_STRCASECMP */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE int
ACE_OS_String::strncasecmp (const wchar_t *s, const wchar_t *t, size_t len)
{
#if defined (ACE_LACKS_WCSNICMP)
  return ACE_OS_String::wcsnicmp_emulation (s, t, len);
#else  /* ACE_LACKS_WCSNICMP */
  return ::_wcsnicmp (s, t, len);
#endif /* ACE_LACKS_WCSNICMP */
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE char *
ACE_OS_String::strnstr (char *s, const char *t, size_t len)
{
#if defined ACE_PSOS_DIAB_PPC  /* Compiler problem Diab 4.2b */
  const char *const_char_s=s;
  return (char *) ACE_OS_String::strnstr (const_char_s, t, len);
#else
  return (char *) ACE_OS_String::strnstr ((const char *) s, t, len);
#endif
}

ACE_INLINE ACE_WCHAR_T *
ACE_OS_String::strnstr (ACE_WCHAR_T *s, const ACE_WCHAR_T *t, size_t len)
{
  return ACE_const_cast (ACE_WCHAR_T *,
    ACE_OS_String::strnstr (ACE_static_cast (const ACE_WCHAR_T *, s), t, len));
}

ACE_INLINE char *
ACE_OS_String::strtok_r (char *s, const char *tokens, char **lasts)
{
#if defined (ACE_HAS_REENTRANT_FUNCTIONS)
  return ::strtok_r (s, tokens, lasts);
#else
  return ACE_OS_String::strtok_r_emulation (s, tokens, lasts);
#endif /* (ACE_HAS_REENTRANT_FUNCTIONS) */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE wchar_t*
ACE_OS_String::strtok_r (ACE_WCHAR_T *s, const ACE_WCHAR_T *tokens, ACE_WCHAR_T **lasts)
{
#if defined (ACE_LACKS_WCSTOK)
    return ACE_OS_String::strtok_r_emulation (s, tokens, lasts);
#else
#  if defined (ACE_HAS_3_PARAM_WCSTOK)
    return ::wcstok (s, tokens, lasts);
#  else /* ACE_HAS_3_PARAM_WCSTOK */
    *lasts = ::wcstok (s, tokens);
    return *lasts;
#  endif /* ACE_HAS_3_PARAM_WCSTOK */
#endif  /* ACE_LACKS_WCSTOK */
}
#endif  // ACE_HAS_WCHAR

#if !defined (ACE_LACKS_STRTOD)
ACE_INLINE double
ACE_OS_String::strtod (const char *s, char **endptr)
{
  return ::strtod (s, endptr);
}
#endif /* !ACE_LACKS_STRTOD */

#if defined (ACE_HAS_WCHAR) && !defined (ACE_LACKS_WCSTOD)
ACE_INLINE double
ACE_OS_String::strtod (const wchar_t *s, wchar_t **endptr)
{
  return ::wcstod (s, endptr);
}
#endif /* ACE_HAS_WCHAR && !ACE_LACKS_WCSTOD */

ACE_INLINE long
ACE_OS_String::strtol (const char *s, char **ptr, int base)
{
#if defined (ACE_LACKS_STRTOL)
  return ACE_OS_String::strtol_emulation (s, ptr, base);
#else  /* ACE_LACKS_STRTOL */
  return ::strtol (s, ptr, base);
#endif /* ACE_LACKS_STRTOL */
}

#if defined (ACE_HAS_WCHAR) && !defined (ACE_LACKS_WCSTOL)
ACE_INLINE long
ACE_OS_String::strtol (const wchar_t *s, wchar_t **ptr, int base)
{
  return ::wcstol (s, ptr, base);
}
#endif /* ACE_HAS_WCHAR && !ACE_LACKS_WCSTOL */

ACE_INLINE unsigned long
ACE_OS_String::strtoul (const char *s, char **ptr, int base)
{
#if defined (ACE_LACKS_STRTOUL)
  return ACE_OS_String::strtoul_emulation (s, ptr, base);
#else /* ACE_LACKS_STRTOUL */
  return ::strtoul (s, ptr, base);
#endif /* ACE_LACKS_STRTOUL */
}

#if defined (ACE_HAS_WCHAR) && !defined (ACE_LACKS_WCSTOUL)
ACE_INLINE unsigned long
ACE_OS_String::strtoul (const wchar_t *s, wchar_t **ptr, int base)
{
  return ::wcstoul (s, ptr, base);
}
#endif /* ACE_HAS_WCHAR && !ACE_LACKS_WCSTOUL */
