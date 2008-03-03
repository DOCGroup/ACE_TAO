#include "Trigger_exec.h"
#include "ciao/CIAO_common.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Time_Value.h"
#include "RT.h"
#include <sstream>

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
        // Set RT priorities for the process.
        RT_Utils::set_priority ();

      }

      Trigger_exec_i::~Trigger_exec_i (void)
      {
        delete this->logger_;
      }


      int
      Trigger_exec_i::svc ()
      {
        // Timer to measure the e-2-e latency.
        ACE_High_Res_Timer timer;
        ACE_hrtime_t elapsed_time;
        while (this->active_)
          {
//             ACE_DEBUG ((LM_DEBUG, "\n%s::Trigger: Pushing test event\n",
//                         this->ID_.in ()));

            Test_var ev = new OBV_CIAO::RACE::Test;
            ev->load (this->load_);
            ev->task_ID (::CORBA::string_dup (this->ID_.in ()));

            try
              {
                timer.start ();
                this->context_->push_test_out (ev);
                timer.stop ();
              }
            catch (CORBA::Exception &ex)
              {
                ACE_PRINT_EXCEPTION (ex, "Excaption caught!\n");
                return -1;
              }

            // Measure the time taken to execute the invocation.
            timer.elapsed_microseconds (elapsed_time);

            ACE_Time_Value interval (0, 0);
            ACE_hrtime_t period;
            this->mutex_.acquire ();
            period = this->period_;
            this->mutex_.release ();

//             ACE_DEBUG ((LM_DEBUG, "%s::Trigger::Period: %Q msec\t"
//                         "Elapsed time: %Q msec\n",
//                         this->ID_.in (),
//                         period / 1000,
//                         elapsed_time / 1000));


            // Both period and elapsed_time are in to usec.
            if (period  >= elapsed_time)
              {
                this->metrics_mutex_.acquire ();
                // Now update the performance metrics.
                this->metrics_.total_count_++;
                this->metrics_mutex_.release ();

                // Sleep for the remaining time period.
                interval.set (0,
                              static_cast <suseconds_t>
                              (period - elapsed_time));

              }
            else
              {
                // If the elapsed time is grater than the period, then this
                // taks has missed its deadline.
                this->metrics_mutex_.acquire ();
                // Now update the performance metrics.
                this->metrics_.total_count_++;
                this->metrics_.miss_count_++;
                this->metrics_mutex_.release ();
              }
            //            ACE_DEBUG ((LM_DEBUG, "Sleeping for %f seconds.\n",
            //                        interval.msec () /1000.0));
            ACE_OS::sleep (interval);
          }
        return 0;
      }

      ::CORBA::Boolean
      Trigger_exec_i::set_rate (::CORBA::Double rate)
      {
        // Obtain the collected performance metrics.
        Performance metrics;
        ACE_hrtime_t elapsed_time;

        this->metrics_mutex_.acquire ();
        this->timer_.stop ();
        metrics = this->metrics_;
        this->timer_.elapsed_microseconds (elapsed_time);

        // Reset counters.
        this->metrics_.miss_count_ = 0;
        this->metrics_.total_count_ = 0;

        this->timer_.start ();

        this->metrics_mutex_.release ();

        std::stringstream msg;
        msg << (metrics.miss_count_ * 1.0) / metrics.total_count_
            << "\t"
            << (metrics.total_count_ * 1000000.0) / elapsed_time
            << "\t"
            << rate
            << std::endl;


        this->logger_->log (msg.str ());

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
      }

      void
      Trigger_exec_i::ciao_postactivate ()
      {
        // Now start the active object.
        if (this->activate () != 0)
          {
            ACE_ERROR ((LM_ERROR, "Trigger_exec: Error will activating the "
                        "periodic task!\n"));
          }
        // Start the timer.
        this->timer_.start ();
        this->active_ = true;

        // Create the logger.
        this->logger_ = new Logger (this->ID_.in ());
        this->logger_->log (std::string ("#DMR\tThroughput\tRate\n"));
      }

      void
      Trigger_exec_i::ccm_activate ()
      {
      }

      void
      Trigger_exec_i::ccm_passivate ()
      {
      }

      void
      Trigger_exec_i::ccm_remove ()
      {
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
