//
// $Id$
//

#include "Client_Task.h"

ACE_RCSID(AMI_Buffering, Client_Task, "$Id$")

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
  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      while (1)
        {
          // run the even loop for 1 second...
          ACE_Time_Value tv (1, 0);
          this->orb_->run (tv TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
          if (this->terminate_loop_ != 0)
            break;
        }
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Client task finished\n"));
  return 0;
}
