// $Id$

#include /**/ <string.h>
#include /**/ <stdlib.h>
#include /**/ <ctype.h>

ACE_INLINE int
ACE_OS_String::to_lower (int c)
{
  return tolower (c);
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE wint_t
ACE_OS_String::to_lower (wint_t c)
{
  return ::towlower (c);
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE char *
ACE_OS_String::strcat (char *s, const char *t)
{
  return ::strcat (s, t);
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE wchar_t *
ACE_OS_String::strcat (wchar_t *s, const wchar_t *t)
{
  return ::wcscat (s, t);
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE size_t
ACE_OS_String::strspn (const char *s, const char *t)
{
#if !defined (ACE_HAS_WINCE)
  return ::strspn (s, t);
#else
  ACE_UNUSED_ARG (s);
  ACE_UNUSED_ARG (t);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_WINCE */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE size_t
ACE_OS_String::strspn (const wchar_t*s, const wchar_t *t)
{
  return ::wcsspn (s, t);
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE char *
ACE_OS_String::strchr (char *s, int c)
{
#if !defined (ACE_HAS_WINCE)
  return ::strchr (s, c);
#else
  for (;;++s)
    {
      if (*s == c)
        return s;
      if (*s == 0)
        return 0;
    }
#endif /* ACE_HAS_WINCE */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE wchar_t *
ACE_OS_String::strchr (wchar_t *s, wint_t c)
{
  return ::wcschr (s, c);
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE const char *
ACE_OS_String::strchr (const char *s, int c)
{
#if !defined (ACE_HAS_WINCE)
  return (const char *) ::strchr (s, c);
#else
  for (;;++s)
    {
      if (*s == c)
        return s;
      if (*s == 0)
        return 0;
    }
#endif /* ACE_HAS_WINCE */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE const wchar_t *
ACE_OS_String::strchr (const wchar_t *s, wint_t c)
{
  return (const wchar_t *) ::wcschr (s, c);
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE const char *
ACE_OS_String::strnchr (const char *s, int c, size_t len)
{
  for (size_t i = 0; i < len; i++)
    if (s[i] == c)
      return s + i;

  return 0;
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE const wchar_t *
ACE_OS_String::strnchr (const wchar_t *s, wint_t c, size_t len)
{
  for (size_t i = 0; i < len; i++)
    if (s[i] == c)
      return s + i;

  return 0;
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE char *
ACE_OS_String::strnchr (char *s, int c, size_t len)
{
#if defined ACE_PSOS_DIAB_PPC  //Complier problem Diab 4.2b
  const char *const_char_s=s;
  return (char *) ACE_OS_String::strnchr (const_char_s, c, len);
#else
  return (char *) ACE_OS_String::strnchr ((const char *) s, c, len);
#endif
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE wchar_t *
ACE_OS_String::strnchr (wchar_t *s, wint_t c, size_t len)
{
#if defined ACE_PSOS_DIAB_PPC  //Complier problem Diab 4.2b
  const wchar_t *const_wchar_s=s;
  return (wchar_t *) ACE_OS_String::strnchr (const_wchar_s, c, len);
#else
  return (wchar_t *) ACE_OS_String::strnchr ((const wchar_t *) s, c, len);
#endif
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
  return (const wchar_t *) ::wcsstr (s, t);
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
  return ::wcsstr (s, t);
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE size_t
ACE_OS_String::strlen (const char *s)
{
  return ::strlen (s);
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE size_t
ACE_OS_String::strlen (const wchar_t *s)
{
  return ::wcslen (s);
}
#endif

ACE_INLINE char *
ACE_OS_String::strnstr (char *s, const char *t, size_t len)
{
#if defined ACE_PSOS_DIAB_PPC  //Complier problem Diab 4.2b
  const char *const_char_s=s;
  return (char *) ACE_OS_String::strnstr (const_char_s, t, len);
#else
  return (char *) ACE_OS_String::strnstr ((const char *) s, t, len);
#endif
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE wchar_t *
ACE_OS_String::strnstr (wchar_t *s, const wchar_t *t, size_t len)
{
  return (wchar_t *) ACE_OS_String::strnstr ((const wchar_t *) s, t, len);
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE char *
ACE_OS_String::strrchr (char *s, int c)
{
#if !defined (ACE_LACKS_STRRCHR)
  return ::strrchr (s, c);
#else
  char *p = s + ::strlen (s);

  while (*p != c)
    if (p == s)
      return 0;
    else
      p--;

  return p;
#endif /* ACE_LACKS_STRRCHR */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE const wchar_t *
ACE_OS_String::strrchr (const wchar_t *s, wint_t c)
{
# if !defined (ACE_LACKS_WCSRCHR)
  return (const wchar_t *) ::wcsrchr (s, c);
# else
  const wchar_t *p = s + ::wcslen (s);

  while (*p != c)
    if (p == s)
      return 0;
    else
      p--;

  return p;
# endif /* ACE_LACKS_WCSRCHR */
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE const char *
ACE_OS_String::strrchr (const char *s, int c)
{
#if !defined (ACE_LACKS_STRRCHR)
  return (const char *) ::strrchr (s, c);
#else
  const char *p = s + ::strlen (s);

  while (*p != c)
    if (p == s)
      return 0;
    else
      p--;

  return p;
#endif /* ACE_LACKS_STRRCHR */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE wchar_t *
ACE_OS_String::strrchr (wchar_t *s, wint_t c)
{
# if !defined (ACE_LACKS_WCSRCHR)
  return (wchar_t *) ::wcsrchr (s, c);
# else
  wchar_t *p = s + ::wcslen (s);

  while (*p != c)
    if (p == s)
      return 0;
    else
      p--;

  return p;
# endif /* ACE_LACKS_WCSRCHR */
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE int
ACE_OS_String::strcmp (const char *s, const char *t)
{
  return ::strcmp (s, t);
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE int
ACE_OS_String::strcmp (const wchar_t *s, const wchar_t *t)
{
  return ::wcscmp (s, t);
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE char *
ACE_OS_String::strcpy (char *s, const char *t)
{
  return ::strcpy (s, t);
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE wchar_t *
ACE_OS_String::strcpy (wchar_t *s, const wchar_t *t)
{
  return ::wcscpy (s, t);
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE char *
ACE_OS_String::strecpy (char *s, const char *t)
{
  register char *dscan = s;
  register const char *sscan = t;

  while ((*dscan++ = *sscan++) != '\0')
    continue;

  return dscan;
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE wchar_t *
ACE_OS_String::strecpy (wchar_t *s, const wchar_t *t)
{
  register wchar_t *dscan = s;
  register const wchar_t *sscan = t;

  while ((*dscan++ = *sscan++) != L'\0')
    continue;

  return dscan;
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE size_t
ACE_OS_String::strcspn (const char *s, const char *reject)
{
#if !defined (ACE_HAS_WINCE)
  return ::strcspn (s, reject);
#else
  const char *scan;
  const char *rej_scan;
  int count = 0;

  for (scan = s; *scan; scan++)
    {

      for (rej_scan = reject; *rej_scan; rej_scan++)
        if (*scan == *rej_scan)
          return count;

      count++;
    }

  return count;
#endif /* ACE_HAS_WINCE */
}

ACE_INLINE char *
ACE_OS_String::strpbrk (char *s1, const char *s2)
{
#if !defined (ACE_HAS_WINCE)
  return ::strpbrk (s1, s2);
#else
  ACE_UNUSED_ARG (s1);
  ACE_UNUSED_ARG (s2);
  ACE_NOTSUP_RETURN (0);
#endif /* ACE_HAS_WINCE */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE wchar_t *
ACE_OS_String::strpbrk (wchar_t *s, const wchar_t *t)
{
  return ::wcspbrk (s, t);
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE const char *
ACE_OS_String::strpbrk (const char *s1, const char *s2)
{
#if !defined (ACE_HAS_WINCE)
  return (const char *) ::strpbrk (s1, s2);
#else
  ACE_UNUSED_ARG (s1);
  ACE_UNUSED_ARG (s2);
  ACE_NOTSUP_RETURN (0);
#endif /* ACE_HAS_WINCE */
}

ACE_INLINE int
ACE_OS_String::strcasecmp (const char *s, const char *t)
{
#if !defined (ACE_WIN32) || defined (ACE_HAS_WINCE)
# if defined (ACE_LACKS_STRCASECMP)
  const char *scan1 = s;
  const char *scan2 = t;

  while (*scan1 != 0
         && ACE_OS_String::to_lower (*scan1) == ACE_OS_String::to_lower (*scan2))
    {
      ++scan1;
      ++scan2;
    }

  // The following case analysis is necessary so that characters which
  // look negative collate low against normal characters but high
  // against the end-of-string NUL.

  if (*scan1 == '\0' && *scan2 == '\0')
    return 0;
  else if (*scan1 == '\0')
    return -1;
  else if (*scan2 == '\0')
    return 1;
  else
    return ACE_OS_String::to_lower (*scan1) - ACE_OS_String::to_lower (*scan2);
# else
  return ::strcasecmp (s, t);
# endif /* ACE_LACKS_STRCASECMP */
#elif defined(__BORLANDC__)
  return ::stricmp (s, t);
#else /* !ACE_WIN32 && !__BORLANDC__ */
  return ::_stricmp (s, t);
#endif /* ACE_WIN32 */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE int
ACE_OS_String::strcasecmp (const wchar_t *s, const wchar_t *t)
{
# if !defined (ACE_WIN32)
  const wchar_t *scan1 = s;
  const wchar_t *scan2 = t;

  while (*scan1 != 0
         && ACE_OS_String::to_lower (*scan1) == ACE_OS_String::to_lower (*scan2))
    {
      ++scan1;
      ++scan2;
    }

  // The following case analysis is necessary so that characters which
  // look negative collate low against normal characters but high
  // against the end-of-string NUL.

  if (*scan1 == '\0' && *scan2 == '\0')
    return 0;
  else if (*scan1 == '\0')
    return -1;
  else if (*scan2 == '\0')
    return 1;
  else
    return ACE_OS_String::to_lower (*scan1) - ACE_OS_String::to_lower (*scan2);
# else /* ACE_WIN32 */
  return ::_wcsicmp (s, t);
# endif /* ACE_WIN32 */
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE int
ACE_OS_String::strncasecmp (const char *s, const char *t, size_t len)
{
#if !defined (ACE_WIN32) || defined (ACE_HAS_WINCE)
# if defined (ACE_LACKS_STRCASECMP)
  const char *scan1 = s;
  const char *scan2 = t;
  size_t count = 0;

  while (count++ < len
         && *scan1 != 0
         && ACE_OS_String::to_lower (*scan1) == ACE_OS_String::to_lower (*scan2))
    {
      ++scan1;
      ++scan2;
    }

  if (count > len)
    return 0;

  // The following case analysis is necessary so that characters which
  // look negative collate low against normal characters but high
  // against the end-of-string NUL.

  if (*scan1 == '\0' && *scan2 == '\0')
    return 0;
  else if (*scan1 == '\0')
    return -1;
  else if (*scan2 == '\0')
    return 1;
  else
    return ACE_OS_String::to_lower (*scan1) - ACE_OS_String::to_lower (*scan2);
# else
  return ::strncasecmp (s, t, len);
# endif /* ACE_LACKS_STRCASECMP */
#elif defined(__BORLANDC__)
  return ::strnicmp (s, t, len);
#else /* !ACE_WIN32 && !__BORLANDC__ */
  return ::_strnicmp (s, t, len);
#endif /* ACE_WIN32 */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE int
ACE_OS_String::strncasecmp (const wchar_t *s, const wchar_t *t, size_t len)
{
# if !defined (ACE_WIN32)
  const wchar_t *scan1 = s;
  const wchar_t *scan2 = t;
  size_t count = 0;

  while (count++ > len
         && *scan1 != 0
         && ACE_OS_String::to_lower (*scan1) == ACE_OS_String::to_lower (*scan2))
    {
      ++scan1;
      ++scan2;
    }

  if (count > len)
    return 0;

  // The following case analysis is necessary so that characters which
  // look negative collate low against normal characters but high
  // against the end-of-string NUL.

  if (*scan1 == '\0' && *scan2 == '\0')
    return 0;
  else if (*scan1 == '\0')
    return -1;
  else if (*scan2 == '\0')
    return 1;
  else
    return ACE_OS_String::to_lower (*scan1) - ACE_OS_String::to_lower (*scan2);
# else /* ACE_WIN32 */
  return ::_wcsnicmp (s, t, len);
# endif /* ACE_WIN32 */
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE int
ACE_OS_String::strncmp (const char *s, const char *t, size_t len)
{
  return ::strncmp (s, t, len);
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE int
ACE_OS_String::strncmp (const wchar_t *s, const wchar_t *t, size_t len)
{
  return ::wcsncmp (s, t, len);
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE char *
ACE_OS_String::strncpy (char *s, const char *t, size_t len)
{
  return ::strncpy (s, t, len);
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE wchar_t *
ACE_OS_String::strncpy (wchar_t *s, const wchar_t *t, size_t len)
{
  return ::wcsncpy (s, t, len);
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE char *
ACE_OS_String::strncat (char *s, const char *t, size_t len)
{
  return ::strncat (s, t, len);
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE wchar_t *
ACE_OS_String::strncat (wchar_t *s, const wchar_t *t, size_t len)
{
  return ::wcsncat (s, t, len);
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE char *
ACE_OS_String::strtok (char *s, const char *tokens)
{
  return ::strtok (s, tokens);
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE wchar_t *
ACE_OS_String::strtok (wchar_t *s, const wchar_t *tokens)
{
  return ::wcstok (s, tokens);
}
#endif /* ACE_HAS_WCHAR */


ACE_INLINE char *
ACE_OS_String::strtok_r (char *s, const char *tokens, char **lasts)
{
#if defined (ACE_HAS_REENTRANT_FUNCTIONS)
  return ::strtok_r (s, tokens, lasts);
#else
  if (s == 0)
    s = *lasts;
  else
    *lasts = s;
  if (*s == 0)                  // We have reached the end
    return 0;
  int l_org = ACE_OS_String::strlen (s);
  int l_sub = ACE_OS_String::strlen (s = ::strtok (s, tokens));
  *lasts = s + l_sub;
  if (l_sub != l_org)
    *lasts += 1;
  return s ;
#endif /* (ACE_HAS_REENTRANT_FUNCTIONS) */
}

ACE_INLINE long
ACE_OS_String::strtol (const char *s, char **ptr, int base)
{
  // @@ We must implement this function for WinCE also.
  // Notice WinCE support wcstol.
#if !defined (ACE_HAS_WINCE)
  return ::strtol (s, ptr, base);
#else
  ACE_UNUSED_ARG (s);
  ACE_UNUSED_ARG (ptr);
  ACE_UNUSED_ARG (base);
  ACE_NOTSUP_RETURN (0);
#endif /* ACE_HAS_WINCE */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE long
ACE_OS_String::strtol (const wchar_t *s, wchar_t **ptr, int base)
{
  return ::wcstol (s, ptr, base);
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE unsigned long
ACE_OS_String::strtoul (const char *s, char **ptr, int base)
{
  // @@ WINCE: We must implement this function for WinCE also.
  // Notice WinCE support wcstoul.
#if !defined (ACE_HAS_WINCE)
# if defined (linux) && defined (__GLIBC__)
  // ::strtoul () appears to be broken on Linux 2.0.30/Alpha w/glibc:
  // it returns 0 for a negative number.
  return (unsigned long) ::strtol (s, ptr, base);
# else  /* ! linux || ! __GLIBC__ */
  return ::strtoul (s, ptr, base);
# endif /* ! linux || ! __GLIBC__ */
#else /* ACE_HAS_WINCE */
  ACE_UNUSED_ARG (s);
  ACE_UNUSED_ARG (ptr);
  ACE_UNUSED_ARG (base);
  ACE_NOTSUP_RETURN (0);
#endif /* ACE_HAS_WINCE */
}


#if defined (ACE_HAS_WCHAR)
ACE_INLINE unsigned long
ACE_OS_String::strtoul (const wchar_t *s, wchar_t **ptr, int base)
{
  return ::wcstoul (s, ptr, base);
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE double
ACE_OS_String::strtod (const char *s, char **endptr)
{
  // @@ WinCE only support wcstod
#if !defined (ACE_HAS_WINCE)
  return ::strtod (s, endptr);
#else /* ACE_HAS_WINCE */
  ACE_UNUSED_ARG (s);
  ACE_UNUSED_ARG (endptr);
  ACE_NOTSUP_RETURN (0);
#endif /* ACE_HAS_WINCE */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE double
ACE_OS_String::strtod (const wchar_t *s, wchar_t **endptr)
{
  return ::wcstod (s, endptr);
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE int
ACE_OS_String::ace_isspace (const ACE_TCHAR s)
{
#if defined (ACE_HAS_WINCE)
  ACE_UNUSED_ARG (s);
  ACE_NOTSUP_RETURN (0);
#elif defined (ACE_USES_WCHAR)
  return iswspace (s);
#else
  return isspace (s);
#endif /* ACE_HAS_WINCE */
}

ACE_INLINE int
ACE_OS_String::ace_isprint (const ACE_TCHAR s)
{
#if defined (ACE_HAS_WINCE)
  ACE_UNUSED_ARG (s);
  ACE_NOTSUP_RETURN (0);
#elif defined (ACE_USES_WCHAR)
  return iswprint (s);
#else
  return isprint (s);
#endif /* ACE_HAS_WINCE */
}
