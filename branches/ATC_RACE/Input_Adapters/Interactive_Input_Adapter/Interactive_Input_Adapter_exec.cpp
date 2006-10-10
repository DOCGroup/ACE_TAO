// $Id$
#include "Interactive_Input_Adapter_exec.h"
#include "ciao/CIAO_common.h"
#include "Config_Handlers/XML_File_Intf.h"
#include "Config_Handlers/Package_Handlers/PCD_Handler.h"
#include "Config_Handlers/Common.h"

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_Interactive_Input_Adapter_Impl
    {
      Interactive_Input_Adapter_exec_i::Interactive_Input_Adapter_exec_i (void)
      {
      }

      Interactive_Input_Adapter_exec_i::~Interactive_Input_Adapter_exec_i (void)
      {
      }

      void
      Interactive_Input_Adapter_exec_i::push_deployment
      (::CIAO::RACE::Deploy_Input * ev)
        throw (CORBA::SystemException)
      {
	try
	  {
	    ACE_Auto_Ptr < ::Deployment::DeploymentPlan > dp;
	    ACE_Auto_Ptr < ::Deployment::PackageConfiguration > pcd;

	    if (ev->plan_uri () != 0 || ev->plan_uri () != "")
	      {
		ACE_DEBUG ((LM_DEBUG, "Opening the plan\n"));

		Config_Handlers::XML_File_Intf xfi (ev->plan_uri ());

		dp.reset ( xfi.get_plan ());
	      }
	    else
              {
                dp.reset ( new ::Deployment::DeploymentPlan );
              }


	    pcd.reset (new ::Deployment::PackageConfiguration);

	    if ((ACE_OS::strcmp (ev->package_uri (), "") != 0))
	      {
		ACE_DEBUG ((LM_DEBUG, "Opening the package descriptor: :%s:\n",
			    ev->package_uri ()));
		Config_Handlers::Packaging::PCD_Handler::package_config
                  (ev->package_uri (), *pcd);
	      }

            ::CIAO::RACE::Plan_Actions plan_action_seq;

            /// Create a Plan_Action_seq structure of length 1.
            plan_action_seq.length (1);

            /// Now populate the Plan_Action_seq structure.
            plan_action_seq [0].command = ev->command ();
            plan_action_seq [0].plan = *dp;
            plan_action_seq [0].package = *pcd;

            /// Now invoke the plan_analyzer.
	    this->context_->get_connection_ingress  ()
	      ->analyze_plan (plan_action_seq);
	  }
        catch (CIAO::Config_Handlers::Config_Error &ex)
          {
            ACE_ERROR ((LM_ERROR, "Config_Error exception caught in "
                        "Interactive_Input_Adapter::push_deployment: %s, %s\n",
                        ex.error_.c_str (),
                        ex.name_.c_str ()));
          }
        catch (...)
	  {
	    ACE_ERROR ((LM_ERROR, "Exception caught in push_deployment\n"));
	  }
      }

      void
      Interactive_Input_Adapter_exec_i::push_meta_data
      (::CIAO::RACE::Metadata * ev)
        throw (CORBA::SystemException)
      {

        ACE_DEBUG ((LM_DEBUG, "In push_meta_data\n"));
        if (ACE_OS::strcmp (ev->plan ().UUID.in (), "") != 0)
          {
            /// Now populate the Plan_Action_seq structure.
            ::CIAO::RACE::Plan_Actions plan_action_seq;

            /// Create a Plan_Action_seq structure of length 1.
            plan_action_seq.length (1);

            plan_action_seq [0].command = ev->command ();
            plan_action_seq [0].plan = ev->plan ();
            try
              {
                /// Now invoke the plan_analyzer.
                this->context_->get_connection_ingress  ()
                  ->analyze_plan (plan_action_seq);
              }
            catch (CORBA::Exception &ex)
              {
                ACE_PRINT_EXCEPTION
                  (ex, "Exception caught in "
                   "Interactive_Input_Adapter_exec_i::push_meta_data");
              }
            catch (...)
              {
                ACE_ERROR ((LM_ERROR, "Exception caught in push_mata_data\n"));
              }
          }
      }

      // Operations from Components::SessionComponent

      void
      Interactive_Input_Adapter_exec_i::set_session_context (
        ::Components::SessionContext_ptr ctx
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        this->context_ =
        Interactive_Input_Adapter_Context::_narrow (
          ctx
          ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        if (this->context_ == 0)
        {
          ACE_THROW (CORBA::INTERNAL ());
        }
      }

      void
      Interactive_Input_Adapter_exec_i::ciao_preactivate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      void
      Interactive_Input_Adapter_exec_i::ciao_postactivate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      void
      Interactive_Input_Adapter_exec_i::ccm_activate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      void
      Interactive_Input_Adapter_exec_i::ccm_passivate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      void
      Interactive_Input_Adapter_exec_i::ccm_remove (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      //==================================================================
      // Home Executor Implementation Class:   Interactive_Input_Adapter_Home_exec_i
      //==================================================================

      Interactive_Input_Adapter_Home_exec_i::Interactive_Input_Adapter_Home_exec_i (void)
      {
      }

      Interactive_Input_Adapter_Home_exec_i::~Interactive_Input_Adapter_Home_exec_i (void)
      {
      }

      // Supported or inherited operations.

      // Home operations.

      // Factory and finder operations.

      // Attribute operations.

      // Implicit operations.

      ::Components::EnterpriseComponent_ptr
      Interactive_Input_Adapter_Home_exec_i::create (
        ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        ::Components::EnterpriseComponent_ptr retval =
        ::Components::EnterpriseComponent::_nil ();

        ACE_NEW_THROW_EX (
          retval,
          Interactive_Input_Adapter_exec_i,
          CORBA::NO_MEMORY ());
        ACE_CHECK_RETURN (::Components::EnterpriseComponent::_nil ());

        return retval;
      }

      extern "C" INTERACTIVE_INPUT_ADAPTER_EXEC_Export ::Components::HomeExecutorBase_ptr
      create_CIAO_RACE_Interactive_Input_Adapter_Home_Impl (void)
      {
        ::Components::HomeExecutorBase_ptr retval =
        ::Components::HomeExecutorBase::_nil ();

        ACE_NEW_RETURN (
          retval,
          Interactive_Input_Adapter_Home_exec_i,
          ::Components::HomeExecutorBase::_nil ());

        return retval;
      }
    }
  }
}
