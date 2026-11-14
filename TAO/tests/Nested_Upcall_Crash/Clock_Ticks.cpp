/**
 * @file Clock_Ticks.cpp
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#include "Clock_Ticks.h"

long
Clock_Ticks::get_hz ()
{
#if defined(_SC_CLK_TCK)
  long const ticks = ACE_OS::sysconf (_SC_CLK_TCK);
  if (ticks != -1)
    return ticks;
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
Clock_Ticks::get_usecs_per_tick ()
{
  return 1000000 / get_hz ();
}
