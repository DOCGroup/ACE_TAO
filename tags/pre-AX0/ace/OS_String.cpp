//=============================================================================
/**
 *  @file   OS_String.cpp
 *
 *  $Id$
 *
 *  @brief  Contains definitions for class ACE_OS_String.
 */
//=============================================================================

#include "ace/OS_String.h"
#include "ace/OS_Memory.h"

ACE_RCSID (ace, OS_String, "$Id$")

#if !defined (ACE_HAS_INLINED_OSCALLS)
# include "ace/OS_String.inl"
#endif /* ACE_HAS_INLINED_OS_CALLS */

#if defined (ACE_LACKS_WCSDUP_PROTOTYPE)
extern "C" wchar_t *wcsdup __P ((__const wchar_t *__s));
#endif /* ACE_LACKS_WCSDUP_PROTOTYPE */

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

const ACE_WCHAR_T *
ACE_OS_String::strnstr (const ACE_WCHAR_T *s1, const ACE_WCHAR_T *s2, size_t len2)
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
      if (ACE_OS_String::memcmp (s1 + i, s2, len2 * sizeof (ACE_WCHAR_T)) == 0)
        // Found a match!  Return the index.
        return s1 + i;
    }

  return 0;
}

char *
ACE_OS_String::strdup (const char *s)
{
#if defined (ACE_HAS_STRDUP_EMULATION)
  char *t = (char *) ACE_OS_Memory::malloc (ACE_OS_String::strlen (s) + 1);
  if (t == 0)
    return 0;

  return ACE_OS_String::strcpy (t, s);
#else
  return ::strdup (s);
#endif /* ACE_HAS_STRDUP_EMULATION */
}

#if defined (ACE_HAS_WCHAR)
wchar_t *
ACE_OS_String::strdup (const wchar_t *s)
{
#   if defined (ACE_LACKS_WCSDUP)
  wchar_t *buffer =
    (wchar_t *) ACE_OS_Memory::malloc ((ACE_OS_String::strlen (s) + 1)
                                       * sizeof (wchar_t));
  if (buffer == 0)
    return 0;

  return ACE_OS_String::strcpy (buffer, s);
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

#if defined (ACE_LACKS_STRERROR)
/**
 * Just returns "Unknown Error" all the time.
 */
char *
ACE_OS_String::strerror_emulation (int errnum)
{
  return "Unknown Error";
}
#endif /* ACE_LACKS_STRERROR */

#if defined (ACE_LACKS_STRCHR)
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
#endif /* ACE_LACKS_STRCHR */

const char *
ACE_OS_String::strnchr (const char *s, int c, size_t len)
{
  for (size_t i = 0; i < len; i++)
    if (s[i] == c)
      return s + i;

  return 0;
}

const ACE_WCHAR_T *
ACE_OS_String::strnchr (const ACE_WCHAR_T *s, ACE_WINT_T c, size_t len)
{
  for (size_t i = 0; i < len; i++)
    if (s[i] == ACE_static_cast(ACE_WCHAR_T, c))
      return s + i;

  return 0;
}

#if defined (ACE_LACKS_STRRCHR)
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
#endif /* ACE_LACKS_STRRCHR */

#if defined (ACE_HAS_WCHAR) && defined (ACE_LACKS_WCSRCHR)
const wchar_t *
ACE_OS_String::wcsrchr_emulation (const wchar_t *s, wint_t c)
{
  const wchar_t *p = s + ACE_OS_String::strlen (s);

  while (*p != ACE_static_cast (wchar_t, c))
    if (p == s)
      return 0;
    else
      p--;

  return p;
}

wchar_t *
ACE_OS_String::wcsrchr_emulation (wchar_t *s, wint_t c)
{
  wchar_t *p = s + ACE_OS_String::strlen (s);

  while (*p != ACE_static_cast(wchar_t, c))
    if (p == s)
      return 0;
    else
      p--;

  return p;
}
#endif /* ACE_HAS_WCHAR && ACE_LACKS_WCSRCHR */

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

  while ((*dscan++ = *sscan++) != ACE_TEXT_WIDE ('\0'))
    continue;

  return dscan;
}
#endif /* ACE_HAS_WCHAR */

#if defined (ACE_LACKS_STRCSPN)
size_t
ACE_OS_String::strcspn_emulation (const char *s, const char *reject)
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

#if defined (ACE_LACKS_STRCASECMP)
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
#endif /* ACE_LACKS_STRCASECMP */

#if defined (ACE_HAS_WCHAR) && defined (ACE_LACKS_WCSICMP)
int
ACE_OS_String::wcsicmp_emulation (const wchar_t *s, const wchar_t *t)
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
#endif /* ACE_HAS_WCHAR && ACE_LACKS_WCSICMP */

#if defined (ACE_LACKS_STRCASECMP)
int
ACE_OS_String::strncasecmp_emulation (const char *s,
                                      const char *t,
                                      size_t len)
{
  const char *scan1 = s;
  const char *scan2 = t;
  size_t count = 0;

  while (count++ < len
         && *scan1 != 0
         && ACE_OS_String::to_lower (*scan1)
            == ACE_OS_String::to_lower (*scan2))
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
}
#endif /* ACE_LACKS_STRCASECMP */

#if defined (ACE_HAS_WCHAR) && defined (ACE_LACKS_WCSNICMP)
int
ACE_OS_String::wcsnicmp_emulation (const wchar_t *s,
                                   const wchar_t *t,
                                   size_t len)
{
  const wchar_t *scan1 = s;
  const wchar_t *scan2 = t;
  size_t count = 0;

  while (count++ < len
         && *scan1 != 0
         && ACE_OS_String::to_lower (*scan1)
            == ACE_OS_String::to_lower (*scan2))
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
}
#endif /* ACE_HAS_WCHAR && ACE_LACKS_WCSNICMP */

#if !defined (ACE_HAS_REENTRANT_FUNCTIONS)
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
  s = ::strtok (s, tokens);
  if (s == 0)
    return 0;
  int l_sub = ACE_OS_String::strlen (s);
  *lasts = s + l_sub;
  if (l_sub != l_org)
    *lasts += 1;
  return s ;
}
#endif /* !ACE_HAS_REENTRANT_FUNCTIONS */

#if !defined (ACE_HAS_MEMCHR)
const void *
ACE_OS_String::memchr_emulation (const void *s, int c, size_t len)
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

#if !defined (ACE_HAS_ITOA)
char *
ACE_OS_String::itoa_emulation (int value, char *string, int radix)
{
  char *e = string;
  char *b = string;

  // Short circuit if 0

  if (value == 0)
    {
      string[0] = '0';
      string[1] = 0;
      return string;
    }

  // If negative and base 10, print a - and then do the
  // number.

  if (value < 0 && radix == 10)
    {
      string[0] = '-';
      b++;
    }

  // Convert to base <radix>, but in reverse order

  while (value != 0)
    {
      int mod = value % radix;
      value = value / radix;

      *e++ = (mod < 10) ? '0' + mod : 'a' + mod - 10;
    }

  *e-- = 0;

  // Now reverse the string to get the correct result

  while (e > b)
  {
    char temp = *e;
    *e = *b;
    *b = temp;
    ++b;
    --e;
  }

  return string;
}
#endif /* !ACE_HAS_ITOA */

#if defined (ACE_HAS_WCHAR) && defined (ACE_LACKS_ITOW)
wchar_t *
ACE_OS_String::itow_emulation (int value, wchar_t *string, int radix)
{
  wchar_t *e = string;
  wchar_t *b = string;

  // Short circuit if 0

  if (value == 0)
    {
      string[0] = '0';
      string[1] = 0;
      return string;
    }

  // If negative and base 10, print a - and then do the
  // number.

  if (value < 0 && radix == 10)
    {
      string[0] = '-';
      b++;
    }

  // Convert to base <radix>, but in reverse order

  while (value != 0)
    {
      int mod = value % radix;
      value = value / radix;

      *e++ = (mod < 10) ? '0' + mod : 'a' + mod - 10;
    }

  *e-- = 0;

  // Now reverse the string to get the correct result

  while (e > b)
  {
    wchar_t temp = *e;
    *e = *b;
    *b = temp;
    ++b;
    --e;
  }

  return string;
}
#endif /* ACE_HAS_WCHAR && ACE_LACKS_ITOW */

#if defined (ACE_HAS_WCHAR) && defined (ACE_LACKS_WCSCSPN)
size_t
ACE_OS_String::wcscspn_emulation (const wchar_t *s, const wchar_t *reject)
{
  const wchar_t *scan;
  const wchar_t *rej_scan;
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
#endif /* ACE_HAS_WCHAR && ACE_LACKS_WCSCSPN */

// The following wcs*_emulation methods were created based on BSD code:
/*-
 * Copyright (c) 1991, 1993
 *      The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * James W. Williams of NASA Goddard Space Flight Center.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by the University of
 *      California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */




#if defined (ACE_HAS_WCHAR) && defined (ACE_LACKS_WCSCAT)
wchar_t *
ACE_OS_String::wcscat_emulation (wchar_t *destination,
                                 const wchar_t *source)
{
  wchar_t *save = destination;

  for (; *destination; ++destination);
  while ((*destination++ = *source++));
  return save;
}
#endif /* ACE_HAS_WCHAR && ACE_LACKS_WCSCAT */

#if defined (ACE_LACKS_STRSPN)
size_t
ACE_OS_String::strspn_emulation (const char *string,
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

#if defined (ACE_HAS_WCHAR) && defined (ACE_LACKS_WCSSPN)
size_t
ACE_OS_String::wcsspn_emulation (const wchar_t *string,
                                 const wchar_t *charset)
{
  const wchar_t *p = string;
  const wchar_t *spanp;
  wchar_t c, sc;

  // Skip any characters in charset, excluding the terminating \0.
cont:
  c = *p++;
  for (spanp = charset; (sc = *spanp++) != 0;)
    if (sc == c)
      goto cont;
  return (p - 1 - string);
}
#endif /* ACE_HAS_WCHAR && ACE_LACKS_WCSSPN */

#if defined (ACE_HAS_WCHAR) && defined (ACE_LACKS_WCSSTR)
wchar_t *
ACE_OS_String::wcsstr_emulation (const wchar_t *string,
                                 const wchar_t *charset)
{
  wchar_t c, sc;
  size_t len;

  if ((c = *charset++) != 0)
    {
      len = strlen(charset);
      do
        {
          do
            {
              if ((sc = *string++) == 0)
                return NULL;
            } while (sc != c);
        } while (strncmp(string, charset, len) != 0);
      string--;
    }

  return ACE_const_cast (wchar_t *, string);
}
#endif /* ACE_HAS_WCHAR && ACE_LACKS_WCSSTR */

#if !defined (ACE_HAS_WCHAR) || defined (ACE_LACKS_WCSLEN)
size_t
ACE_OS_String::wcslen_emulation (const ACE_WCHAR_T *string)
{
  const ACE_WCHAR_T *s;

  for (s = string; *s; ++s);
    return s - string;
}
#endif /* !ACE_HAS_WCHAR || ACE_LACKS_WCSLEN */

#if !defined (ACE_HAS_WCHAR) || defined (ACE_LACKS_WCSNCPY)
ACE_WCHAR_T *
ACE_OS_String::wcsncpy_emulation (ACE_WCHAR_T *destination,
                                  const ACE_WCHAR_T *source,
                                  size_t len)
{
  if (len != 0)
    {
      ACE_WCHAR_T *d = destination;
      const ACE_WCHAR_T *s = source;

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

  return destination;
}
#endif /* !ACE_HAS_WCHAR || ACE_LACKS_WCSNCPY */

#if !defined (ACE_HAS_WCHAR) || defined (ACE_LACKS_WCSCMP)
int
ACE_OS_String::wcscmp_emulation (const ACE_WCHAR_T *string1,
                                 const ACE_WCHAR_T *string2)
{
  while (*string1 == *string2++)
    if (*string1++ == 0)
      return (0);
  return (*string1 - *--string2);
}
#endif /* !ACE_HAS_WCHAR || ACE_LACKS_WCSCMP */

#if defined (ACE_HAS_WCHAR) && defined (ACE_LACKS_WCSCPY)
wchar_t *
ACE_OS_String::wcscpy_emulation (wchar_t *destination,
                                 const wchar_t *source)
{
  wchar_t *save = destination;

  for (; (*destination = *source); ++source, ++destination);
  return save;
}
#endif /* ACE_HAS_WCHAR && ACE_LACKS_WCSCPY */

#if defined (ACE_LACKS_STRPBRK)
char *
ACE_OS_String::strpbrk_emulation (const char *string,
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

  return NULL;
}
#endif /* ACE_LACKS_STRPBRK */

#if defined (ACE_HAS_WCHAR) && defined (ACE_LACKS_WCSPBRK)
wchar_t *
ACE_OS_String::wcspbrk_emulation (const wchar_t *string,
                                  const wchar_t *charset)
{
  const wchar_t *scanp;
  int c, sc;

  while ((c = *string++) != 0)
    {
      for (scanp = charset; (sc = *scanp++) != 0;)
        if (sc == c)
          return ACE_const_cast (wchar_t *, string - 1);
    }

  return NULL;
}
#endif /* ACE_HAS_WCHAR && ACE_LACKS_WCSPBRK */

#if !defined (ACE_HAS_WCHAR) || defined (ACE_LACKS_WCSNCAT)
ACE_WCHAR_T *
ACE_OS_String::wcsncat_emulation (ACE_WCHAR_T *destination,
                                  const ACE_WCHAR_T *source,
                                  size_t count)
{
  if (count != 0)
    {
      ACE_WCHAR_T *d = destination;
      const ACE_WCHAR_T *s = source;

      while (*d != 0)
        d++;

      do
        {
          if ((*d = *s++) == 0)
            break;

          d++;
        } while (--count != 0);

      *d = 0;
    }

  return destination;
}
#endif /* !ACE_HAS_WCHAR || ACE_LACKS_WCSCAT */

#if defined (ACE_HAS_WCHAR) && defined (ACE_LACKS_WCSCHR)
wchar_t *
ACE_OS_String::wcschr_emulation (const wchar_t *string, wint_t c)
{
  for (;*string ; ++string)
    if (*string == ACE_static_cast (wchar_t, c))
      return ACE_const_cast (wchar_t *, string);

  return NULL;
}
#endif /* ACE_HAS_WCHAR && ACE_LACKS_WCSCHR */

#if !defined (ACE_HAS_WCHAR) || defined (ACE_LACKS_WCSNCMP)
int
ACE_OS_String::wcsncmp_emulation (const ACE_WCHAR_T *s1,
                                  const ACE_WCHAR_T *s2,
                                  size_t len)
{
  if (len == 0)
    return 0;

  do
    {
      if (*s1 != *s2++)
        return (*s1 - *--s2);
      if (*s1++ == 0)
        break;
    } while (--len != 0);

  return 0;
}
#endif /* !ACE_HAS_WCHAR || ACE_LACKS_WCSNCMP */

#if defined (ACE_LACKS_STRTOL)
long
ACE_OS_String::strtol_emulation (const char *nptr, char **endptr, int base)
{
  register const char *s = nptr;
  register unsigned long acc;
  register int c;
  register unsigned long cutoff;
  register int neg = 0, any, cutlim;

  /*
   * Skip white space and pick up leading +/- sign if any.
   * If base is 0, allow 0x for hex and 0 for octal, else
   * assume decimal; if base is already 16, allow 0x.
   */
  do {
    c = *s++;
  } while (isspace(c));
  if (c == '-') {
    neg = 1;
    c = *s++;
  } else if (c == '+')
    c = *s++;
  if ((base == 0 || base == 16) &&
    c == '0' && (*s == 'x' || *s == 'X')) {
    c = s[1];
    s += 2;
    base = 16;
  }
  if (base == 0)
    base = c == '0' ? 8 : 10;

  /*
   * Compute the cutoff value between legal numbers and illegal
   * numbers.  That is the largest legal value, divided by the
   * base.  An input number that is greater than this value, if
   * followed by a legal input character, is too big.  One that
   * is equal to this value may be valid or not; the limit
   * between valid and invalid numbers is then based on the last
   * digit.  For instance, if the range for longs is
   * [-2147483648..2147483647] and the input base is 10,
   * cutoff will be set to 214748364 and cutlim to either
   * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
   * a value > 214748364, or equal but the next digit is > 7 (or 8),
   * the number is too big, and we will return a range error.
   *
   * Set any if any `digits' consumed; make it negative to indicate
   * overflow.
   */
  cutoff = neg ? -(unsigned long)LONG_MIN : LONG_MAX;
  cutlim = cutoff % (unsigned long)base;
  cutoff /= (unsigned long)base;
  for (acc = 0, any = 0;; c = *s++) {
    if (isdigit(c))
      c -= '0';
    else if (isalpha(c))
      c -= isupper(c) ? 'A' - 10 : 'a' - 10;
    else
      break;
    if (c >= base)
      break;
    if (any < 0 || acc > cutoff || acc == cutoff && c > cutlim)
      any = -1;
    else {
      any = 1;
      acc *= base;
      acc += c;
    }
  }
  if (any < 0) {
    acc = neg ? LONG_MIN : LONG_MAX;
    errno = ERANGE;
  } else if (neg)
    acc = -acc;
  if (endptr != 0)
    *endptr = any ? (char *)s - 1 : (char *)nptr;
  return (acc);
}
#endif /* ACE_LACKS_STRTOL */

#if defined (ACE_LACKS_STRTOUL)
unsigned long
ACE_OS_String::strtoul_emulation (const char *nptr,
                                  char **endptr,
                                  register int base)
{
  register const char *s = nptr;
  register unsigned long acc;
  register int c;
  register unsigned long cutoff;
  register int neg = 0, any, cutlim;

  /*
   * See strtol for comments as to the logic used.
   */
  do {
    c = *s++;
  } while (isspace(c));
  if (c == '-') {
    neg = 1;
    c = *s++;
  } else if (c == '+')
    c = *s++;
  if ((base == 0 || base == 16) &&
    c == '0' && (*s == 'x' || *s == 'X')) {
    c = s[1];
    s += 2;
    base = 16;
  }
  if (base == 0)
    base = c == '0' ? 8 : 10;
  cutoff = (unsigned long)ULONG_MAX / (unsigned long)base;
  cutlim = (unsigned long)ULONG_MAX % (unsigned long)base;
  for (acc = 0, any = 0;; c = *s++) {
    if (isdigit(c))
      c -= '0';
    else if (isalpha(c))
      c -= isupper(c) ? 'A' - 10 : 'a' - 10;
    else
      break;
    if (c >= base)
      break;
    if (any < 0 || acc > cutoff || acc == cutoff && c > cutlim)
      any = -1;
    else {
      any = 1;
      acc *= base;
      acc += c;
    }
  }
  if (any < 0) {
    acc = ULONG_MAX;
    errno = ERANGE;
  } else if (neg)
    acc = -acc;
  if (endptr != 0)
    *endptr = any ? (char *)s - 1 : (char *)nptr;
  return (acc);
}
#endif /* ACE_LACKS_STRTOUL */

char *
ACE_OS_String::strsncpy (char *dst, const char *src, size_t maxlen)
{
  register char *rdst = dst;
  register const char *rsrc = src;
  register size_t rmaxlen = maxlen;

  if (rmaxlen > 0)
    {
      *rdst = '\0';
      if (rsrc != 0)
        strncat (rdst, rsrc, --rmaxlen);
    }
  return dst;
}

ACE_WCHAR_T *
ACE_OS_String::strsncpy (ACE_WCHAR_T *dst, const ACE_WCHAR_T *src, size_t maxlen)
{
  register ACE_WCHAR_T *rdst = dst;
  register const ACE_WCHAR_T *rsrc = src;
  register size_t rmaxlen = maxlen;

  if (rmaxlen > 0)
    {
      *rdst = ACE_TEXT_WIDE ('\0');
      if (rsrc != 0)
        strncat (rdst, rsrc ,--rmaxlen);
    }
  return dst;
}
