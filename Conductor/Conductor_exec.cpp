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

        ::CORBA::Boolean
        Conductor_Component_exec_i::deploy_plan (const char * uri)
        {

          std::stringstream msg;
          msg << "Conductor_Component_exec_i:: Entering deploy_plan.\n";          
          auto_ptr < ::Deployment::DeploymentPlan > dp;
          Config_Handlers::XML_File_Intf xfi (uri);
          dp.reset (xfi.get_plan ());
          try
            {
              DAnCE_OA::Admin_var OA = this->context_->get_connection_OA ();
              OA->deploy_plan (*dp);

              msg << "Trying to initialize the controller.\n";
              if (this->context_->get_connection_controllerAdmin()->init_controller())
              {
                msg << "Successfully initialized the controller.\n";
              }
              else
              {
                msg << "Error while initializing the controller.\n";
              }

              msg << "Now trying to start the controller.\n";
              if (this->context_->get_connection_controllerAdmin()->start_controller())
              {
                msg << "Successfully started the controller.\n";
              }
              else
              {
                msg << "Error while starting the controller.\n";
              }

              msg << "Conductor_Component_exec_i::Leaving deploy_plan.";
              this->logger_.log (msg.str());
              return true;
            }
          catch (CORBA::Exception &ex)
            {
              msg << "Exception caught::Conductor_Component_exec_i::deploy_plan.";
              msg << ex._info ().c_str();
              this->logger_.log (msg.str());
              return false;
            }
        }

        ::CORBA::Boolean
        Conductor_Component_exec_i::redeploy_plan (const char *  uri)
        {
          std::string msg = "Conductor_Component_exec_i::"
                            "Entering redeploy_plan.";
          this->logger_.log (msg);

          auto_ptr < ::Deployment::DeploymentPlan > dp;
          Config_Handlers::XML_File_Intf xfi (uri);
          dp.reset (xfi.get_plan ());
          try
            {
              DAnCE_OA::Admin_var OA = this->context_->get_connection_OA ();
              OA->redeploy_plan (*dp);
              std::string msg = "Conductor_Component_exec_i::"
                                "Leaving redeploy_plan.";
              this->logger_.log (msg);
              return true;
            }
          catch (CORBA::Exception &ex)
            {
              std::string msg = "Exception caught::"
                                "Conductor_Component_exec_i::redeploy_plan.";
              msg += ex._info ().c_str();
              this->logger_.log (msg);
              return false;
            }
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
        Conductor_Component_exec_i::ciao_postactivate ()
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
