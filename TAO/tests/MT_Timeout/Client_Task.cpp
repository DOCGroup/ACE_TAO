
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
  , successful_calls_ {}
  , timed_out_calls_ {}
  , too_big_difference_ {}
{
}

int
Client_Task::successful_calls () const
{
  return this->successful_calls_;
}

int
Client_Task::timed_out_calls () const
{
  return this->timed_out_calls_;
}

int
Client_Task::too_big_difference_calls () const
{
  return this->too_big_difference_;
}

int
Client_Task::svc ()
{
  int successful_calls {};
  int timed_out_calls {};

  try
    {
      this->validate_connection ();

      CORBA::Object_var const object =
        this->orb_->resolve_initial_references ("PolicyCurrent");
      CORBA::PolicyCurrent_var const policy_current =
        CORBA::PolicyCurrent::_narrow (object.in ());

      TimeBase::TimeT const timeout_period = 10 * this->timeout_;

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
          int const retval = this->one_iteration ();

          if (retval == 1)
            ++successful_calls;
          else if (retval == 0)
            ++timed_out_calls;
        }
    }
  catch (CORBA::Exception const& ex)
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
Client_Task::validate_connection ()
{
  try
    {
      for (int i = 0; i != 100; ++i)
        {
          (void) this->sleep_service_->go_to_sleep (0);
        }
    }
  catch (CORBA::TRANSIENT const&)
    {
      // Ignore transient exceptions
    }
}

int
Client_Task::one_iteration ()
{
  try
    {
      ACE_Time_Value const start = ACE_OS::gettimeofday ();

      this->sleep_service_->go_to_sleep (this->sleep_time_);

      ACE_Time_Value const end = ACE_OS::gettimeofday ();

      if (this->sleep_time_ > this->timeout_)
        {
          // If we don't sleep then the next request will also
          // timeout:
          // The server is already sleeping, the client sends a new
          // request, but the server cannot process it until the
          // previous sleep completes. The sleep times for multiple
          // requests accumulate, and the client always fails.
          CORBA::ULong const remainder =
            this->sleep_time_ - this->timeout_;
          ACE_Time_Value const tv (0, remainder);
          ACE_OS::sleep (tv);
        }

      if (this->timeout_ > 0)
        {
          CORBA::ULong const max_milliseconds = this->timeout_ / 1000;

          ACE_Time_Value const elapsed = end - start;
          CORBA::ULong const elapsed_milliseconds = elapsed.msec ();

          if (elapsed_milliseconds > max_milliseconds)
            {
              CORBA::ULong const difference =
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
  catch (CORBA::TIMEOUT const&)
    {
      // Ignore this exception, it is usually caused by a transient
      // condition
      return 0;
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Exception caught:");
    }

  return -1;
}
