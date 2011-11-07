//
// $Id$
//

#include "Client_Task.h"

Client_Task::Client_Task (Test::Process_Factory_ptr process_factory,
                          int iterations)
  : process_factory_ (Test::Process_Factory::_duplicate (process_factory))
  , iterations_ (iterations)
  , successful_calls_ (0)
{
}

int
Client_Task::successful_calls (void) const
{
  return this->successful_calls_;
}

int
Client_Task::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting client task\n"));

  int successful_calls = 0;

  try
    {
      this->validate_connection ();

      for (int i = 0; i != this->iterations_; ++i)
        {
          int retval = this->one_iteration ();

          if (retval != 0)
            successful_calls++;

          if (i % 10 == 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) - Client_Task::svc %d / %d iterations\n",
                          i, this->iterations_));
            }
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Client task finished\n"));

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
  this->successful_calls_ += successful_calls;

  return 0;
}

void
Client_Task::validate_connection (void)
{
  try
    {
      for (int i = 0; i != 100; ++i)
        {
          (void) this->process_factory_->noop ();
        }
    }
  catch (const CORBA::TRANSIENT& )
    {
      // Ignore transient exceptions
    }
}

int
Client_Task::one_iteration (void)
{
  try
    {
      Test::Process_var process =
        this->process_factory_->create_new_process ();

      (void) process->get_process_id ();

      process->shutdown ();

      return 1;
    }
  catch (const Test::Spawn_Failed& )
    {
      // Ignore this exception, it is usually caused by a transient
      // condition
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
    }

  return 0;
}
