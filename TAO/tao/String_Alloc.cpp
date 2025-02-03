// -*- C++ -*-
#include "tao/String_Alloc.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_wchar.h"
#include "ace/OS_Memory.h"
#include <cstring>

#ifndef TAO_NO_SHARED_NULL_CORBA_STRING
  static char null_char[]= "";
  static CORBA::WChar null_wchar[]=
# if defined(ACE_HAS_WCHAR) || defined(ACE_HAS_XPG4_MULTIBYTE_CHAR)
    L"";
# else
    { 0 };
# endif
#endif /* TAO_NO_SHARED_NULL_CORBA_STRING */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

char *
CORBA::string_dup (const char *str)
{
  if (!str)
    {
      errno = EINVAL;
      return nullptr;
    }

#ifndef TAO_NO_SHARED_NULL_CORBA_STRING
  if (!*str)
    return null_char;
#endif /* TAO_NO_SHARED_NULL_CORBA_STRING */

  size_t const len = std::strlen (str);

  // This allocates an extra byte for the '\0';
  char * copy = CORBA::string_alloc (static_cast<CORBA::ULong> (len));
  if (copy)
    {
      // The memcpy() assumes that the destination is a valid buffer.
      ACE_OS::memcpy (copy, str, len + 1);
    }

  return copy;
}

char *
CORBA::string_alloc (CORBA::ULong len)
{
  // Allocate 1 + strlen to accomodate the null terminating character.
  char *s = nullptr;
  ACE_NEW_RETURN (s,
                  char[size_t (len + 1)],
                  nullptr);
  s[0]= '\0';

  return s;
}

void
CORBA::string_free (char *str)
{
#ifndef TAO_NO_SHARED_NULL_CORBA_STRING
  if (null_char != str)
#endif /* TAO_NO_SHARED_NULL_CORBA_STRING */
  delete [] str;
}

// ****************************************************************

CORBA::WChar*
CORBA::wstring_dup (const WChar *const str)
{
  if (!str)
    {
      errno = EINVAL;
      return nullptr;
    }

#ifndef TAO_NO_SHARED_NULL_CORBA_STRING
  if (!*str)
    return null_wchar;
#endif /* TAO_NO_SHARED_NULL_CORBA_STRING */

  CORBA::WChar* retval =
    CORBA::wstring_alloc (static_cast <CORBA::ULong> (ACE_OS::strlen (str)));
  if (retval == nullptr)
    {
      // The wscpy() below assumes that the destination is a valid buffer.
      return nullptr;
    }

  return ACE_OS::wscpy (retval, str);
}

CORBA::WChar*
CORBA::wstring_alloc (CORBA::ULong len)
{
  CORBA::WChar *s = nullptr;
  ACE_NEW_RETURN (s,
                  CORBA::WChar [(size_t) (len + 1)],
                  nullptr);
  return s;
}

void
CORBA::wstring_free (CORBA::WChar *const str)
{
#ifndef TAO_NO_SHARED_NULL_CORBA_STRING
  if (null_wchar != str)
#endif /* TAO_NO_SHARED_NULL_CORBA_STRING */
  delete [] str;
}

TAO_END_VERSIONED_NAMESPACE_DECL
