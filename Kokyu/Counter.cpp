/* $Id$ */
#include "Counter.h"

#if ! defined (__ACE_INLINE__)
#include "Counter.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, Object_Counter, "Object_Counter,v 1.0 2004/02/19 10:24:13 storri Exp")

namespace Kokyu {

Object_Counter::Object_Counter ()
  : m_counter (0)
{
}

} /* namespace Kokyu */
