// $Id$

#include "ace/Sched_Params.h"
#include "tao/ORB_Core.h"

const char *
sched_policy_name (int sched_policy)
{
  const char *name = 0;

  switch (sched_policy)
    {
    case ACE_SCHED_OTHER:
      name = "SCHED_OTHER";
      break;
    case ACE_SCHED_RR:
      name = "SCHED_RR";
      break;
    case ACE_SCHED_FIFO:
      name = "SCHED_FIFO";
      break;
    }

  return name;
}

void
check_supported_priorities (CORBA::ORB_ptr orb)
{
  int sched_policy =
    orb->orb_core ()->orb_params ()->ace_sched_policy ();

  // Check that we have sufficient priority range to run this test,
  // i.e., more than 1 priority level.
  int max_priority =
    ACE_Sched_Params::priority_max (sched_policy);
  int min_priority =
    ACE_Sched_Params::priority_min (sched_policy);

  if (max_priority == min_priority)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Not enough priority levels with the %s scheduling policy\n"
                  "on this platform to run the test, terminating program....\n"
                  "Check svc.conf options\n",
                  sched_policy_name (sched_policy)));

      ACE_OS::exit (2);
    }
}
