
#include "Client_Task.h"

Client_Task::Client_Task (Test::Process_Factory_ptr process_factory,
                          int const iterations)
  : process_factory_ (Test::Process_Factory::_duplicate (process_factory))
  , iterations_ (iterations)
{
}

int
Client_Task::successful_calls () const
{
  return this->successful_calls_;
}

int
Client_Task::svc ()
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting client task\n"));

  int successful_calls {};

  try
    {
      this->validate_connection ();

      for (int i = 0; i != this->iterations_; ++i)
        {
          int const retval = this->one_iteration ();

          if (retval != 0)
            ++successful_calls;

          if (i % 10 == 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) - Client_Task::svc %d / %d iterations\n",
                          i, this->iterations_));
            }
        }
    }
  catch (CORBA::Exception const &ex)
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
Client_Task::validate_connection ()
{
  try
    {
      for (int i = 0; i != 100; ++i)
        {
          (void) this->process_factory_->noop ();
        }
  }
  catch (CORBA::TRANSIENT const &)
    {
      // Ignore transient exceptions
    }
}

int
Client_Task::one_iteration ()
{
  try
    {
      Test::Process_var process =
        this->process_factory_->create_new_process ();

      (void) process->get_process_id ();

      process->shutdown ();

      return 1;
    }
  catch (Test::Spawn_Failed const &)
    {
      // Ignore this exception, it is usually caused by a transient
      // condition
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
    }

  return 0;
}
