#include "Echo_Caller.h"
#include "Server_Task.h"

ACE_RCSID (Bug_1020_Basic_Regression,
           Echo_Caller,
           "$Id$")

Echo_Caller::Echo_Caller(CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate(orb))
{
}

void
Echo_Caller::start_task(Test::Echo_ptr client
                        ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC((CORBA::SystemException))
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
Echo_Caller::shutdown(ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "Server received shutdown message\n"));
  orb_->shutdown(0 ACE_ENV_ARG_PARAMETER);
}
