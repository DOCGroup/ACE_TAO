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
      ACE_OS::sprintf (out, "%d", s);
    }
  else if (any >>= us)
    {
      ACE_OS::sprintf (out, "%u", us);
    }
  else if (any >>= l)
    {
      ACE_OS::sprintf (out, "%d", l);
    }
  else if (any >>= ul)
    {
      ACE_OS::sprintf (out, "%u", ul);
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
      double temp =
# if defined (ACE_CONFIG_WIN32_H)
      ACE_static_cast(double, ACE_static_cast (CORBA::LongLong, ull));
# else
              ull;
# endif /* ACE_CONFIG_WIN32_H */

      ACE_OS::sprintf (out, "%.0f", temp);
#endif /* ACE_LACKS_LONGLONG_T */
    }
  else
    {
      ACE_OS::strcpy (out, "Unsupported Any Type");
    }

  return out;
}
