//
// $Id$
//

#include "Client_Task.h"

ACE_RCSID(Client_Leaks, Client_Task, "$Id$")

Client_Task::Client_Task (Test::Process_Factory_ptr process_factory,
                          int iterations)
  : process_factory_ (Test::Process_Factory::_duplicate (process_factory))
  , iterations_ (iterations)
{
}

int
Client_Task::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting client task\n"));
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      for (int i = 0; i != this->iterations_; ++i)
        {
          this->one_iteration (ACE_TRY_ENV);
          ACE_TRY_CHECK;

          if (i % 10 == 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) - Client_Task::svc %d / %d iterations\n",
                          i, this->iterations_));
            }
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

void
Client_Task::one_iteration (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      Test::Process_var process =
        this->process_factory_->create_new_process (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      (void) process->get_process_id (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      process->shutdown (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH(Test::Spawn_Failed, ignored)
    {
      // Ignore this exception, it is usually caused by a transient
      // condition
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
    }
  ACE_ENDTRY;
}
