#include "Trigger_exec.h"
#include "ciao/CIAO_common.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Time_Value.h"

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_Trigger
    {
      Trigger_exec_i::Trigger_exec_i (void)
        : period_ (1000),
          load_ (1.0),
          ID_ (::CORBA::string_dup ("CIAO::RACE::Task")),
          active_ (false)
      {
      }

      Trigger_exec_i::~Trigger_exec_i (void)
      {
      }


      int
      Trigger_exec_i::svc ()
      {
        while (this->active_)
          {
            ACE_DEBUG ((LM_DEBUG, "\n%s::Trigger: Pushing test event\n",
                        this->ID_.in ()));

            Test_var ev = new OBV_CIAO::RACE::Test;
            ev->load (this->load_);
            ev->task_ID (::CORBA::string_dup (this->ID_.in ()));

            try
              {
                this->timer_.start ();
                this->context_->push_test_out (ev);
                this->timer_.stop ();
              }
            catch (CORBA::Exception &ex)
              {
                ACE_PRINT_EXCEPTION (ex, "Excaption caught!\n");
                return -1;
              }


            // Measure the time taken to execute the invocation.
            this->timer_.elapsed_microseconds (this->elapsed_time_);

            ACE_Time_Value interval (0, 0);
            ACE_hrtime_t period;
            this->mutex_.acquire ();
            period = this->period_;
            this->mutex_.release ();

            ACE_DEBUG ((LM_DEBUG, "%s::Trigger::Period: %Q msec\t"
                        "Elapsed time: %Q msec\n",
                        this->ID_.in (),
                        period / 1000,
                        this->elapsed_time_ / 1000));

            // Both period and elapsed_time_ are in to usec.
            if (period  > this->elapsed_time_)
              {
                // Sleep for the remaining time period.
                interval.set (0, period - this->elapsed_time_ );

              }
            //            ACE_DEBUG ((LM_DEBUG, "Sleeping for %f seconds.\n",
            //                        interval.msec () /1000.0));
            ACE_OS::sleep (interval);
          }
      }

      ::CORBA::Boolean
      Trigger_exec_i::set_rate (::CORBA::Double rate)
      {
        ACE_DEBUG ((LM_DEBUG, "(%P) Got rate! New rate is %f\n", rate));
        this->mutex_.acquire ();
        // We multiply by 1000000 since the period is specified in usecs.
        this->period_ = static_cast <ACE_hrtime_t> ((1.0 / rate) * 1000000);
        this->mutex_.release ();
        return true;
      }



      ::CORBA::Long
      Trigger_exec_i::period ()
      {
        return static_cast < ::CORBA::Long > (this->period_);
      }

      void
      Trigger_exec_i::period (::CORBA::Long period)
      {
        if (period > 0)
          {
            this->period_ = period;
          }
      }

      ::CORBA::Double
      Trigger_exec_i::load ()
      {
        return this->load_;
      }

      void
      Trigger_exec_i::load (::CORBA::Double load)
      {
        if (load > 0)
          {
            this->load_ = load;
          }
      }

      char*
      Trigger_exec_i::task_ID ()
      {
        return ::CORBA::string_dup (this->ID_);
      }

      void
      Trigger_exec_i::task_ID (const char* task_ID)
      {
        if (task_ID)
          {
            this->ID_ = ::CORBA::string_dup (task_ID);
          }
      }

      void
      Trigger_exec_i::set_session_context (
        ::Components::SessionContext_ptr ctx)
      {
        this->context_ =
          ::CIAO::RACE::CCM_Trigger_Context::_narrow (ctx);

        if (CORBA::is_nil (this->context_.in ()))
        {
          throw ::CORBA::INTERNAL ();
        }
      }

      void
      Trigger_exec_i::ciao_preactivate ()
      {
        // Your code here.
      }

      void
      Trigger_exec_i::ciao_postactivate ()
      {
        // Now activate th   e active object.
        if (this->activate () != 0)
          {
            ACE_ERROR ((LM_ERROR, "Trigger_exec: Error will activating the "
                        "periodic task!\n"));
          }
        this->active_ = true;
      }

      void
      Trigger_exec_i::ccm_activate ()
      {
        // Your code here.
      }

      void
      Trigger_exec_i::ccm_passivate ()
      {
        // Your code here.
      }

      void
      Trigger_exec_i::ccm_remove ()
      {
        // Your code here.
      }

      //==================================================================
      // Home Executor Implementation Class:   Trigger_Factory_exec_i
      //==================================================================

      Trigger_Factory_exec_i::Trigger_Factory_exec_i (void)
      {
      }

      Trigger_Factory_exec_i::~Trigger_Factory_exec_i (void)
      {
      }

      // Supported or inherited operations.

      // Home operations.

      // Factory and finder operations.

      // Attribute operations.

      // Implicit operations.

      ::Components::EnterpriseComponent_ptr
      Trigger_Factory_exec_i::create ()
      {
        ::Components::EnterpriseComponent_ptr retval =
          ::Components::EnterpriseComponent::_nil ();

        ACE_NEW_THROW_EX (
          retval,
          Trigger_exec_i,
          ::CORBA::NO_MEMORY ());

        return retval;
      }

      extern "C" TRIGGER_EXEC_Export ::Components::HomeExecutorBase_ptr
      create_CIAO_RACE_Trigger_Factory_Impl (void)
      {
        ::Components::HomeExecutorBase_ptr retval =
          ::Components::HomeExecutorBase::_nil ();

        ACE_NEW_RETURN (
          retval,
          Trigger_Factory_exec_i,
          ::Components::HomeExecutorBase::_nil ());

        return retval;
      }
    }
  }
}
