//
// $Id$
//

#include "Server_Task.h"

ACE_RCSID(Client_Leaks, Server_Task, "$Id$")

Server_Task::Server_Task (CORBA::ORB_ptr orb,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
  , orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Server_Task::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting server task\n"));
  try
    {
      // run the test for at most 120 seconds...
      // ACE_Time_Value tv (120, 0);
      this->orb_->run ( /* tv, */);
    }
  catch (const CORBA::Exception&)
    {
      return -1;
    }
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Server task finished\n"));
  return 0;
}
