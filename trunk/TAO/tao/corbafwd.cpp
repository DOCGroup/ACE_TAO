// $Id$

#include "tao/corbafwd.h"

#if !defined (__ACE_INLINE__)
# include "tao/corbafwd.i"
#endif /* ! __ACE_INLINE__ */

char *
CORBA::string_dup (const CORBA::Char *str)
{
  if (!str)
    return 0;

  size_t len = ACE_OS::strlen (str);

  // This allocates an extra byte for the '\0';
  char * copy = CORBA::string_alloc (len);

  ACE_OS::memcpy (copy, str, len + 1);
  return copy;
}

// ----------------------------------------------------------------------
// Wide strings
// ----------------------------------------------------------------------

CORBA::WChar*
CORBA::wstring_dup (const WChar *const str)
{
  if (!str)
    return 0;

  CORBA::WChar* retval = CORBA::wstring_alloc (ACE_OS::wslen (str));
  return ACE_OS::wscpy (retval, str);
}

// ****************************************************************

ACE_RCSID(tao, corbafwd, "$Id$")

TAO_NAMESPACE_TYPE (const CORBA::ServiceType)
TAO_NAMESPACE_BEGIN (CORBA)
TAO_NAMESPACE_DEFINE (const CORBA::ServiceType, Security, 1)
TAO_NAMESPACE_END
