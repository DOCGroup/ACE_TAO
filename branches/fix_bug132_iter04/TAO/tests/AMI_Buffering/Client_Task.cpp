//
// $Id$
//

#include "Client_Task.h"

ACE_RCSID(AMI_Buffering, Client_Task, "$Id$")

Client_Task::Client_Task (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Client_Task::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting client task\n"));
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // run the test for at most 240 seconds...
      ACE_Time_Value tv (240, 0);
      this->orb_->run (tv, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Client task finished\n"));
  return 0;
}
