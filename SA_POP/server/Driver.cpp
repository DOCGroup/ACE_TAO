#include "Driver.h"

namespace CIAO
{
  namespace RACE
  {

    namespace SA_POP
    {
      Driver_i::Driver_i (CORBA::ORB_ptr orb)
        : orb_ (CORBA::ORB::_duplicate (orb)),
          iia_name_ ("RACE::InteractiveInput")
      {
      }

      Driver_i::~Driver_i (void)
      {
      }

      int
      Driver_i::init (const char *RM_name)
      {
        try
          {
            // Resolve naming service
            CORBA::Object_var ns_tmp =
              this->orb_->resolve_initial_references ("NameService");
            ::CosNaming::NamingContext_var ns =
                ::CosNaming::NamingContext::_narrow (ns_tmp.in ());
            ::CosNaming::Name ns_name;
            CORBA::ULong i = 0;
            ACE_Tokenizer tok (this->iia_name_.rep ());
            tok.delimiter_replace (':', 0);
            tok.delimiter_replace ('/', 0);
            char *name_element = 0;
            while ((name_element = tok.next ()) != 0)
              {
                ns_name.length (ns_name.length () + 1);
                ns_name[i].id = CORBA::string_dup (name_element);
                ++i;
              }

            /// now try to resolve the reference to the IIA.
            CORBA::Object_var iia_tmp = ns->resolve (ns_name);
            this->iia_ =
              ::CIAO::RACE::Interactive_Input_Adapter::_narrow (iia_tmp.in ());

            // Now initialize the plan generator.
            //            this->plan_gen_.init (this->orb_, true, RM_name);
            return 0;
          }
        catch (CORBA::Exception &ex)
          {
            ACE_PRINT_EXCEPTION (ex, "Error in initializing the driver!\n");
            return -1;
          }
      }


      void
      Driver_i::deploy_plan (const char * plan_uri)
        throw (::CORBA::SystemException)
      {
        try
          {
            // Create deploy input event and populate it.
            CIAO::RACE::Deploy_Input_var input = new OBV_CIAO::RACE::Deploy_Input;
            input->plan_uri (plan_uri);
            input->command (::CIAO::RACE::DEPLOY);

            // Now push the event to the IIA.
            this->iia_->get_consumer_deployment ()->push_Deploy_Input (input.in ());

          }
        catch (CORBA::Exception &ex)
          {
            ACE_PRINT_EXCEPTION (ex, "Exception caught in Driver::deploy_plan");
          }
      }

      void
      Driver_i::deploy_goal (const ::CIAO::RACE::GoalStructure &)
        throw (::CORBA::SystemException)
      {
      }

      int
      Driver_i::convert (::CIAO::RACE::OperationalString &op_string,
                         ::Deployment::DeploymentPlan &plan)
      {
        // Populate the label, UUID, connections, and info property fields of
        // the deployment plan.
        plan.label = op_string.name;
        plan.UUID = op_string.UUID;
        plan.connection = op_string.dataLinks;
        plan.infoProperty = op_string.properties;

        // Now, populate the instance, implementation, and artifact fields of
        // the deployment plan using the plan generator.
        int position;
        for (CORBA::ULong itr = 0; itr < op_string.instances.length (); ++itr)
          {

            ::CIAO::RACE::InstanceDescription op_instance = op_string.instances [itr];
            if (this->plan_gen_.generate_plan
                (plan,
                 op_instance.suggestedImpl.in (),
                 position))
              {
                ::Deployment::InstanceDeploymentDescription instance;
                instance.name = op_instance.name;

                // @@ This is a temporaty hack. Later on we must be using the
                // simple bin packer do perform the node assignment.
                if (strcmp ("IDL:SPACE/Ground_Station:1.0",
                            op_instance.suggestedImpl.in ()) == 0)
                  {
                    instance.node = CORBA::string_dup ("ground");
                  }
                else
                  {
                    instance.node = CORBA::string_dup ("space");
                  }

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
                return -1;
              }
          }
        return 0;
      }

      int
      Driver_i::deploy_plan (::Deployment::DeploymentPlan &plan)
      {
        try
          {
            // Create a metadata event and populate it.
            CIAO::RACE::Metadata_var metadata = new OBV_CIAO::RACE::Metadata;
            metadata->command (::CIAO::RACE::DEPLOY);
            metadata->plan (plan);

            // Now push the event to the IIA.
            this->iia_->get_consumer_meta_data ()->push_Metadata (metadata.in ());
          }
        catch (CORBA::Exception &ex)
          {
            ACE_PRINT_EXCEPTION (ex, "Exception caught\n");
            return -1;
          }

        catch (...)
          {
            ACE_ERROR ((LM_ERROR, "(%P|%t) Driver_i: Unknown exception\n"));
            return -1;
          }
        return 0;
      }
    }
  }
}
