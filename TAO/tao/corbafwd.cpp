// $Id$

#include "tao/corbafwd.h"

#if !defined (__ACE_INLINE__)
# include "tao/corbafwd.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (tao, 
           corbafwd, 
           "$Id$")

char *
CORBA::string_dup (const char *str)
{
  if (!str)
    {
      errno = EINVAL;
      return 0;
    }

  size_t len = ACE_OS::strlen (str);

  // This allocates an extra byte for the '\0';
  char * copy = CORBA::string_alloc (len);

  // The memcpy() below assumes that the destination is a valid buffer.  
  if (copy == 0)
    {
      return 0;
    }

  ACE_OS::memcpy (copy, 
                  str, 
                  len + 1);
  return copy;
}

// ----------------------------------------------------------------------
// Wide strings
// ----------------------------------------------------------------------

CORBA::WChar*
CORBA::wstring_dup (const WChar *const str)
{
  if (!str)
    {
      errno = EINVAL;
      return 0;
    }

  CORBA::WChar* retval = CORBA::wstring_alloc (ACE_OS::wslen (str));

  // The wscpy() below assumes that the destination is a valid buffer.
  if (retval == 0)
    {
      return 0;
    }

  return ACE_OS::wscpy (retval, 
                        str);
}

// ****************************************************************

TAO_NAMESPACE_TYPE (const CORBA::ServiceType)
TAO_NAMESPACE_BEGIN (CORBA)
TAO_NAMESPACE_DEFINE (const CORBA::ServiceType, Security, 1)
TAO_NAMESPACE_END

// ****************************************************************

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (CORBA)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_Visibility, 0)
TAO_NAMESPACE_END

TAO_NAMESPACE_TYPE (const CORBA::Visibility)
TAO_NAMESPACE_BEGIN (CORBA)
TAO_NAMESPACE_DEFINE (const CORBA::Visibility, PRIVATE_MEMBER, 0)
TAO_NAMESPACE_END
TAO_NAMESPACE_TYPE (const CORBA::Visibility)
TAO_NAMESPACE_BEGIN (CORBA)
TAO_NAMESPACE_DEFINE (const CORBA::Visibility, PUBLIC_MEMBER, 1)
TAO_NAMESPACE_END

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (CORBA)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_ValueModifier, 0)
TAO_NAMESPACE_END

TAO_NAMESPACE_TYPE (const CORBA::ValueModifier)
TAO_NAMESPACE_BEGIN (CORBA)
TAO_NAMESPACE_DEFINE (const CORBA::ValueModifier, VM_NONE, 0)
TAO_NAMESPACE_END
TAO_NAMESPACE_TYPE (const CORBA::ValueModifier)
TAO_NAMESPACE_BEGIN (CORBA)
TAO_NAMESPACE_DEFINE (const CORBA::ValueModifier, VM_CUSTOM, 1)
TAO_NAMESPACE_END
TAO_NAMESPACE_TYPE (const CORBA::ValueModifier)
TAO_NAMESPACE_BEGIN (CORBA)
TAO_NAMESPACE_DEFINE (const CORBA::ValueModifier, VM_ABSTRACT, 2)
TAO_NAMESPACE_END
TAO_NAMESPACE_TYPE (const CORBA::ValueModifier)
TAO_NAMESPACE_BEGIN (CORBA)
TAO_NAMESPACE_DEFINE (const CORBA::ValueModifier, VM_TRUNCATABLE, 3)
TAO_NAMESPACE_END

