// $Id$

#include "tao/corbafwd.h"

#if !defined (__ACE_INLINE__)
# include "tao/corbafwd.i"
#endif /* ! __ACE_INLINE__ */

CORBA::String
CORBA::string_alloc (CORBA::ULong len)
{
  // Allocate 1 + strlen to accomodate the null terminating character.
  return new CORBA::Char[size_t (len + 1)];
}

void
CORBA::string_free (CORBA::Char *str)
{
  delete [] str;
}

CORBA::String
CORBA::string_dup (const CORBA::Char *str)
{
  if (!str)
    return 0;

  size_t len = ACE_OS::strlen (str);

  // This allocates an extra byte for the '\0';
  CORBA::String copy = CORBA::string_alloc (len);

  ACE_OS::memcpy (copy, str, len + 1);
  return copy;
}

// ----------------------------------------------------------------------
// Wide strings
// ----------------------------------------------------------------------

CORBA::WChar*
CORBA::wstring_alloc (CORBA::ULong len)
{
  return new CORBA::WChar [(size_t) (len + 1)];
}

void
CORBA::wstring_free (CORBA::WChar *const str)
{
  delete [] str;
}

CORBA::WChar*
CORBA::wstring_dup (const WChar *const str)
{
  if (!str)
    return 0;

  CORBA::WChar* retval = CORBA::wstring_alloc (ACE_OS::wslen (str));
  return ACE_OS::wscpy (retval, str);
}

// ****************************************************************

CORBA_Environment&
CORBA::default_environment ()
{
  return TAO_default_environment ();
}

ACE_RCSID(tao, corbafwd, "$Id$")

TAO_NAMESPACE_TYPE (const CORBA::ServiceType)
TAO_NAMESPACE_BEGIN (CORBA)
TAO_NAMESPACE_DEFINE (const CORBA::ServiceType, Security, 1)
TAO_NAMESPACE_END
