#include "Converter.h"
namespace CIAO
{

  namespace RACE
  {

    Converter::Converter (int argc, char **argv)
      : iia_name_ ("RACE::InteractiveInput"),
        history_ (false)
    {
      try
        {
          this->orb_ = CORBA::ORB_init (argc, argv, "");
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

          this->plan_gen_.init (this->orb_);

        }
      catch (CORBA::Exception &ex)
        {
          ACE_PRINT_EXCEPTION (ex, "Error in initializing the Injector!\n");
        }

    }

    Converter::~Converter ()
    {}


    int
    Converter::convert (OperationalString &op_string,
                        ::Deployment::DeploymentPlan &plan)
    {
      plan.label = op_string.name;

      plan.UUID = op_string.UUID;

      plan.connection = op_string.dataLinks;

      plan.infoProperty = op_string.properties;

      int position;

      for (CORBA::ULong itr = 0; itr < op_string.instances.length (); ++itr)
        {
          ::CIAO::RACE::InstanceDescription op_instance =
            op_string.instances [itr];
          if (this->plan_gen_.generate_plan
              (plan,
               op_instance.suggestedImpl.in (),
               position))
            {
              ::Deployment::InstanceDeploymentDescription instance;
              instance.name = op_instance.name;
              instance.node = CORBA::string_dup ("Satellite");
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
    Converter::deploy_plan (::Deployment::DeploymentPlan &plan)
    {
      CIAO::RACE::Metadata_var metadata = new OBV_CIAO::RACE::Metadata;

      try
        {

          if (this->history_)
            {
              metadata->command (::CIAO::RACE::TEARDOWN);
              metadata->plan (this->previous_);
              this->iia_->get_consumer_meta_data ()->push_Metadata (metadata.in ());
            }

          metadata->command (::CIAO::RACE::DEPLOY);
          metadata->plan (plan);
          this->iia_->get_consumer_meta_data ()->push_Metadata (metadata.in ());
          this->history_ = true;
          this->previous_ = plan;
        }
      catch (CORBA::Exception &ex)
        {
          ACE_PRINT_EXCEPTION (ex, "Exception caught\n");
          return -1;
        }

      catch (...)
        {
          ACE_ERROR ((LM_ERROR, "(%P|%t) Injector: Unknown exception\n"));
          return -1;
        }

      return 0;
    }

  }
}


int main (int argc, char **argv)
{
  try
    {
      ::CIAO::RACE::Converter converter (argc, argv);

      ::CIAO::RACE::OperationalString op_string;

      ::Deployment::DeploymentPlan plan;

      if (converter.convert (op_string, plan) == 0)
        {
          converter.deploy_plan (plan);
        }
      else
        {
          return -1;
        }
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "(%P|%t) Unknown exception\n"));
      return -1;
    }
}
