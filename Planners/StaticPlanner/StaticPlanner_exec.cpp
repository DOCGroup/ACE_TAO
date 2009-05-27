#include "StaticPlanner_exec.h"
#include "ciao/CIAO_common.h"
#include "ciao/Deployment_DataC.h"
#include "common/RACE_Log_Msg.h"

#define cpu_resource_name  "Processor"
#define cpu_resource_type  "CPULoad"
#define cpu_resource_prop_name  "LoadAverage"

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_StaticPlanner_Impl
    {
      //==================================================================
      // Facet Executor Implementation Class:   Planner_I_exec_i
      //==================================================================

      Planner_I_exec_i::Planner_I_exec_i (StaticPlanner_exec_i &comp)
        : comp_ (comp)
      {
      }

      Planner_I_exec_i::~Planner_I_exec_i (void)
      {
      }

      // Operations from ::CIAO::RACE::Planner_I

      ::CORBA::Boolean
      Planner_I_exec_i::process_plan (
        ::CIAO::RACE::Plan_Actions &plan_seq
        ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( ::CORBA::SystemException,
                        ::CIAO::RACE::PlannerFailure))
      {
	ACE_DEBUG ((LM_DEBUG, "Entering StaticPlanner\n"));
        try
          {

            // check here if the plan is for teardown and it has previously been deployed
            if (plan_seq[0].command == CIAO::RACE::TEARDOWN)
              if (deployed_plans_.find (std::string (plan_seq[0].plan.UUID))
                  == deployed_plans_.end ())
                return false; // I dont have any other suitable exception to throw ...

            // there will only be one plan ...
            ::Deployment::DeploymentPlan plan = plan_seq [0].plan;

            for (CORBA::ULong i = 0; i < plan.instance.length (); ++i)
              {
                // Get the monolithic implementation that corresponds to
                // this instance....
                CORBA::ULong pos = plan.instance[i].implementationRef;
                ::Deployment::MonolithicDeploymentDescription &mono =
                    plan.implementation[pos];

                for (CORBA::ULong j = 0; j < mono.execParameter.length (); ++j)
                  {
                    if (ACE_OS::strcmp (mono.execParameter[j].name.in (),
                                        cpu_resource_prop_name) == 0)
                      {
                        // Found an interesting property...
                        CORBA::Long value;
                        mono.execParameter[j].value >>= value;

                        // Build an IRDD
                        Deployment::InstanceResourceDeploymentDescription irdd;
                        irdd.resourceUsage = Deployment::InstanceUsesResource;
                        irdd.requirementName = cpu_resource_name;
                        irdd.resourceName = cpu_resource_type;
                        irdd.property.length (1);

                        irdd.property[0].name = cpu_resource_prop_name;
                        irdd.property[0].value <<= value;

                        // Append onto the instance
                        pos = plan.instance[i].deployedResource.length ();
                        plan.instance[i].deployedResource.length (pos + 1);
                        plan.instance[i].deployedResource[pos] = irdd;
                      }
                  }
              }

            // Reserve resource with the target manager, throwing a failure exception
            // if they are not available.
            ::Deployment::TargetManager_var tgt_mgr = comp_.context_->get_connection_target_mgr ();


            // check if plan is for teardown or deploy
            if (plan_seq[0].command == CIAO::RACE::DEPLOY)
              tgt_mgr->commitResources (plan);
            else // else , teardown , so release resources
              tgt_mgr->releaseResources (plan);

          }
        catch (::Deployment::ResourceNotAvailable &ex)
          {
            ACE_ERROR ((LM_ERROR, "ResourceNotAvailable exception caught:\n"
                            "\tName: %s\n"
                            "\tType: %s\n"
                            "\tProperty Name: %s\n"
                            "\tElementName: %s\n"
                            "\tResourceName: %s\n",
                            ex.name.in (), ex.resourceType.in (),
                            ex.propertyName.in (), ex.elementName.in (),
                            ex.resourceName.in ()));

	    ACE_CString error ("Planner failed due to exhaustion of ");
	    error += ex.resourceName.in ();
	    error += " resource on ";
	    error += ex.elementName.in ();

            throw PlannerFailure ("StaticPlanner", error.c_str ());
          }
        catch (...)
          {
            throw PlannerFailure ("StaticPlanner", "Planning failed for an unknown reason");
          }

        // if plan was for deploy .. add it to a list , else delete it

        if (plan_seq[0].command == CIAO::RACE::DEPLOY)
        {
           DeployedPlanRecord deprec;
           deprec.importance = Log::find_importance( plan_seq[0].package );
           deprec.plan = plan_seq[0].plan;

           deployed_plans_[std::string (plan_seq[0].plan.UUID)] = deprec;
        }
        else
            deployed_plans_.erase( std::string(plan_seq[0].plan.UUID) );


        return true;
      }


      ::CORBA::Boolean Planner_I_exec_i::process_domain_change (
                const ::CIAO::RACE::Planner_I::Domain_Changes & changes,
                ::CIAO::RACE::Plan_Actions_out plans
                ACE_ENV_ARG_DECL_NOT_USED)
          ACE_THROW_SPEC (( ::CORBA::SystemException))
      {

        for ( CORBA::ULong i = 0; i < changes.length(); i++ )
        {
          ACE_DEBUG(( LM_DEBUG, "StaticPlanner::DomainChange - Host [%s] %s.\n",
              changes[i].hostname.in(),
              (changes[i].change == ::CIAO::RACE::Planner_I::NODE_JOINED)?"recovered":"lost"
                    ));

          // if node lost, look for a plan involving that node and emit plan failed message...
          // no other action at this time.

          if ( changes[i].change == ::CIAO::RACE::Planner_I::NODE_FAILED )
          {
             DeployedPlansItr bitr,eitr;

             bitr=deployed_plans_.begin();
             eitr=deployed_plans_.end();

             for (;bitr != eitr;bitr++)
             {
               ACE_DEBUG(( LM_DEBUG, "StaticPlanner::DomainChange - Examining plan [%s] for node [%s].\n",
                   bitr->first.c_str(),
                   changes[i].hostname.in()
                   ));

               ::Deployment::DeploymentPlan& plan = bitr->second.plan;

               for (CORBA::ULong j = 0; j < plan.instance.length(); ++j)
               {
                 if (ACE_OS::strcmp( plan.instance[j].node.in(), changes[i].hostname.in() ) == 0)
                 {
                    RACE_DEPLOYMENT_STATUS(
                              Log::EventType::AUTO_PLAN,  Log::SubType::DEPLOYED_PLAN_FAILED,
                              bitr->first.c_str(), 0,
                              Log::Status::SUCCESS,
                              bitr->second.importance,
                              changes[i].hostname.in()
                              );
                    break;
                 }
               }
             }
          }
        }

      ::CIAO::RACE::Plan_Actions::_var_type plans_out( new ::CIAO::RACE::Plan_Actions(0) );
      plans_out->length(0);

      plans = plans_out._retn();

      return true;
    }


      char *
      Planner_I_exec_i::name (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( ::CORBA::SystemException))
      {
        return CORBA::string_dup ("StaticPlanner");
      }

      //==================================================================
      // Component Executor Implementation Class:   StaticPlanner_exec_i
      //==================================================================

      StaticPlanner_exec_i::StaticPlanner_exec_i (void)
      {
      }

      StaticPlanner_exec_i::~StaticPlanner_exec_i (void)
      {
      }

      // Supported or inherited operations.

      // Attribute operations.

      char *
      StaticPlanner_exec_i::name (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( ::CORBA::SystemException))
      {
	return CORBA::string_dup ("StaticPlanner");
      }

      void
      StaticPlanner_exec_i::name (
        const char * /* name */
        ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( ::CORBA::SystemException))
      {
        // Your code here.
      }

      char *
      StaticPlanner_exec_i::type (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( ::CORBA::SystemException))
      {
        // Your code here.
        return 0;
      }

      void
      StaticPlanner_exec_i::type (
        const char * /* type */
        ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( ::CORBA::SystemException))
      {
        // Your code here.
      }

      // Port operations.

      ::CIAO::RACE::CCM_Planner_I_ptr
      StaticPlanner_exec_i::get_planner_i (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( ::CORBA::SystemException))
      {
        // Your code here.
        return new Planner_I_exec_i (*this);
      }

      // Operations from Components::SessionComponent

      void
      StaticPlanner_exec_i::set_session_context (
        ::Components::SessionContext_ptr ctx
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC (( ::CORBA::SystemException,
                       ::Components::CCMException))
      {
        this->context_ =
          StaticPlanner_Context::_narrow (
            ctx
            ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        if (this->context_ == 0)
        {
          ACE_THROW ( ::CORBA::INTERNAL ());
        }
      }

      void
      StaticPlanner_exec_i::ciao_preactivate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( ::CORBA::SystemException,
                       ::Components::CCMException))
      {
        // Your code here.
      }

      void
      StaticPlanner_exec_i::configuration_complete (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( ::CORBA::SystemException,
                       ::Components::CCMException))
      {
        // Your code here.
      }

      void
      StaticPlanner_exec_i::ccm_activate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( ::CORBA::SystemException,
                       ::Components::CCMException))
      {
        // Your code here.
      }

      void
      StaticPlanner_exec_i::ccm_passivate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( ::CORBA::SystemException,
                       ::Components::CCMException))
      {
        // Your code here.
      }

      void
      StaticPlanner_exec_i::ccm_remove (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( ::CORBA::SystemException,
                       ::Components::CCMException))
      {
        // Your code here.
      }

      //==================================================================
      // Home Executor Implementation Class:   StaticPlanner_Home_exec_i
      //==================================================================

      StaticPlanner_Home_exec_i::StaticPlanner_Home_exec_i (void)
      {
      }

      StaticPlanner_Home_exec_i::~StaticPlanner_Home_exec_i (void)
      {
      }

      // Supported or inherited operations.

      // Home operations.

      // Factory and finder operations.

      // Attribute operations.

      // Implicit operations.

      ::Components::EnterpriseComponent_ptr
      StaticPlanner_Home_exec_i::create (
        ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC (( ::CORBA::SystemException,
                       ::Components::CCMException))
      {
        ::Components::EnterpriseComponent_ptr retval =
          ::Components::EnterpriseComponent::_nil ();

        ACE_NEW_THROW_EX (
          retval,
          StaticPlanner_exec_i,
          ::CORBA::NO_MEMORY ());
        ACE_CHECK_RETURN (::Components::EnterpriseComponent::_nil ());

        return retval;
      }

      extern "C" STATICPLANNER_EXEC_Export ::Components::HomeExecutorBase_ptr
      create_CIAO_RACE_StaticPlanner_Home_Impl (void)
      {
        ::Components::HomeExecutorBase_ptr retval =
          ::Components::HomeExecutorBase::_nil ();

        ACE_NEW_RETURN (
          retval,
          StaticPlanner_Home_exec_i,
          ::Components::HomeExecutorBase::_nil ());

        return retval;
      }
    }
  }
}
