/* -*- C++ -*- */
// $Id$

#include "Notify_Push_Consumer.h"

CORBA::Short Notify_Push_Consumer::event_count = 0;

Notify_Push_Consumer::Notify_Push_Consumer (const char* name)
 : name_ (name)
{
}


CORBA::Short
Notify_Push_Consumer::get_count ()
{
  return event_count;
}



static const char*
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
      ACE_OS_String::strcpy (out, str);
    }
  else if (any >>= ull)
    {
#if defined (ACE_LACKS_LONGLONG_T)
      ACE_OS_String::strcpy (out, ull.as_string (out));
#else
      // @@@@ (JP) Need to cast to signed int64 to cast to
      // double on Win32, but this hack may not fly on 
      // other platforms.
      double temp = (double) (CORBA::LongLong) ull;
      ACE_OS::sprintf (out, "%.0f", temp);
#endif /* ACE_LACKS_LONGLONG_T */
    }
  else
    {
      ACE_OS_String::strcpy (out, "Unsupported Any Type");
    }

  return out;
}


void
Notify_Push_Consumer::push_structured_event (
    const CosNotification::StructuredEvent& event
    ACE_ENV_ARG_NOT_USED
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "%s %d (sent recv) %s %s\n",
              Any_String (event.filterable_data[1].value),
              event_count,
              (const char*)event.filterable_data[0].name,
              Any_String (event.filterable_data[0].value)));

  event_count ++;
}

