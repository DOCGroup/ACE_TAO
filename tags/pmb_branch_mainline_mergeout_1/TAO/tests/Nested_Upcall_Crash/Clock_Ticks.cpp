/**
 * @file Clock_Ticks.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */
#include "Clock_Ticks.h"

ACE_RCSID(Nested_Upcall_Crash, Clock_Ticks, "$Id$")

int
Clock_Ticks::get_hz (void)
{
#if defined(_SC_CLK_TCK)
  int r = ACE_OS::sysconf(_SC_CLK_TCK);
  if(r != -1)
    return r;
#endif /* _SC_CLK_TCK */

#if defined(CLK_TCK)
  return CLK_TCK;
#elif defined(HZ)
  return HZ;
#else
  return 100; // A good guess for most systems!
#endif /* CLK_TCK */
}

int
Clock_Ticks::get_usecs_per_tick (void)
{
  return 1000000 / get_hz ();
}
