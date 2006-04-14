// -*- C++ -*-
// $Id$

ACE_INLINE wint_t
ACE_OS::fgetwc (FILE* fp)
{
#  if defined (ACE_LACKS_FGETWC)
  ACE_UNUSED_ARG (fp);
  ACE_NOTSUP_RETURN (0);
#  else
  ACE_OSCALL_RETURN (ACE_STD_NAMESPACE::fgetwc (fp), wint_t, WEOF);
#  endif /* ACE_LACKS_FGETWC */
}

ACE_INLINE u_int
ACE_OS::wslen (const wchar_t *s)
{
  u_int len = 0;

  while (*s++ != 0)
    len++;

  return len;
}

ACE_INLINE wchar_t *
ACE_OS::wscpy (wchar_t *dest, const wchar_t *src)
{
  wchar_t *original_dest = dest;

  while ((*dest++ = *src++) != 0)
    continue;

  return original_dest;
}

ACE_INLINE int
ACE_OS::wscmp (const wchar_t *s, const wchar_t *t)
{
  const wchar_t *scan1 = s;
  const wchar_t *scan2 = t;

  while (*scan1 != 0 && *scan1 == *scan2)
    {
      ++scan1;
      ++scan2;
    }

  return *scan1 - *scan2;
}

ACE_INLINE int
ACE_OS::wsncmp (const wchar_t *s, const wchar_t *t, size_t len)
{
  const wchar_t *scan1 = s;
  const wchar_t *scan2 = t;

  while (len != 0 && *scan1 != 0 && *scan1 == *scan2)
    {
      ++scan1;
      ++scan2;
      --len;
    }

  return len == 0 ? 0 : *scan1 - *scan2;
}

ACE_INLINE wint_t
ACE_OS::ungetwc (wint_t c, FILE* fp)
{
#  if defined (ACE_LACKS_FGETWC)
  ACE_UNUSED_ARG (c);
  ACE_UNUSED_ARG (fp);
  ACE_NOTSUP_RETURN (0);
#  else
  ACE_OSCALL_RETURN (ACE_STD_NAMESPACE::ungetwc (c, fp), wint_t, WEOF);
#  endif /* ACE_LACKS_FGETWC */
}

