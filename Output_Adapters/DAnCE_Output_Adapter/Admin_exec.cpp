#include "Admin_exec.h"
#include "ciao/CIAO_common.h"
#include "Config_Handlers/DP_Handler.h"
#include "Config_Handlers/Utils/XML_Helper.h"
#include "Config_Handlers/Deployment.hpp"
#include <string>

namespace CIAO
{
  namespace RACE
  {
    namespace DAnCE_OA
    {
      namespace CIDL_DAnCE_OA_Impl
      {
        Admin_exec_i::Admin_exec_i (
          DAnCE_OA_Component_Context *context,
          const char * repoman_id)
          : context_ (context),
            repoman_id_ (CORBA::string_dup (repoman_id)),
            repoman_ (false)
        {
          this->launcher_.init (
            0,
            this->context_->_ciao_the_Container ()->the_ORB ());

          if (this->repoman_id_.in () != 0)
            {
              ACE_ERROR ((LM_ERROR, "Trying to initialize the RM PLANNER with ID %s!!!!!!\n",
                this->repoman_id_.in()));

              if (this->plan_gen_.init (
                    this->context_->_ciao_the_Container ()->the_ORB (),
                    true,
                    this->repoman_id_.in ()))
              {
                this->repoman_ = true;
                ACE_ERROR ((LM_ERROR, "DONE\n"));
              }
              else
                {
                  ACE_ERROR ((LM_ERROR, "DAnCE_OA::Admin_exec:: "
                              "Error while initializing the repoman plan_generator!\n"));
                }
            }
          else {
                  ACE_ERROR ((LM_ERROR, "DAnCE_OA::Admin_exec:: No repoman ID provided to constructor\n"));
          }
        }

        Admin_exec_i::~Admin_exec_i (void)
        {
        }

        ::CORBA::Boolean
        Admin_exec_i::deploy_plan (const ::Deployment::DeploymentPlan &plan)
        {
          try
            {


              // First let's dump the plan to a file.

              using namespace CIAO::Config_Handlers;

              DP_Handler reverse_handler (plan);
              xercesc::DOMDocument *the_xsc (XML_HELPER->create_dom (0));

              deploymentPlan (*reverse_handler.xsc (), the_xsc);

              std::string dom_out (plan.UUID.in ());
              dom_out += ".cdp";
              XML_HELPER->write_DOM (the_xsc, dom_out.c_str());

              delete the_xsc;

              // Now do the remaining stuff.

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
        Admin_exec_i::redeploy_plan (const ::Deployment::DeploymentPlan &plan)
        {
          try
            {
              ACE_CString uuid (this->launcher_.re_launch_plan (plan));

              if (uuid.c_str () == 0)
                {
                  ACE_ERROR ((LM_ERROR, "Unable to relaunch plan with UUID %s\n",
                              plan.UUID.in ()));
                }
              else
                {
                  ACE_DEBUG ((LM_DEBUG, "Plan %s redeployed!\n", plan.UUID.in ()));
                  return true;
                }

            }
          catch (Plan_Launcher::Plan_Launcher_i::Deployment_Failure &ex)
            {
              ACE_ERROR ((LM_ERROR, "Unable to redeploy plan %s.\n", plan.UUID.in ()));
            }
          return false;
        }

        ::CORBA::Boolean
        Admin_exec_i::deploy_string
        (const ::CIAO::RACE::OperationalString & op_string)
        {
          ACE_DEBUG ((LM_DEBUG, "In DANCE_OA::deploy string()\n"));

          ::Deployment::DeploymentPlan plan;

          if (this->opstring_to_dplan (op_string, plan))
            {
              return this->deploy_plan (plan);
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG, "DANCE_OA::deploy_string(): Failed to generate dp from opstring\n"));
              return false;
            }

          return false;
        }


        ::CORBA::Boolean
        Admin_exec_i::redeploy_string
        (const ::CIAO::RACE::OperationalString & op_string)
        {
          ::Deployment::DeploymentPlan plan;

          if (this->opstring_to_dplan (op_string, plan))
            {
              return this->redeploy_plan (plan);
            }
          else
            {
              return false;
            }
        }

        bool
        Admin_exec_i::opstring_to_dplan (
          const ::CIAO::RACE::OperationalString &op_string,
          ::Deployment::DeploymentPlan &plan)
        {
          /// We need the repo man to convert an operational string to a
          /// deployment plan since the artifact information is not
          /// available in the operational string. If the repo man is not
          /// available, we return false indicating that the string was not
          /// deployed.
          if (!this->repoman_)
            {
              ACE_DEBUG ((LM_DEBUG, "DANCE_OA::opstring_to_dplan(): this->repoman_ not set\n"));
              return false;
            }

          // Populate the label, UUID, connections, and info property fields of
          // the deployment plan.
          plan.label = op_string.name;
          plan.UUID = op_string.ID;
          plan.connection = op_string.dataLink;
          plan.infoProperty = op_string.infoProperty;

          // Now, populate the instance, implementation, and artifact fields of
          // the deployment plan using the plan generator.
          int position;
          for (CORBA::ULong itr = 0; itr < op_string.instance.length (); ++itr)
            {

              ::CIAO::RACE::InstanceDescription op_instance = op_string.instance [itr];
              if (this->plan_gen_.generate_plan
                  (plan,
                   op_instance.suggestedImpl.in (),
                   position))
                {
                  ::Deployment::InstanceDeploymentDescription instance;
                  instance.name = op_instance.name;

                  // @@ This is a temporaty hack. Later on we must be using the
                  // simple bin packer do perform the node assignment.
                  instance.node = op_instance.node;

                  instance.node = CORBA::string_dup("SPACE");
                  instance.implementationRef = position;
                  instance.configProperty = op_instance.configProperty;
                  CORBA::ULong cur_len = plan.instance.length ();
                  plan.instance.length (cur_len+1);
                  plan.instance [cur_len] = instance;

                }
              else
                {
                  ACE_ERROR ((LM_ERROR, "Given suggested type is not available "
                              "in the Repoman!!\n Bailing out....\n"));
                  return false;
                }
            }
          return true;
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

      }
    }
  }
}
