#include "DAnCE_Output_Adapter_exec.h"
#include "ciao/CIAO_common.h"


namespace CIAO
{
  namespace RACE
  {
    namespace DAnCE_OA
    {
      namespace CIDL_DAnCE_OA_Impl
      {
        Admin_exec_i::Admin_exec_i (DAnCE_OA_Component_Context *context)
          : context_ (context)
        {
          this->launcher_.init (0, this->context_->_ciao_the_Container ()->the_ORB ());
        }

        Admin_exec_i::~Admin_exec_i (void)
        {
        }

        ::CORBA::Boolean
        Admin_exec_i::deploy_plan (const ::Deployment::DeploymentPlan &plan)
        {
          try
            {
              ACE_CString uuid (this->launcher_.launch_plan (plan));

              if (uuid.c_str () == 0)
                {
                  ACE_ERROR ((LM_ERROR, "Unable to launch plan with UUID %s\n",
                              plan.UUID.in ()));
                }
              else
                {
                  ACE_DEBUG ((LM_DEBUG, "Plan %s deployed!\n", plan.UUID.in ()));
                  return true;
                }

            }
          catch (Plan_Launcher::Plan_Launcher_i::Deployment_Failure &ex)
            {
              ACE_ERROR ((LM_ERROR, "Unable to deploy plan %s.\n", plan.UUID.in ()));
            }
          return false;
        }

        ::CORBA::Boolean
        Admin_exec_i::deploy_string
        (const ::CIAO::RACE::OperationalString & /* opstring */)
        {
          // Your code here.
          return false;
        }

        ::CORBA::Boolean
        Admin_exec_i::tear_down_string (const char *ID )
        {
          try
            {
              if (this->launcher_.teardown_plan(ID))
                {
                  ACE_DEBUG ((LM_DEBUG, "Plan with UUID: %s has been "
                              "successfully torn down\n"));
                  return true;
                }
              else
                {
                  ACE_DEBUG ((LM_DEBUG, "Oops! Error while tearing down plan "
                              "with UUID: %s\n"));
                }
            }
          catch (CORBA::Exception &ex)
            {
              ACE_PRINT_EXCEPTION (ex, "Exeption caught in "
                                   "Admin_exec_i::teardown_string:\n");
            }
          return false;
        }

        DAnCE_OA_Component_exec_i::DAnCE_OA_Component_exec_i (void)
        {
        }

        DAnCE_OA_Component_exec_i::~DAnCE_OA_Component_exec_i (void)
        {
        }


        ::CIAO::RACE::DAnCE_OA::CCM_Admin_ptr
        DAnCE_OA_Component_exec_i::get_admin ()
        {
          return (new Admin_exec_i (this->context_));
        }

        void
        DAnCE_OA_Component_exec_i::set_session_context (
          ::Components::SessionContext_ptr ctx)
        {
          this->context_ = DAnCE_OA_Component_Context::_narrow (ctx);

          if (this->context_ == 0)
          {
            throw ::CORBA::INTERNAL ();
          }
        }

        void
        DAnCE_OA_Component_exec_i::ciao_preactivate ()
        {
          // Your code here.
        }

        void
        DAnCE_OA_Component_exec_i::ciao_postactivate ()
        {
          // Your code here.
        }

        void
        DAnCE_OA_Component_exec_i::ccm_activate ()
        {
          // Your code here.
        }

        void
        DAnCE_OA_Component_exec_i::ccm_passivate ()
        {
          // Your code here.
        }

        void
        DAnCE_OA_Component_exec_i::ccm_remove ()
        {
          // Your code here.
        }

        DAnCE_OA_Component_Home_exec_i::DAnCE_OA_Component_Home_exec_i (void)
        {
        }

        DAnCE_OA_Component_Home_exec_i::~DAnCE_OA_Component_Home_exec_i (void)
        {
        }

        ::Components::EnterpriseComponent_ptr
        DAnCE_OA_Component_Home_exec_i::create ()
        {
          ::Components::EnterpriseComponent_ptr retval =
            ::Components::EnterpriseComponent::_nil ();

          ACE_NEW_THROW_EX (
            retval,
            DAnCE_OA_Component_exec_i,
            ::CORBA::NO_MEMORY ());

          return retval;
        }

        extern "C" DANCE_OUTPUT_ADAPTER_EXEC_Export ::Components::HomeExecutorBase_ptr
        create_CIAO_RACE_DAnCE_OA_DAnCE_OA_Component_Home_Impl (void)
        {
          ::Components::HomeExecutorBase_ptr retval =
            ::Components::HomeExecutorBase::_nil ();

          ACE_NEW_RETURN (
            retval,
            DAnCE_OA_Component_Home_exec_i,
            ::Components::HomeExecutorBase::_nil ());

          return retval;
        }
      }
    }
  }
}
