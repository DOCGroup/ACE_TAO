#include "Conductor_exec.h"
#include "ciao/CIAO_common.h"

#include "Admin_exec.h"
#include "Config_Handlers/XML_File_Intf.h"
#include "Config_Handlers/Package_Handlers/PCD_Handler.h"
#include "Config_Handlers/Common.h"
#include <sstream>

namespace CIAO
{
  namespace RACE
  {
    namespace Conductor
    {
      namespace CIDL_Conductor_Component_Impl
      {
        Conductor_Component_exec_i::Conductor_Component_exec_i (void)
          : logger_ ("Conductor.log")
        {
        }

        Conductor_Component_exec_i::~Conductor_Component_exec_i (void)
        {
        }

        ::CIAO::RACE::Conductor::CCM_Admin_ptr
        Conductor_Component_exec_i::get_admin ()
        {
          std::string msg = "Conductor_Component_exec_i::"
                            "Creating the admin object.\n";
          this->logger_.log (msg);          
          return (new Admin_exec_i (this->context_, this->logger_));
        }

        // Operations from Components::SessionComponent

        void
        Conductor_Component_exec_i::set_session_context (
          ::Components::SessionContext_ptr ctx)
        {
          this->context_ =
            ::CIAO::RACE::Conductor::
            CCM_Conductor_Component_Context::_narrow (ctx);

          if (CORBA::is_nil (this->context_.in ()))
          {
            throw ::CORBA::INTERNAL ();
          }
        }

        void
        Conductor_Component_exec_i::ciao_preactivate ()
        {
          // Your code here.
        }

        void
        Conductor_Component_exec_i::configuration_complete ()
        {
          // Your code here.
        }

        void
        Conductor_Component_exec_i::ccm_activate ()
        {
          // Your code here.
        }

        void
        Conductor_Component_exec_i::ccm_passivate ()
        {
          // Your code here.
        }

        void
        Conductor_Component_exec_i::ccm_remove ()
        {
          // Your code here.
        }

        Conductor_Component_Home_exec_i::Conductor_Component_Home_exec_i ()
        {
        }

        Conductor_Component_Home_exec_i::~Conductor_Component_Home_exec_i ()
        {
        }

        ::Components::EnterpriseComponent_ptr
        Conductor_Component_Home_exec_i::create ()
        {
          ::Components::EnterpriseComponent_ptr retval =
            ::Components::EnterpriseComponent::_nil ();

          ACE_NEW_THROW_EX (
            retval,
            Conductor_Component_exec_i,
            ::CORBA::NO_MEMORY ());

          return retval;
        }

        extern "C" CONDUCTOR_EXEC_Export ::Components::HomeExecutorBase_ptr
        create_CIAO_RACE_Conductor_Conductor_Component_Home_Impl (void)
        {
          ::Components::HomeExecutorBase_ptr retval =
            ::Components::HomeExecutorBase::_nil ();

          ACE_NEW_RETURN (
            retval,
            Conductor_Component_Home_exec_i,
            ::Components::HomeExecutorBase::_nil ());

          return retval;
        }
      }
    }
  }
}
