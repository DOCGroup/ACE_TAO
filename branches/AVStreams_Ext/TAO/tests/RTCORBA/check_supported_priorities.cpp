// $Id$

#include "ace/Sched_Params.h"
#include "tao/ORB_Core.h"

void
check_supported_priorities (CORBA::ORB_ptr orb)
{
  long current_policy =
      orb->orb_core ()->orb_params ()->sched_policy ();

  // Conversion.
  int sched_policy = ACE_SCHED_OTHER;
  if (current_policy == THR_SCHED_FIFO)
    sched_policy = ACE_SCHED_FIFO;
  else if (current_policy == THR_SCHED_RR)
    sched_policy = ACE_SCHED_RR;

  // Check that we have sufficient priority range to run this
  // test, i.e., more than 1 priority level.
  int max_priority =
    ACE_Sched_Params::priority_max (sched_policy);
  int min_priority =
    ACE_Sched_Params::priority_min (sched_policy);

  if (max_priority == min_priority)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Not enough priority levels on this platform"
                  " to run the test, aborting\n"));
      ACE_OS::exit (2);
    } 
}
