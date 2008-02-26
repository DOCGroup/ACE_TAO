#include "Trigger_exec.h"
#include "ciao/CIAO_common.h"
#include "ace/OS_NS_unistd.h"
//#include "ace/Time_Value.h"

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_Trigger
    {
      Trigger_exec_i::Trigger_exec_i (void)
        : period_ (10),
          load_ (1.0),
          ID_ (::CORBA::string_dup ("CIAO::RACE::Task"))
      {
      }

      Trigger_exec_i::~Trigger_exec_i (void)
      {
      }

      int
      Trigger_exec_i::svc ()
      {
        ACE_Time_Value interval (0, this->period_ * 1000);
        while (true)
          {
            ACE_DEBUG ((LM_DEBUG, "\n%s::Trigger: Pushing test event\n",
                        this->ID_.in ()));

            Test_var ev = new OBV_CIAO::RACE::Test;
            ev->load (this->load_);
            ev->task_ID (::CORBA::string_dup (this->ID_.in ()));

            try
              {
                this->context_->push_test_out (ev);
              }
            catch (CORBA::Exception &ex)
              {
                ACE_PRINT_EXCEPTION (ex, "Excaption caught!\n");
                return -1;
              }
            ACE_OS::sleep (interval);
          }
      }


      ::CORBA::Long
      Trigger_exec_i::period ()
      {
        return this->period_;
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
