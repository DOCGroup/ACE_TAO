// $Id$

#include "Echo_Caller.h"
#include "Server_Task.h"

Echo_Caller::Echo_Caller(CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate(orb))
{
}

void
Echo_Caller::start_task(Test::Echo_ptr client)
{
  Server_Task task (client,
                    32768);

  if (task.activate (THR_NEW_LWP | THR_JOINABLE,
                     4,
                     1) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "Error activating client task\n"));
    }

  ACE_Thread_Manager::instance ()->wait ();

  return;
}

void
Echo_Caller::shutdown(void)
{
  ACE_DEBUG ((LM_DEBUG, "Server received shutdown message\n"));
  orb_->shutdown(0);
}
