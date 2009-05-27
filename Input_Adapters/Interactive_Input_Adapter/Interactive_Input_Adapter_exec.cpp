// $Id$
#include "Interactive_Input_Adapter_exec.h"
#include "ciao/CIAO_common.h"
#include "Admin_exec.h"
#include <sstream>

namespace CIAO
{
  namespace RACE
  {
    namespace Input_Adapter
    {
      namespace CIDL_Interactive_IA_Component_Impl
      {
        Interactive_IA_Component_exec_i::Interactive_IA_Component_exec_i ()
          : logger_ ("Interactive_IA.log")
        {
        }

        Interactive_IA_Component_exec_i::~Interactive_IA_Component_exec_i ()
        {
        }
        ::CIAO::RACE::Input_Adapter::CCM_Admin_ptr
        Interactive_IA_Component_exec_i::get_admin ()
        {
          std::stringstream msg;
          msg << "Interactive_IA_Component_exec_i::"
              << "Creating the admin object.\n";
          this->logger_.log (msg.str());
          return new Admin_exec_i (this->context_.in (), this->logger_);
        }

        void
        Interactive_IA_Component_exec_i::set_session_context (
          ::Components::SessionContext_ptr ctx)
        {
          this->context_ =
            ::CIAO::RACE::Input_Adapter::
            CCM_Interactive_IA_Component_Context::_narrow (ctx);

          if (CORBA::is_nil (this->context_.in ()))
            {
              throw ::CORBA::INTERNAL ();
            }
        }

        void
        Interactive_IA_Component_exec_i::ciao_preactivate ()
        {
        }

        void
        Interactive_IA_Component_exec_i::configuration_complete ()
        {
        }

        void
        Interactive_IA_Component_exec_i::ccm_activate ()
        {
        }

        void
        Interactive_IA_Component_exec_i::ccm_passivate ()
        {
        }

        void
        Interactive_IA_Component_exec_i::ccm_remove ()
        {
        }



        Interactive_IA_Component_Home_exec_i::
        Interactive_IA_Component_Home_exec_i (void)
        {
        }

      Interactive_IA_Component_Home_exec_i::
      ~Interactive_IA_Component_Home_exec_i (void)
        {
        }

        ::Components::EnterpriseComponent_ptr
        Interactive_IA_Component_Home_exec_i::create ()
        {
          ::Components::EnterpriseComponent_ptr retval =
            ::Components::EnterpriseComponent::_nil ();

          ACE_NEW_THROW_EX (
            retval,
            Interactive_IA_Component_exec_i,
            ::CORBA::NO_MEMORY ());

          return retval;
        }

        extern "C" INTERACTIVE_INPUT_ADAPTER_EXEC_Export
        ::Components::HomeExecutorBase_ptr
        create_CIAO_RACE_Input_Adapter_Interactive_IA_Component_Home_Impl (void)
        {
          ::Components::HomeExecutorBase_ptr retval =
            ::Components::HomeExecutorBase::_nil ();

          ACE_NEW_RETURN (
            retval,
            Interactive_IA_Component_Home_exec_i,
            ::Components::HomeExecutorBase::_nil ());

          return retval;
        }
      }
    }
  }
}
