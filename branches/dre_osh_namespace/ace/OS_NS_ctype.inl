// -*- C++ -*-
// $Id$

ACE_INLINE int
ACE_OS::ace_isprint (const ACE_TCHAR c)
{
#if defined (ACE_USES_WCHAR)
  return iswprint (c);
#else /* ACE_USES_WCHAR */
  return isprint ((unsigned char) c);
#endif /* ACE_USES_WCHAR */
}

ACE_INLINE int
ACE_OS::ace_isspace (const ACE_TCHAR c)
{
#if defined (ACE_USES_WCHAR)
  return iswspace (c);
#else /* ACE_USES_WCHAR */
  return isspace ((unsigned char) c);
#endif /* ACE_USES_WCHAR */
}

ACE_INLINE int
ACE_OS::to_lower (int c)
{
  return tolower (c);
}

#if defined (ACE_HAS_WCHAR) && !defined (ACE_LACKS_TOWLOWER)
ACE_INLINE wint_t
ACE_OS::to_lower (wint_t c)
{
  return towlower (c);
}
#endif /* ACE_HAS_WCHAR && !ACE_LACKS_TOWLOWER */

