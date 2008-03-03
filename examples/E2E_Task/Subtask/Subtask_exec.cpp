#include "Subtask_exec.h"
#include "ciao/CIAO_common.h"
#include "RT.h"

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_Subtask
    {
      Subtask_exec_i::Subtask_exec_i (void)
        :iterations_ (1000),
         ID_ (::CORBA::string_dup ("CIAO::RACE::Subtaks"))
      {
        // Ser RT priority.
        set_priority ();

      }

      Subtask_exec_i::~Subtask_exec_i (void)
      {
      }

      ::CORBA::Long
      Subtask_exec_i::iterations ()
      {
        return this->iterations_;
      }

      void
      Subtask_exec_i::iterations (::CORBA::Long iterations)
      {
        if (iterations > 0)
          {
            this->iterations_ = iterations;
          }
      }

      void
      Subtask_exec_i::ID (const char *ID)
      {
        if (ID)
          {
            this->ID_ = ::CORBA::string_dup (ID);
          }
      }

      char*
      Subtask_exec_i::ID ()
      {
        return ::CORBA::string_dup (this->ID_.in ());
      }


      void
      Subtask_exec_i::push_test_in (
        ::CIAO::RACE::Test *ev )
      {
        size_t load = static_cast <size_t> (this->iterations_ * ev->load ());

        for (size_t i = 0; i < load; ++i)
          {
            ACE::is_prime (16127, 2, 16127 / 2);
          }
//         ACE_DEBUG ((LM_DEBUG, "%s::%s: Data processing complete.\n",
//                     ev->task_ID (),
//                     this->ID_.in ()));
        this->context_->push_test_out (ev);
      }

      // Operations from Components::SessionComponent

      void
      Subtask_exec_i::set_session_context (
        ::Components::SessionContext_ptr ctx)
      {
        this->context_ =
          ::CIAO::RACE::CCM_Subtask_Context::_narrow (ctx);

        if (CORBA::is_nil (this->context_.in ()))
        {
          throw ::CORBA::INTERNAL ();
        }
      }

      void
      Subtask_exec_i::ciao_preactivate ()
      {
        // Your code here.
      }

      void
      Subtask_exec_i::ciao_postactivate ()
      {
        // Your code here.
      }

      void
      Subtask_exec_i::ccm_activate ()
      {
        // Your code here.
      }

      void
      Subtask_exec_i::ccm_passivate ()
      {
        // Your code here.
      }

      void
      Subtask_exec_i::ccm_remove ()
      {
        // Your code here.
      }

      //==================================================================
      // Home Executor Implementation Class:   Subtask_Factory_exec_i
      //==================================================================

      Subtask_Factory_exec_i::Subtask_Factory_exec_i (void)
      {
      }

      Subtask_Factory_exec_i::~Subtask_Factory_exec_i (void)
      {
      }

      // Supported or inherited operations.

      // Home operations.

      // Factory and finder operations.

      // Attribute operations.

      // Implicit operations.

      ::Components::EnterpriseComponent_ptr
      Subtask_Factory_exec_i::create ()
      {
        ::Components::EnterpriseComponent_ptr retval =
          ::Components::EnterpriseComponent::_nil ();

        ACE_NEW_THROW_EX (
          retval,
          Subtask_exec_i,
          ::CORBA::NO_MEMORY ());

        return retval;
      }

      extern "C" SUBTASK_EXEC_Export ::Components::HomeExecutorBase_ptr
      create_CIAO_RACE_Subtask_Factory_Impl (void)
      {
        ::Components::HomeExecutorBase_ptr retval =
          ::Components::HomeExecutorBase::_nil ();

        ACE_NEW_RETURN (
          retval,
          Subtask_Factory_exec_i,
          ::Components::HomeExecutorBase::_nil ());

        return retval;
      }
    }
  }
}
