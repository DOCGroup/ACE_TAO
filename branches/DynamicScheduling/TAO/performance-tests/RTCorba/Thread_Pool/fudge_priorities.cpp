// $Id$

#include "ace/Sched_Params.h"

void 
fudge_priorities (CORBA::ORB_ptr orb)
{

#if defined (ACE_CONFIG_LINUX_H)      

  // On Linux SCHED_OTHER default to 0, while other scheduling classes
  // start at 1.  Therefore we do the following to get things in
  // synch.
  {
    int thread_policy = 
      orb->orb_core ()->orb_params ()->sched_policy ();
    
    // Conversion.
    int sched_policy = SCHED_OTHER;
    if (thread_policy == THR_SCHED_FIFO)
      sched_policy = SCHED_FIFO;
    else if (thread_policy == THR_SCHED_RR)
      thread_policy = SCHED_RR;
    
    sched_param sparam;
    memset ((void *) &sparam, 0, sizeof sparam);
    
    sparam.sched_priority = 
      ACE_Sched_Params::priority_min (sched_policy);
    
    int result =
      pthread_setschedparam (pthread_self (),
			     sched_policy,
			     &sparam);
    if (result != 0)
      {
	errno = result;
	perror ("pthread_setschedparam");
	ACE_DEBUG ((LM_DEBUG, 
		    "Priority = %d; Policy = %d\n",
		    sparam.sched_priority,
		    sched_policy));
      }
  }
  
#else /* ACE_CONFIG_LINUX_H */
  
  ACE_UNUSED_ARG (orb);

#endif /* ACE_CONFIG_LINUX_H */

}
