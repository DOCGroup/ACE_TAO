//
// $Id$
//

#include "Server_Task.h"
#include "Sender.h"

Server_Task::Server_Task (CORBA::ORB_ptr orb,
                          const Sender *sender,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
  , orb_ (CORBA::ORB::_duplicate (orb))
  , sender_ (sender)
{
}

int
Server_Task::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting server task\n"));
  try
    {
      while (!this->sender_->is_done ())
        {
          // run the test for at most 120 seconds...
          ACE_Time_Value tv (240, 0);
          this->orb_->run (tv);
        }

    }
  catch (const CORBA::Exception&)
    {
      return -1;
    }
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Server task finished\n"));
  return 0;
}
