// $Id$

#include "ace/Sched_Params.h"

void
check_supported_priorities (void)
{
  // Check that we have sufficient priority range to run this
  // test, i.e., more than 1 priority level.
  int max_priority =
    ACE_Sched_Params::priority_max (ACE_SCHED_OTHER);
  int min_priority =
    ACE_Sched_Params::priority_min (ACE_SCHED_OTHER);

  if (max_priority == min_priority)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Not enough priority levels on this platform"
                  "to run the test, aborting\n"));
      ACE_OS::exit (0);
    }
}
