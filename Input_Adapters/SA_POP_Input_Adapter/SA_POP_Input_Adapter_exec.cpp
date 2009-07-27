#include "SA_POP_Input_Adapter_exec.h"
#include "ciao/CIAO_common.h"

#include "Admin_exec.h"
#include <string>

namespace CIAO
{
  namespace RACE
  {
    namespace Input_Adapter
    {
      namespace CIDL_SA_POP_IA_Component_Impl
      {
        SA_POP_IA_Component_exec_i::SA_POP_IA_Component_exec_i (void)
          : logger_ ("SA_POP_IA.log")
        {
        }

        SA_POP_IA_Component_exec_i::~SA_POP_IA_Component_exec_i (void)
        {
        }

        ::CIAO::RACE::Input_Adapter::CCM_Admin_ptr
        SA_POP_IA_Component_exec_i::get_admin ()
        {
          std::string msg = "SA_POP_IA_Component_exec_i::"
                            "Creating the admin object.\n";
          this->logger_.log (msg);
          return (new Admin_exec_i (this->context_, this->logger_));
        }

        void
        SA_POP_IA_Component_exec_i::set_session_context
        (::Components::SessionContext_ptr ctx)
        {
          this->context_ =
            CIAO_CIAO_RACE_Input_Adapter_SA_POP_IA_Component_Impl::SA_POP_IA_Component_Context::_narrow (ctx);

          if (this->context_ == 0)
            {
              throw ::CORBA::INTERNAL ();
            }
        }

        void
        SA_POP_IA_Component_exec_i::ciao_preactivate ()
        {
        }

        void
        SA_POP_IA_Component_exec_i::configuration_complete ()
        {
        }

        void
        SA_POP_IA_Component_exec_i::ccm_activate ()
        {
        }

        void
        SA_POP_IA_Component_exec_i::ccm_passivate ()
        {
        }

        void
        SA_POP_IA_Component_exec_i::ccm_remove ()
        {
        }


        SA_POP_IA_Component_Home_exec_i::SA_POP_IA_Component_Home_exec_i ()
        {
        }

        SA_POP_IA_Component_Home_exec_i::~SA_POP_IA_Component_Home_exec_i ()
        {
        }
        ::Components::EnterpriseComponent_ptr
        SA_POP_IA_Component_Home_exec_i::create ()
        {
          ::Components::EnterpriseComponent_ptr retval =
            ::Components::EnterpriseComponent::_nil ();

          ACE_NEW_THROW_EX (
            retval,
            SA_POP_IA_Component_exec_i,
            ::CORBA::NO_MEMORY ());

          return retval;
        }

        extern "C" SA_POP_INPUT_ADAPTER_EXEC_Export ::Components::
        HomeExecutorBase_ptr
        create_CIAO_RACE_Input_Adapter_SA_POP_IA_Component_Home_Impl (void)
        {
          ::Components::HomeExecutorBase_ptr retval =
            ::Components::HomeExecutorBase::_nil ();

          ACE_NEW_RETURN (
            retval,
            SA_POP_IA_Component_Home_exec_i,
            ::Components::HomeExecutorBase::_nil ());

          return retval;
        }
      }
    }
  }
}
