// $Id$

#include "common.h"

#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"


const char*
Any_String (const CORBA::Any& any)
{
  static char out[256] = "";
  CORBA::Short s;
  CORBA::UShort us;
  CORBA::Long l;
  CORBA::ULong ul;
  CORBA::ULongLong ull;
  const char* str;

  if (any >>= s)
    {
      ACE_OS::sprintf (out, ACE_INT16_FORMAT_SPECIFIER, s);
    }
  else if (any >>= us)
    {
      ACE_OS::sprintf (out, ACE_UINT16_FORMAT_SPECIFIER, us);
    }
  else if (any >>= l)
    {
      ACE_OS::sprintf (out, ACE_INT32_FORMAT_SPECIFIER, l);
    }
  else if (any >>= ul)
    {
      ACE_OS::sprintf (out, ACE_UINT32_FORMAT_SPECIFIER, ul);
    }
  else if (any >>= str)
    {
      ACE_OS::strcpy (out, str);
    }
  else if (any >>= ull)
    {
#if defined (ACE_LACKS_LONGLONG_T)
      ACE_OS::strcpy (out, ull.as_string (out));
#else
      ACE_OS::sprintf (out, ACE_UINT64_FORMAT_SPECIFIER, ull);
#endif /* ACE_LACKS_LONGLONG_T */
    }
  else
    {
      ACE_OS::strcpy (out, "Unsupported Any Type");
    }

  return out;
}
