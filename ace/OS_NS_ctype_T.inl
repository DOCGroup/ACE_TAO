// -*- C++ -*-
// $Id$

#include "ace/os_include/os_ctype.h"
#include "ace/os_include/os_wctype.h"

namespace ACE_OS
{

// For the time being we are safe lumping the wchar specializations
// with ACE_USES_WCHAR. When the time comes that they are needed
// outside of a "wide" build, then we will need to us appropriate
// ACE_LACKS_XXXX.

template <typename CHAR_TYPE> inline int
ace_isalnum (CHAR_TYPE c)
{
  return isalnum (static_cast<unsigned char> (c));
}

#if defined (ACE_USES_WCHAR)
template <> inline int
ace_isalnum (wchar_t c)
{
  return iswalnum (c);
}
#endif /* ACE_USES_WCHAR */

template <typename CHAR_TYPE> inline int
ace_isalpha (CHAR_TYPE c)
{
  return isalpha (static_cast<unsigned char> (c));
}

#if defined (ACE_USES_WCHAR)
template <> inline int
ace_isalpha (wchar_t c)
{
  return iswalpha (c);
}
#endif /* ACE_USES_WCHAR */

template <typename CHAR_TYPE> inline int
ace_iscntrl (CHAR_TYPE c)
{
  return iscntrl (static_cast<unsigned char> (c));
}

#if defined (ACE_USES_WCHAR)
template <> inline int
ace_iscntrl (wchar_t c)
{
  return iswcntrl (c);
}
#endif /* ACE_USES_WCHAR */

template <typename CHAR_TYPE> inline int
ace_isdigit (CHAR_TYPE c)
{
  return isdigit (static_cast<unsigned char> (c));
}

#if defined (ACE_USES_WCHAR)
template <> inline int
ace_isdigit (wchar_t c)
{
  return iswdigit (c);
}
#endif /* ACE_USES_WCHAR */

template <typename CHAR_TYPE> inline int
ace_isgraph (CHAR_TYPE c)
{
  return isgraph (static_cast<unsigned char> (c));
}

#if defined (ACE_USES_WCHAR)
template <> inline int
ace_isgraph (wchar_t c)
{
  return iswgraph (c);
}
#endif /* ACE_USES_WCHAR */

template <typename CHAR_TYPE> inline int
ace_islower (CHAR_TYPE c)
{
  return islower (static_cast<unsigned char> (c));
}

#if defined (ACE_USES_WCHAR)
template <> inline int
ace_islower (wchar_t c)
{
  return iswlower (c);
}
#endif /* ACE_USES_WCHAR */

template <typename CHAR_TYPE> inline int
ace_isprint (CHAR_TYPE c)
{
  return isprint (static_cast<unsigned char> (c));
}

#if defined (ACE_USES_WCHAR)
template <> inline int
ace_isprint (wchar_t c)
{
  return iswprint (c);
}
#endif /* ACE_USES_WCHAR */

template <typename CHAR_TYPE> inline int
ace_ispunct (CHAR_TYPE c)
{
  return ispunct (static_cast<unsigned char> (c));
}

#if defined (ACE_USES_WCHAR)
template <> inline int
ace_ispunct (wchar_t c)
{
  return iswpunct (c);
}
#endif /* ACE_USES_WCHAR */

template <typename CHAR_TYPE> inline int
ace_isspace (CHAR_TYPE c)
{
  return isspace (static_cast<unsigned char> (c));
}

#if defined (ACE_USES_WCHAR)
template <> inline int
ace_isspace (wchar_t c)
{
  return iswspace (c);
}
#endif /* ACE_USES_WCHAR */

template <typename CHAR_TYPE> inline int
ace_isupper (CHAR_TYPE c)
{
  return isupper (static_cast<unsigned char> (c));
}

#if defined (ACE_USES_WCHAR)
template <> inline int
ace_isupper (wchar_t c)
{
  return iswupper (c);
}
#endif /* ACE_USES_WCHAR */

template <typename CHAR_TYPE> inline int
ace_isxdigit (CHAR_TYPE c)
{
  return isxdigit (static_cast<unsigned char> (c));
}

#if defined (ACE_USES_WCHAR)
template <> inline int
ace_isxdigit (wchar_t c)
{
  return iswxdigit (c);
}
#endif /* ACE_USES_WCHAR */

}
