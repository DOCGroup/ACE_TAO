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

      //==================================================================
      // Facet Executor Implementation Class:   ScienceData_exec_i
      //==================================================================

      ScienceData_exec_i::ScienceData_exec_i (Trigger_exec_i &component)
        : component_ (component)
      {
      }

      ScienceData_exec_i::~ScienceData_exec_i (void)
      {
      }

      void
      ScienceData_exec_i::push_data (const ::CIAO::RACE::Message & /* msg */)
      {

        // Measure the e-2-e response time.
        this->component_.response_timer_.stop ();
        ACE_hrtime_t elapsed_time;
        this->component_.response_timer_.elapsed_microseconds (elapsed_time);

//         ACE_DEBUG ((LM_DEBUG, "%s::Trigger::Period: %f msec\t"
//                     "Elapsed time: %f msec\n",
//                     this->component_.ID_.in (),
//                     1000 / this->component_.rate_,
//                     elapsed_time / 1000.0));

        if (elapsed_time <
            ((1000000 / this->component_.rate_) * this->component_.subtasks_))
          {
            // Now we don't have a deadline miss.
            this->component_.metrics_.increment_total ();
          }
        else
          {
            // If the elapsed time is grater than the period, then we have
            // a deadline miss!
            this->component_.metrics_.increment_all ();
          }
      }

      //==================================================================
      // Component Executor Implementation Class:   Trigger_exec_i
      //==================================================================


      Trigger_exec_i::Trigger_exec_i (void)
        : rate_ (10),
          load_ (1.0),
          subtasks_ (1),
          ID_ (::CORBA::string_dup ("CIAO::RACE::Task")),
          active_ (false)
      {
        this->update_priority ();

      }

      Trigger_exec_i::~Trigger_exec_i (void)
      {
        delete this->logger_;
      }

      ::CIAO::RACE::CCM_ScienceData_ptr
      Trigger_exec_i::get_data_in ()
      {
        return (new ScienceData_exec_i (*this));
      }

      void
      Trigger_exec_i::update_priority ()
      {
        // Compute the priority based on current execution rate.
        this->priority_ = static_cast <size_t>
                          ( 20 + 20 * (this->rate_ /110));
        // Set RT priorities for the process.
       RT_Utils::set_priority (this->priority_);
      }


      int
      Trigger_exec_i::svc ()
      {
        ::CIAO::RACE::Message message;
        message.task_ID  = ::CORBA::string_dup (this->ID_.in ());
        ACE_Time_Value interval (0, 0);
        ACE_hrtime_t period;
        ACE_High_Res_Timer timer;
        while (this->active_)
          {
            message.load  = this->load_;
            message.priority = this->priority_;
            this->mutex_.acquire ();
            period = static_cast <ACE_hrtime_t> (1000000 / this->rate_);
            this->mutex_.release ();
            try
              {
                this->response_timer_.start ();
                timer.start ();
                this->context_->
                  get_connection_data_out ()->push_data (message);
                timer.stop ();
                ACE_hrtime_t elapsed_time;
                timer.elapsed_microseconds (elapsed_time);
                interval.set (0, period - elapsed_time);
                ACE_OS::sleep (interval);
              }
            catch (CORBA::Exception &ex)
              {
                ACE_DEBUG ((LM_ERROR, "Exception caught in %s.\n%s\n",
                            this->ID_.in (),
                            ex._info ().c_str ()));
                break;
              }
          }
        return 0;
      }

      ::CORBA::Boolean
      Trigger_exec_i::set_rate (::CORBA::Double rate)
      {

        this->tp_timer_.stop ();
        ACE_hrtime_t elapsed_time;
        this->tp_timer_.elapsed_microseconds (elapsed_time);
        this->tp_timer_.start ();

        // Obtain the collected performance metrics.
        size_t total_count;
        double DMR;
        this->metrics_.get_perf (DMR, total_count);

        std::stringstream msg;
        msg.width (10);
        msg.setf (ios::fixed,ios::floatfield);
        msg.precision (5);
        msg << DMR
            << "\t\t"
          // Rate is in Hz and the period is in usecs.
            << (total_count * 1000000.0) / elapsed_time
            << "\t\t"
            << this->rate_
            << "\n";
        this->logger_->log (msg.str ());

        this->mutex_.acquire ();
        this->rate_ = rate;
        this->update_priority ();
        this->mutex_.release ();
        return true;
      }

      ::CORBA::Double
      Trigger_exec_i::rate ()
      {
        return this->rate_;
      }

      void
      Trigger_exec_i::rate (::CORBA::Double rate)
      {
        if (rate > 0)
          {
            this->rate_ = rate;
            this->update_priority ();
          }
      }

      ::CORBA::Long
      Trigger_exec_i::subtasks ()
      {
        return static_cast < ::CORBA::Long > (this->subtasks_);
      }

      void
      Trigger_exec_i::subtasks (::CORBA::Long subtasks)
      {
        if (subtasks > 0)
          {
            this->subtasks_ = subtasks;
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
            ACE_DEBUG ((LM_DEBUG, "Load uodate: %f\n",
                        load));
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
        ACE_ERROR ((LM_ERROR, "Trigger_exec: Activated the periodic task!\n\n\n"));
        // Start the timer.
        this->tp_timer_.start ();
        this->active_ = true;

        // Create the logger.
        this->logger_ = new Logger (this->ID_.in ());
        this->logger_->log (std::string ("#DMR\tT/P\t\tRate\n"));
      }

      void
      Trigger_exec_i::ccm_activate ()
      {
      }

      void
      Trigger_exec_i::ccm_passivate ()
      {
        this->active_ = false;
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
