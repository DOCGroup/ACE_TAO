//
// $Id$
//

#include "Client_Task.h"

Client_Task::Client_Task (CORBA::ORB_ptr orb,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
  , orb_ (CORBA::ORB::_duplicate (orb))
 {
}

int
Client_Task::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting client task\n"));

  // Wait for 2 minutes...
  ACE_Time_Value tv (60,0);

  try
    {
      this->orb_->run (tv);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("(%P|%t) Client_Task::svc exception!!!\n");

    }

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Client task finished\n"));
  return 0;
}
