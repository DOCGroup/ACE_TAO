// $Id$

#include "ace/OS_String.h"
#include "ace/OS_Memory.h"

ACE_RCSID(ace, OS_String, "$Id$")

#if !defined (ACE_HAS_INLINED_OSCALLS)
# include "ace/OS_String.inl"
#endif /* ACE_HAS_INLINED_OS_CALLS */

const char *
ACE_OS_String::strnstr (const char *s1, const char *s2, size_t len2)
{
  // Substring length
  size_t len1 = ACE_OS_String::strlen (s1);

  // Check if the substring is longer than the string being searched.
  if (len2 > len1)
    return 0;

  // Go upto <len>
  size_t len = len1 - len2;

  for (size_t i = 0; i <= len; i++)
    {
      if (ACE_OS_String::memcmp (s1 + i, s2, len2) == 0)
        // Found a match!  Return the index.
        return s1 + i;
    }

  return 0;
}

#if defined (ACE_HAS_WCHAR)
const wchar_t *
ACE_OS_String::strnstr (const wchar_t *s1, const wchar_t *s2, size_t len2)
{
  // Substring length
  size_t len1 = ACE_OS_String::strlen (s1);

  // Check if the substring is longer than the string being searched.
  if (len2 > len1)
    return 0;

  // Go upto <len>
  size_t len = len1 - len2;

  for (size_t i = 0; i <= len; i++)
    {
      if (ACE_OS_String::memcmp (s1 + i, s2, len2 * sizeof (wchar_t)) == 0)
        // Found a match!  Return the index.
        return s1 + i;
    }

  return 0;
}
#endif /* ACE_HAS_WCHAR */

char *
ACE_OS_String::strdup (const char *s)
{
  // @@ WINCE Should we provide this function on WinCE?
#if defined (ACE_HAS_STRDUP_EMULATION)
  char *t = (char *) ACE_OS_Memory::malloc (ACE_OS_String::strlen (s) + 1);
  if (t == 0)
    return 0;
  else
    return ACE_OS_String::strcpy (t, s);
#else
  return ::strdup (s);
#endif /* ACE_HAS_STRDUP_EMULATION */
}

#if defined (ACE_HAS_WCHAR)
wchar_t *
ACE_OS_String::strdup (const wchar_t *s)
{
#   if defined (__BORLANDC__)
  wchar_t *buffer =
    (wchar_t *) ACE_OS_Memory::malloc ((ACE_OS_String::strlen (s) + 1)
                                       * sizeof (wchar_t));
  return ::wcscpy (buffer, s);
#   else
  return ::wcsdup (s);
#   endif /* __BORLANDC__ */
}
#endif /* ACE_HAS_WCHAR */

char *
ACE_OS_String::strchr_emulation (char *s, int c)
{
  for (;;++s)
    {
      if (*s == c)
        return s;
      if (*s == 0)
        return 0;
    }
}

const char *
ACE_OS_String::strchr_emulation (const char *s, int c)
{
  for (;;++s)
    {
      if (*s == c)
        return s;
      if (*s == 0)
        return 0;
    }
}

const char *
ACE_OS_String::strnchr (const char *s, int c, size_t len)
{
  for (size_t i = 0; i < len; i++)
    if (s[i] == c)
      return s + i;

  return 0;
}

#if defined (ACE_HAS_WCHAR)
const wchar_t *
ACE_OS_String::strnchr (const wchar_t *s, wint_t c, size_t len)
{
  for (size_t i = 0; i < len; i++)
    if (s[i] == c)
      return s + i;

  return 0;
}
#endif /* ACE_HAS_WCHAR */

char *
ACE_OS_String::strrchr_emulation (char *s, int c)
{
  char *p = s + ACE_OS_String::strlen (s);

  while (*p != c)
    if (p == s)
      return 0;
    else
      p--;

  return p;
}

const char *
ACE_OS_String::strrchr_emulation (const char *s, int c)
{
  const char *p = s + ACE_OS_String::strlen (s);

  while (*p != c)
    if (p == s)
      return 0;
    else
      p--;

  return p;
}

#if defined (ACE_HAS_WCHAR)
const wchar_t *
ACE_OS_String::strrchr_emulation (const wchar_t *s, wint_t c)
{
  const wchar_t *p = s + ACE_OS_String::strlen (s);

  while (*p != c)
    if (p == s)
      return 0;
    else
      p--;

  return p;
}

wchar_t *
ACE_OS_String::strrchr_emulation (wchar_t *s, wint_t c)
{
  wchar_t *p = s + ACE_OS_String::strlen (s);

  while (*p != c)
    if (p == s)
      return 0;
    else
      p--;

  return p;
}
#endif /* ACE_HAS_WCHAR */

char *
ACE_OS_String::strecpy (char *s, const char *t)
{
  register char *dscan = s;
  register const char *sscan = t;

  while ((*dscan++ = *sscan++) != '\0')
    continue;

  return dscan;
}

#if defined (ACE_HAS_WCHAR)
wchar_t *
ACE_OS_String::strecpy (wchar_t *s, const wchar_t *t)
{
  register wchar_t *dscan = s;
  register const wchar_t *sscan = t;

  while ((*dscan++ = *sscan++) != L'\0')
    continue;

  return dscan;
}
#endif /* ACE_HAS_WCHAR */

#if defined (ACE_HAS_WINCE)
// ACE_HAS_WINCE version in .cpp file
size_t
ACE_OS_String::strcspn (const char *s, const char *reject)
{
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
}
#endif /* ACE_HAS_WINCE */

int
ACE_OS_String::strcasecmp_emulation (const char *s, const char *t)
{
  const char *scan1 = s;
  const char *scan2 = t;

  while (*scan1 != 0
         && ACE_OS_String::to_lower (*scan1) 
            == ACE_OS_String::to_lower (*scan2))
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
}

#if defined (ACE_HAS_WCHAR)
// ACE_WIN32 version in .cpp file
int
ACE_OS_String::strcasecmp_emulation (const wchar_t *s, const wchar_t *t)
{
  const wchar_t *scan1 = s;
  const wchar_t *scan2 = t;

  while (*scan1 != 0
         && ACE_OS_String::to_lower (*scan1) 
            == ACE_OS_String::to_lower (*scan2))
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
}
#endif /* ACE_HAS_WCHAR */

char *
ACE_OS_String::strtok_r_emulation (char *s, const char *tokens, char **lasts)
{
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
}

const void *
ACE_OS_String::memchr_emulation (const void *s, int c, size_t len)
{
  const u_char *t = (const u_char *) s;
  const u_char *e = (const u_char *) s + len;

  while (t < e)
    if (((int) *t) == c)
      return t;
    else
      t++;

  return 0;
}

