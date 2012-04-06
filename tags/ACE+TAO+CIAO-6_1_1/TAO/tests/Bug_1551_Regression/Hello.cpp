// $Id$

#include "Hello.h"
#include "ace/OS_NS_unistd.h"

Hello::Hello(
    CORBA::ORB_ptr orb,
    bool simulate_crashes)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , simulate_crashes_(simulate_crashes)
  , atomic_counter_(200)
{
}

void
Hello::short_sleep (void)
{
  long count = atomic_counter_--;

#if 0
  if(count % 40 == 0)
  {
    ACE_DEBUG((LM_DEBUG, "(%P|%t) Counter was %d\n", count));
  }
#endif /* 0 */
  if(simulate_crashes_ && count == 0)
  {
    ACE_DEBUG((LM_DEBUG, "Aborting!\n"));
    ACE_Time_Value tv(5, 0);
    ACE_OS::sleep(tv);
    ACE_OS::abort();
  }
}
