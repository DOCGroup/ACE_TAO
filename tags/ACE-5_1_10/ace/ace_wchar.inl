/* -*- C++ -*- */
// $Id$

// These are always inlined
// FUZZ: disable check_for_inline

#if defined (ACE_HAS_WCHAR)

inline
ACE_Wide_To_Ascii::ACE_Wide_To_Ascii (const wchar_t *s) 
  : s_ (ACE_Wide_To_Ascii::convert (s)) 
{
}


inline
ACE_Wide_To_Ascii::~ACE_Wide_To_Ascii (void) 
{ 
  delete [] this->s_; 
}

inline char *
ACE_Wide_To_Ascii::char_rep (void) 
{ 
  return this->s_; 
}


inline char *
ACE_Wide_To_Ascii::convert (const wchar_t *wstr)
{
# if defined (ACE_WIN32)
  size_t len = ::WideCharToMultiByte (CP_OEMCP, 
                                      0, 
                                      wstr, 
                                      -1, 
                                      NULL, 
                                      0, 
                                      NULL, 
                                      NULL);
# elif defined (VXWORKS)
  // @@ we should use a different macro than VXWORKS here, ACE_LACKS_WCSLEN?
  const wchar_t *wtemp = wstr;
  while (wtemp != 0)
    ++wtemp;

  size_t len = wtemp - wstr + 1;
# else  /* ACE_WIN32 */
  size_t len = ::wcslen (wstr) + 1;
# endif /* ACE_WIN32 */

  char *str = new char[len];

# if defined (ACE_WIN32)
  ::WideCharToMultiByte (CP_OEMCP, 0, wstr, -1, str, len, NULL, NULL);
# elif defined (VXWORKS)
  ::wcstombs (str, wstr, len);
# else /* ACE_WIN32 */
  for (size_t i = 0; i < len; i++)
    {
      wchar_t *t = ACE_const_cast (wchar_t *, wstr);
      str[i] = ACE_static_cast (char, *(t + i));
    }
# endif /* ACE_WIN32 */
  return str;
}


inline 
ACE_Ascii_To_Wide::ACE_Ascii_To_Wide (const char *s) 
: s_ (ACE_Ascii_To_Wide::convert (s)) 
{
}


inline 
ACE_Ascii_To_Wide::~ACE_Ascii_To_Wide (void) 
{ 
  delete [] this->s_; 
}

inline wchar_t *
ACE_Ascii_To_Wide::wchar_rep (void) 
{ 
  return this->s_; 
}


inline wchar_t *
ACE_Ascii_To_Wide::convert (const char *str)
{
# if defined (ACE_WIN32)
  size_t len = ::MultiByteToWideChar (CP_OEMCP, 0, str, -1, NULL, 0);
# else /* ACE_WIN32 */
  size_t len = strlen (str) + 1;
# endif /* ACE_WIN32 */
    
  wchar_t *wstr = new wchar_t[len];

# if defined (ACE_WIN32)
  ::MultiByteToWideChar (CP_OEMCP, 0, str, -1, wstr, len);
# elif defined (VXWORKS)
  ::mbstowcs (wstr, str, len);
# else /* ACE_WIN32 */
  for (size_t i = 0; i < len; i++)
    {
      char *t = ACE_const_cast (char *, str);
      wstr[i] = ACE_static_cast (wchar_t, *(t + i));
    }
# endif /* ACE_WIN32 */
  return wstr;
}

#endif /* ACE_HAS_WCHAR */

