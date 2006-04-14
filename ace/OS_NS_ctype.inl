// -*- C++ -*-
// $Id$

#include "ace/os_include/os_ctype.h"
#include "ace/os_include/os_wctype.h"

ACE_INLINE int
ACE_OS::ace_tolower (int c)
{
  return tolower (c);
}

ACE_INLINE wint_t
ACE_OS::ace_towlower (wint_t c)
{
  #if !defined (ACE_LACKS_TOWLOWER)
    return towlower (c);
  #else
    ACE_UNUSED_ARG( c );
    ACE_NOTSUP_RETURN (0);
  #endif
}

ACE_INLINE int
ACE_OS::ace_toupper (int c)
{
  return toupper (c);
}

ACE_INLINE wint_t
ACE_OS::ace_towupper (wint_t c)
{
  #if !defined (ACE_LACKS_TOWUPPER)
    return towupper (c);
  #else
    ACE_UNUSED_ARG( c );
    ACE_NOTSUP_RETURN (0);
  #endif
}

