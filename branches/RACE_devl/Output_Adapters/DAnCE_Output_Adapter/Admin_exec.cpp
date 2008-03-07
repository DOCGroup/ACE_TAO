#include "Admin_exec.h"
#include "ciao/CIAO_common.h"
#include "Config_Handlers/DP_Handler.h"
#include "Config_Handlers/Utils/XML_Helper.h"
#include "Config_Handlers/Deployment.hpp"
#include "Config_Handlers/DnC_Dump.h"
#include <sstream>

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
          const char * repoman_id,
          Logger &logger)
          : context_ (context),
            repoman_id_ (CORBA::string_dup (repoman_id)),
            repoman_ (false),
            logger_ (logger)
        {
          this->launcher_.init (
            0,
            this->context_->_ciao_the_Container ()->the_ORB ());

          std::string msg;

          if (this->repoman_id_.in () != 0)
            {
              msg = "Trying to initialize the RM PLANNER with ID: ";
              msg += this->repoman_id_.in();
              this->logger_.log (msg);


              if (this->plan_gen_.init (
                    this->context_->_ciao_the_Container ()->the_ORB (),
                    true,
                    this->repoman_id_.in ()))
                {
                  this->repoman_ = true;
                  msg = "Done!";
                  this->logger_.log (msg);
                }
              else
                {
                  msg = "DAnCE_OA::Admin_exec:: Error while "
                        "initializing the repoman plan_generator!";
                  this->logger_.log (msg);
                }
            }
          else {
            msg = "DAnCE_OA::Admin_exec:: No repoman ID provided "
                  "to constructor.\n";
            this->logger_.log (msg);
          }
        }

        Admin_exec_i::~Admin_exec_i (void)
        {
        }

        bool
        Admin_exec_i::deploy_plan (const ::Deployment::DeploymentPlan &plan)
        {
          try
            {
              // First let's dump the plan to a file.
              // using namespace CIAO::Config_Handlers;
              // DP_Handler reverse_handler (plan);
              // xercesc::DOMDocument *the_xsc (XML_HELPER->create_dom (0));
              // deploymentPlan (*reverse_handler.xsc (), the_xsc);
              // std::string dom_out (plan.UUID.in ());
              // dom_out += ".cdp";
              // XML_HELPER->write_DOM (the_xsc, dom_out.c_str());
              // delete the_xsc;
              // Now do the remaining stuff.
              //              Deployment::DnC_Dump::dump (plan);
              ACE_CString uuid (this->launcher_.launch_plan (plan));
              std::string msg;
              if (uuid.c_str () == 0)
                {
                  msg = "Unable to launch plan with UUID ";
                  msg += plan.UUID.in ();
                  this->logger_.log (msg);
                }
              else
                {
                  msg = "Plan ";
                  msg += plan.UUID.in ();
                  msg += " was deployed!";
                  this->logger_.log (msg);
                  return true;
                }

            }
          catch (Plan_Launcher::Plan_Launcher_i::Deployment_Failure &)
            {
              std::string msg = "Exception caught::Unable to "
                                "launch plan with UUID ";
              msg += plan.UUID.in ();
              this->logger_.log (msg);
            }
          return false;
        }

        bool
        Admin_exec_i::redeploy_plan (const ::Deployment::DeploymentPlan &plan)
        {
          try
            {
              ACE_CString uuid (this->launcher_.re_launch_plan (plan));

              if (uuid.c_str () == 0)
                {
                  std::string msg = "Unable to relaunch plan with UUID ";
                  msg += plan.UUID.in ();
                  this->logger_.log (msg);
                }
              else
                {
                  std::string msg = "Successfully relaunched plan with UUID ";
                  msg += plan.UUID.in ();
                  this->logger_.log (msg);

                  return true;
                }

            }
          catch (Plan_Launcher::Plan_Launcher_i::Deployment_Failure &)
            {
              std::string msg = "Exception caught::Unable to "
                                "relaunch plan with UUID ";
              msg += plan.UUID.in ();
              this->logger_.log (msg);
            }
          return false;
        }

        ::CORBA::Boolean
        Admin_exec_i::deploy_string
        (const ::CIAO::RACE::OperationalString & op_string)
        {
          std::string msg	= "In DANCE_OA::deploy string()";
          this->logger_.log (msg);


          ::Deployment::DeploymentPlan plan;

          if (this->opstring_to_dplan (op_string, plan))
            {
              return this->deploy_plan (plan);
            }
          else
            {
              msg = "deploy_string()::Failed to generate dp from opstring.";
              msg += plan.UUID.in ();

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
          std::string msg;
          /// We need the repo man to convert an operational string to a
          /// deployment plan if the artifact information is not available
          /// in the operational string. We first check if the repoman is
          /// avaialble. If not, we check if the artifact info is avaialble
          /// in the operational string structure. Else, we bail out.
          if (!this->repoman_)
            {
              msg = "DANCE_OA::opstring_to_dplan(): this->repoman_ not set";
              this->logger_.log (msg);
            }
          else if (op_string.implementation.length() == 0)
            {
              msg = "DANCE_OA::opstring_to_dplan(): opstring does not "
                    "conatin any info about the implemenations!"
                    "\nTherefore, bailing out!\n";
              this->logger_.log (msg);
              return false;
            }

          // Populate the label, UUID, connections, and info property
          // fields of the deployment plan.
          plan.label = op_string.name;
          plan.UUID = op_string.ID;
          plan.connection = op_string.dataLink;
          plan.infoProperty = op_string.infoProperty;

          /// If the opstring contains the artifact and implementation
          /// info, we copy them over directly. Else, we rely on the
          /// plan_generator to populate these fields.
          if (op_string.implementation.length() != 0 &&
              op_string.artifact.length() != 0)
            {
              plan.implementation = op_string.implementation;
              plan.artifact = op_string.artifact;
              /// Now copy over each instance description.
              for (CORBA::ULong itr = 0;
                   itr < op_string.instance.length (); ++itr)
                {

                  ::CIAO::RACE::InstanceDescription op_instance =
                    op_string.instance [itr];
                  ::Deployment::InstanceDeploymentDescription instance;
                  instance.name = op_instance.name;
                  instance.node = op_instance.node;
                  instance.implementationRef = op_instance.implementationRef;
                  instance.configProperty = op_instance.configProperty;
                  CORBA::ULong cur_len = plan.instance.length ();
                  plan.instance.length (cur_len+1);
                  plan.instance [cur_len] = instance;

                }
            }

          else
            {
              /// Now, populate the instance, implementation, and artifact
              /// fields of the deployment plan using the plan generator.
              int position;
              for (CORBA::ULong itr = 0;
                   itr < op_string.instance.length (); ++itr)
                {

                  ::CIAO::RACE::InstanceDescription op_instance =
                    op_string.instance [itr];
                  if (this->plan_gen_.generate_plan
                      (plan,
                       op_instance.suggestedImpl.in (),
                       position))
                    {
                      ::Deployment::InstanceDeploymentDescription instance;
                      instance.name = op_instance.name;

                      // @@ This is a temporaty hack. Later on we must be
                      // using the simple bin packer do perform the node
                      // assignment.
                      instance.node = op_instance.node;
                      //instance.node = CORBA::string_dup("SPACE");
                      instance.implementationRef = position;
                      instance.configProperty = op_instance.configProperty;
                      CORBA::ULong cur_len = plan.instance.length ();
                      plan.instance.length (cur_len+1);
                      plan.instance [cur_len] = instance;

                    }
                  else
                    {
                      msg = "Given suggested type is not "
                            "available in the Repoman!!\n Bailing out....";
                      this->logger_.log (msg);
                      return false;
                    }
                }
            }

          return true;
        }

        ::CORBA::Boolean
        Admin_exec_i::tear_down_string (const char *ID )
        {
          std::stringstream msg;
          try
            {
              if (this->launcher_.teardown_plan(ID))
                {
                  msg << "DANCE_OA::tear_down_string(): "
                      <<   "successfully torn down plan with UUID "
                      << ID
                      << std::endl;
                  this->logger_.log (msg.str());
                  return true;
                }
              else
                {
                  msg << "DANCE_OA::tear_down_string():Oops! "
                      << "Error while tearing down plan with UUID: "
                      << ID
                      << std::endl;;
                  this->logger_.log (msg.str());

                }
            }
          catch (CORBA::Exception &ex)
            {
              msg << "DANCE_OA::tear_down_string():Exeption caught ";
              msg << ex._info().c_str();
              this->logger_.log (msg.str());
            }
          return false;
        }

      }
    }
  }
}
