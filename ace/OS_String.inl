// $Id$

// Matthew Stevens 7-10-95 Fix GNU GCC 2.7 for memchr() problem.
#if defined (ACE_HAS_GNU_CSTRING_H)
// Define this file to keep /usr/include/memory.h from being included.
# include /**/ <cstring>
#else
# if !defined (ACE_LACKS_MEMORY_H)
#   include /**/ <memory.h>
# endif /* !ACE_LACKS_MEMORY_H */
#endif /* ACE_HAS_GNU_CSTRING_H */

#if !defined (ACE_PSOS_DIAB_MIPS)
# include /**/ <stdlib.h>
# include /**/ <string.h>
#endif /* !ACE_PSOS_DIAB_MIPS */

// We need strings.h on some platforms (qnx-neutrino, for example)
// to get the declaration for strcasecmp
#if defined (ACE_HAS_STRINGS)
# include /**/ <strings.h>
#endif /* ACE_HAS_STRINGS */

#include /**/ <ctype.h>

ACE_INLINE int
ACE_OS_String::memcmp (const void *t, const void *s, size_t len)
{
  return ::memcmp (t, s, len);
}

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
  return ACE_OS::strchr_emulation (s, c);
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
  return ACE_OS_String::strchr_emulation (s, c);
#endif /* ACE_HAS_WINCE */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE const wchar_t *
ACE_OS_String::strchr (const wchar_t *s, wint_t c)
{
  return (const wchar_t *) ::wcschr (s, c);
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE char *
ACE_OS_String::strnchr (char *s, int c, size_t len)
{
#if defined ACE_PSOS_DIAB_PPC  /* Compiler problem Diab 4.2b */
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
#if defined ACE_PSOS_DIAB_PPC  /* Compiler problem Diab 4.2b */
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
#if defined ACE_PSOS_DIAB_PPC  /* Compiler problem Diab 4.2b */
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
  return ACE_OS_String::strrchr_emulation (s, c);
#endif /* ACE_LACKS_STRRCHR */
}

#if defined (ACE_HAS_WCHAR) 
ACE_INLINE const wchar_t *
ACE_OS_String::strrchr (const wchar_t *s, wint_t c)
{
#if !defined (ACE_LACKS_WCSRCHR)
  return (const wchar_t *) ::wcsrchr (s, c);
#else 
  return ACE_OS_String::strrchr (s, c);
#endif /* ACE_LACKS_WCSRCHR */
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE const char *
ACE_OS_String::strrchr (const char *s, int c)
{
#if !defined (ACE_LACKS_STRRCHR)
  return (const char *) ::strrchr (s, c);
#else 
  return ACE_OS_String::strrchr_emulation (s, c);
#endif /* ACE_LACKS_STRRCHR */
}

#if defined (ACE_HAS_WCHAR) 
ACE_INLINE wchar_t *
ACE_OS_String::strrchr (wchar_t *s, wint_t c)
{
#if !defined (ACE_LACKS_WCSRCHR)
  return (wchar_t *) ::wcsrchr (s, c);
#else 
  return ACE_OS_String::strrchr (s, c);
#endif /* ACE_LACKS_WCSRCHR */
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

ACE_INLINE size_t
ACE_OS_String::strcspn (const char *s, const char *reject)
{
#if !defined (ACE_HAS_WINCE)
  return ::strcspn (s, reject);
#else 
  return ACE_OS_String::strcspn_emulation (s, reject);
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
  return ACE_OS_String::strcasecmp_emulation (s, t);
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
#if !defined (ACE_WIN32)
  return ACE_OS_String::strcasecmp_emulation (s, t);
#else /* ACE_WIN32 */
  return ::_wcsicmp (s, t);
#endif /* ACE_WIN32 */ 
}
#endif /* ACE_HAS_WCHAR*/

ACE_INLINE int
ACE_OS_String::strncasecmp (const char *s, const char *t, size_t len)
{
#if !defined (ACE_WIN32) || defined (ACE_HAS_WINCE)
# if defined (ACE_LACKS_STRCASECMP)
  return ACE_OS_String::strncasecmp_emulation (s, t, len);
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
#if !defined (ACE_WIN32)
  return ACE_OS::strncasecmp_emulation (s, t, len);
#else /* ACE_WIN32 */
  return ::_wcsnicmp (s, t, len);
#endif /* ACE_WIN32 */
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
  return ACE_OS_String::strtok_r_emulation (s, tokens, lasts);
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
