// -*- C++ -*-
// $Id$

#include "ace/OS_NS_string.h"

ACE_RCSID(ace, OS_NS_string, "$Id$")

#if !defined (ACE_HAS_INLINED_OSCALLS)
# include "ace/OS_NS_string.inl"
#endif /* ACE_HAS_INLINED_OS_CALLS */

#if defined (ACE_HAS_WCHAR)
#  include "ace/OS_NS_stdlib.h"
#endif /* ACE_HAS_WCHAR */

#if defined (ACE_LACKS_SYS_NERR)
#  if defined (__rtems__)
int sys_nerr = EWOULDBLOCK + 1;  // definitely a hack.
#  else
int sys_nerr = ERRMAX + 1;
#  endif /* __rtems__ */
#endif /* ACE_LACKS_SYS_NERR */

#if !defined (ACE_HAS_MEMCHR)
const void *
ACE_OS::memchr_emulation (const void *s, int c, size_t len)
{
  const unsigned char *t = (const unsigned char *) s;
  const unsigned char *e = (const unsigned char *) s + len;

  while (t < e)
    if (((int) *t) == c)
      return t;
    else
      t++;

  return 0;
}
#endif /*ACE_HAS_MEMCHR*/

#if defined (ACE_LACKS_STRCHR)
char *
ACE_OS::strchr_emulation (char *s, int c)
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
ACE_OS::strchr_emulation (const char *s, int c)
{
  for (;;++s)
    {
      if (*s == c)
        return s;
      if (*s == 0)
        return 0;
    }
}
#endif /* ACE_LACKS_STRCHR */

#if defined (ACE_LACKS_STRCSPN)
size_t
ACE_OS::strcspn_emulation (const char *s, const char *reject)
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
#endif /* ACE_LACKS_STRCSPN */

char *
ACE_OS::strdup (const char *s)
{
#if defined (ACE_HAS_STRDUP_EMULATION)
  char *t = (char *) ACE_OS::malloc (ACE_OS::strlen (s) + 1);
  if (t == 0)
    return 0;

  return ACE_OS::strcpy (t, s);
#else
  return ::strdup (s);
#endif /* ACE_HAS_STRDUP_EMULATION */
}

#if defined (ACE_HAS_WCHAR)
wchar_t *
ACE_OS::strdup (const wchar_t *s)
{
#   if defined (ACE_LACKS_WCSDUP)
  wchar_t *buffer =
    (wchar_t *) ACE_OS::malloc ((ACE_OS::strlen (s) + 1)
                                * sizeof (wchar_t));
  if (buffer == 0)
    return 0;

  return ACE_OS::strcpy (buffer, s);
#   elif defined (ACE_WCSDUP_EQUIVALENT)
  return ACE_WCSDUP_EQUIVALENT (s);
#   else /* ACE_LACKS_WCSDUP */
#     if defined (__MINGW32__)
  return ::wcsdup (ACE_const_cast(wchar_t*, s));
#     else /* __MINGW32__ */
  return ::wcsdup (s);
#     endif /* __MINGW32__ */
#   endif /* ACE_LACKS_WCSDUP */
}
#endif /* ACE_HAS_WCHAR */

char *
ACE_OS::strecpy (char *s, const char *t)
{
  register char *dscan = s;
  register const char *sscan = t;

  while ((*dscan++ = *sscan++) != '\0')
    continue;

  return dscan;
}

#if defined (ACE_HAS_WCHAR)
wchar_t *
ACE_OS::strecpy (wchar_t *s, const wchar_t *t)
{
  register wchar_t *dscan = s;
  register const wchar_t *sscan = t;

  while ((*dscan++ = *sscan++) != ACE_TEXT_WIDE ('\0'))
    continue;

  return dscan;
}
#endif /* ACE_HAS_WCHAR */

#if defined (ACE_LACKS_STRERROR)
/**
 * Just returns "Unknown Error" all the time.
 */
char *
ACE_OS::strerror_emulation (int errnum)
{
  return "Unknown Error";
}
#endif /* ACE_LACKS_STRERROR */

const char *
ACE_OS::strnchr (const char *s, int c, size_t len)
{
  for (size_t i = 0; i < len; i++)
    if (s[i] == c)
      return s + i;

  return 0;
}

const ACE_WCHAR_T *
ACE_OS::strnchr (const ACE_WCHAR_T *s, ACE_WINT_T c, size_t len)
{
  for (size_t i = 0; i < len; i++)
    if (s[i] == ACE_static_cast(ACE_WCHAR_T, c))
      return s + i;

  return 0;
}

const char *
ACE_OS::strnstr (const char *s1, const char *s2, size_t len2)
{
  // Substring length
  size_t len1 = ACE_OS::strlen (s1);

  // Check if the substring is longer than the string being searched.
  if (len2 > len1)
    return 0;

  // Go upto <len>
  size_t len = len1 - len2;

  for (size_t i = 0; i <= len; i++)
    {
      if (ACE_OS::memcmp (s1 + i, s2, len2) == 0)
        // Found a match!  Return the index.
        return s1 + i;
    }

  return 0;
}

const ACE_WCHAR_T *
ACE_OS::strnstr (const ACE_WCHAR_T *s1, const ACE_WCHAR_T *s2, size_t len2)
{
  // Substring length
  size_t len1 = ACE_OS::strlen (s1);

  // Check if the substring is longer than the string being searched.
  if (len2 > len1)
    return 0;

  // Go upto <len>
  size_t len = len1 - len2;

  for (size_t i = 0; i <= len; i++)
    {
      if (ACE_OS::memcmp (s1 + i, s2, len2 * sizeof (ACE_WCHAR_T)) == 0)
        // Found a match!  Return the index.
        return s1 + i;
    }

  return 0;
}

#if defined (ACE_LACKS_STRPBRK)
char *
ACE_OS::strpbrk_emulation (const char *string,
                           const char *charset)
{
  const char *scanp;
  int c, sc;

  while ((c = *string++) != 0)
    {
      for (scanp = charset; (sc = *scanp++) != 0;)
        if (sc == c)
          return ACE_const_cast (char *, string - 1);
    }

  return 0;
}
#endif /* ACE_LACKS_STRPBRK */

#if defined (ACE_LACKS_STRRCHR)
char *
ACE_OS::strrchr_emulation (char *s, int c)
{
  char *p = s + ACE_OS::strlen (s);

  while (*p != c)
    if (p == s)
      return 0;
    else
      p--;

  return p;
}

const char *
ACE_OS::strrchr_emulation (const char *s, int c)
{
  const char *p = s + ACE_OS::strlen (s);

  while (*p != c)
    if (p == s)
      return 0;
    else
      p--;

  return p;
}
#endif /* ACE_LACKS_STRRCHR */

char *
ACE_OS::strsncpy (char *dst, const char *src, size_t maxlen)
{
  register char *rdst = dst;
  register const char *rsrc = src;
  register size_t rmaxlen = maxlen;

  if (rmaxlen > 0)
    {
      if (rdst!=rsrc)
        {
          *rdst = '\0';
          if (rsrc != 0)
            strncat (rdst, rsrc, --rmaxlen);
        }
      else
        {
          rdst += (rmaxlen - 1);
          *rdst = '\0';
        }
    }
  return dst;
}

ACE_WCHAR_T *
ACE_OS::strsncpy (ACE_WCHAR_T *dst, const ACE_WCHAR_T *src, size_t maxlen)
{
  register ACE_WCHAR_T *rdst = dst;
  register const ACE_WCHAR_T *rsrc = src;
  register size_t rmaxlen = maxlen;

    if (rmaxlen > 0)
    {
      if (rdst!=rsrc)
        {
          *rdst = ACE_TEXT_WIDE ('\0');
          if (rsrc != 0)
            strncat (rdst, rsrc, --rmaxlen);
        }
      else
        {
          rdst += (rmaxlen - 1);
          *rdst = ACE_TEXT_WIDE ('\0');
        }
    }
  return dst;
}

#if defined (ACE_LACKS_STRSPN)
size_t
ACE_OS::strspn_emulation (const char *string,
                          const char *charset)
{
  const char *p = string;
  const char *spanp;
  wchar_t c, sc;

  // Skip any characters in charset, excluding the terminating \0.
cont:
  c = *p++;
  for (spanp = charset; (sc = *spanp++) != 0;)
    if (sc == c)
      goto cont;
  return (p - 1 - string);
}
#endif /* ACE_LACKS_STRSPN */

#if !defined (ACE_HAS_REENTRANT_FUNCTIONS)
char *
ACE_OS::strtok_r_emulation (char *s, const char *tokens, char **lasts)
{
  if (s == 0)
    s = *lasts;
  else
    *lasts = s;
  if (*s == 0)                  // We have reached the end
    return 0;
  size_t l_org = ACE_OS::strlen (s);
  s = ::strtok (s, tokens);
  if (s == 0)
    return 0;
  size_t l_sub = ACE_OS::strlen (s);
  if (s + l_sub < *lasts + l_org)
    *lasts = s + l_sub + 1;
  else
    *lasts = s + l_sub;
  return s ;
}
#endif /* !ACE_HAS_REENTRANT_FUNCTIONS */

# if defined (ACE_HAS_WCHAR) && defined (ACE_LACKS_WCSTOK)
wchar_t*
ACE_OS::strtok_r_emulation (ACE_WCHAR_T *s,
                            const ACE_WCHAR_T *tokens,
                            ACE_WCHAR_T **lasts)
{
  if (s == 0)
    s = *lasts;
  else
    *lasts = s;
  if (*s == 0)                  // We have reached the end
    return 0;
  int l_org = ACE_OS::strlen (s);
  s = ACE_OS::strtok (s, tokens);
  if (s == 0)
    return 0;
  int l_sub = ACE_OS::strlen (s);
  if (s + l_sub < *lasts + l_org)
    *lasts = s + l_sub + 1;
  else
    *lasts = s + l_sub;
  return s ;
}
# endif  /* ACE_HAS_WCHAR && ACE_LACKS_WCSTOK */

