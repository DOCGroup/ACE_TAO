// $Id$

#include "ace/OS_String.h"
#include "ace/OS.h"

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
      if (ACE_OS::memcmp (s1 + i, s2, len2) == 0)
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
      if (ACE_OS::memcmp (s1 + i, s2, len2 * sizeof (wchar_t)) == 0)
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
  char *t = (char *) ACE_OS::malloc (::strlen (s) + 1);
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
    (wchar_t *) ACE_OS::malloc ((ACE_OS_String::strlen (s) + 1)
                                * sizeof (wchar_t));
  return ::wcscpy (buffer, s);
#   else
  return ::wcsdup (s);
#   endif /* __BORLANDC__ */
}
#endif /* ACE_HAS_WCHAR */



