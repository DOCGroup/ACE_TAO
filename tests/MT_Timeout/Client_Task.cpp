//
// $Id$
//

#include "Client_Task.h"
#include "ace/OS_NS_unistd.h"
#include "tao/TimeBaseC.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"

Client_Task::Client_Task (CORBA::ORB_ptr orb,
                          Test::Sleep_Service_ptr sleep_service,
                          int iterations,
                          CORBA::ULong sleep_time,
                          CORBA::ULong timeout)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , sleep_service_ (Test::Sleep_Service::_duplicate (sleep_service))
  , iterations_ (iterations)
  , sleep_time_ (sleep_time)
  , timeout_ (timeout)
  , successful_calls_ (0)
  , timed_out_calls_ (0)
  , too_big_difference_ (0)
{
}

int
Client_Task::successful_calls (void) const
{
  return this->successful_calls_;
}

int
Client_Task::timed_out_calls (void) const
{
  return this->timed_out_calls_;
}

int
Client_Task::too_big_difference_calls (void) const
{
  return this->too_big_difference_;
}

int
Client_Task::svc (void)
{

  int successful_calls = 0;
  int timed_out_calls = 0;

  try
    {
      this->validate_connection ();

      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("PolicyCurrent");
      CORBA::PolicyCurrent_var policy_current =
        CORBA::PolicyCurrent::_narrow (object.in ());

      TimeBase::TimeT timeout_period = 10 * this->timeout_;

      CORBA::Any timeout_as_any;
      timeout_as_any <<= timeout_period;

      CORBA::PolicyList policy_list (1);
      policy_list.length (1);
      policy_list[0] =
        this->orb_->create_policy (Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
                                   timeout_as_any);

      policy_current->set_policy_overrides (policy_list,
                                            CORBA::ADD_OVERRIDE);

      for (int i = 0; i != this->iterations_; ++i)
        {
          int retval = this->one_iteration ();

          if (retval == 1)
            ++successful_calls;
          else if (retval == 0)
            ++timed_out_calls;

#if 0
          if (i % 50 == 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) - Client_Task::svc %d / %d iterations\n",
                          i, this->iterations_));
            }
#endif /* 0 */
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
  this->successful_calls_ += successful_calls;
  this->timed_out_calls_  += timed_out_calls;

  return 0;
}

void
Client_Task::validate_connection (void)
{
  try
    {
      for (int i = 0; i != 100; ++i)
        {
          (void) this->sleep_service_->go_to_sleep (0);
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
      ACE_Time_Value start = ACE_OS::gettimeofday ();

      this->sleep_service_->go_to_sleep (this->sleep_time_);

      ACE_Time_Value end = ACE_OS::gettimeofday ();

      if (this->sleep_time_ > this->timeout_)
        {
          // If we don't sleep then the next request will also
          // timeout:
          // The server is already sleeping, the client sends a new
          // request, but the server cannot process it until the
          // previous sleep completes. The sleep times for multiple
          // requests accumulate, and the client always fails.
          CORBA::ULong remainder =
            this->sleep_time_ - this->timeout_;
          ACE_Time_Value tv (0, remainder);
          ACE_OS::sleep (tv);
        }

      if (this->timeout_ > 0)
        {
          CORBA::ULong max_milliseconds = this->timeout_ / 1000;

          ACE_Time_Value elapsed = end - start;
          CORBA::ULong elapsed_milliseconds = elapsed.msec ();

          if (elapsed_milliseconds > max_milliseconds)
            {
              CORBA::ULong difference =
                elapsed_milliseconds - max_milliseconds;

              // If the different is more than 10 milliseconds we are
              // *way* off, this is an error.
              if (difference > 10)
                {
                  this->too_big_difference_++;
                  ACE_ERROR ((LM_ERROR,
                              "(%P|%t) Elapsed time = %d, expected %d\n",
                              elapsed_milliseconds, max_milliseconds));
                }
            }
        }

      return 1;
    }
  catch (const CORBA::TIMEOUT& )
    {
      // Ignore this exception, it is usually caused by a transient
      // condition
      return 0;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
    }

  return -1;
}
