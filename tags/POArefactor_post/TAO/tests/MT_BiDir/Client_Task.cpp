//
// $Id$
//

#include "Client_Task.h"

ACE_RCSID(Muxing, Client_Task, "Client_Task.cpp,v 1.1 2001/03/10 23:37:42 coryan Exp")

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

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->orb_->run (tv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "(%P|%t) Client_Task::svc exception!!!\n");

    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Client task finished\n"));
  return 0;
}
