//
// $Id$
//

#include "Client_Task.h"

Client_Task::Client_Task (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , terminate_loop_ (0)
{
}

void
Client_Task::terminate_loop (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  this->terminate_loop_ = 1;
}

int
Client_Task::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting client task\n"));
  try
    {
      while (1)
        {
          // run the even loop for 1 second...
          ACE_Time_Value tv (1, 0);
          this->orb_->run (tv);

          ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
          if (this->terminate_loop_ != 0)
            break;
        }
    }
  catch (const CORBA::Exception&)
    {
      return -1;
    }
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Client task finished\n"));
  return 0;
}
