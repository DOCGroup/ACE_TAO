//$Id$
#include "ace/Get_Opt.h"
#include "Manager.h"
#include "tao/IORManipulation/IORManip_Loader.h"

const char *first_ior = 0;
const char *second_ior = 0;
const char *third_ior = 0;
const char *ior_output_file = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "a:b:c:d:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'a':
        first_ior = get_opts.opt_arg ();
        break;
      case 'b':
        second_ior = get_opts.opt_arg ();
        break;
      case 'c':
        third_ior = get_opts.opt_arg ();
        break;
      case 'd':
        ior_output_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-a <iorfile>"
                           "-b <iorfile>"
                           "-c <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}


int
main (int argc,
      char *argv[])
{
  TAO_ENV_DECLARE_NEW_ENV;

  Manager manager;

  ACE_TRY
    {
      // Initilaize the ORB, POA etc.
      manager.init (argc,
                    argv
                    TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) == -1)
        return -1;

      manager.activate_servant (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      manager.make_iors_register (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      manager.run (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

Manager::Manager (void)
  :orb_ (0),
   new_poa_var_ (0)
{
  //no-op
}

int
Manager::init (int argc,
               char *argv[]
               TAO_ENV_ARG_DECL)
{
  this->orb_ = CORBA::ORB_init (argc,
                                argv,
                                0
                                TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Obtain the RootPOA.
  CORBA::Object_var obj_var =
    this->orb_->resolve_initial_references ("RootPOA" TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Get the POA_var object from Object_var.
  PortableServer::POA_var root_poa_var =
    PortableServer::POA::_narrow (obj_var.in () TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Get the POAManager of the RootPOA.
  PortableServer::POAManager_var poa_manager_var =
    root_poa_var->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  poa_manager_var->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Policies for the childPOA to be created.
  CORBA::PolicyList policies (4);
  policies.length (4);

  // The next two policies are common to both
  // Id Assignment Policy
  policies[0] =
    root_poa_var->create_id_assignment_policy (PortableServer::USER_ID
                                               TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Lifespan policy
  policies[1] =
    root_poa_var->create_lifespan_policy (PortableServer::PERSISTENT
                                          TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Tell the POA to use a servant manager
  policies[2] =
    root_poa_var->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER
                                                    TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Servant Retention Policy -> Use a locator
  policies[3] =
    root_poa_var->create_servant_retention_policy (PortableServer::NON_RETAIN
                                                   TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_CString name = "newPOA";

  new_poa_var_ =
    root_poa_var->create_POA (name.c_str (),
                              poa_manager_var.in (),
                              policies
                              TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Creation of childPOAs is over. Destroy the Policy objects.
  for (CORBA::ULong i = 0;
       i < policies.length ();
       ++i)
    {
      CORBA::Policy_ptr policy = policies[i];
      policy->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }

  return 0;
}


int
Manager::activate_servant (TAO_ENV_SINGLE_ARG_DECL)
{

  ACE_NEW_THROW_EX (this->servant_locator_,
                    Servant_Locator (this->orb_.in ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (-1);

  this->servant_locator_var_ = this->servant_locator_;

  // Set ServantLocator object as the servant Manager of
  // secondPOA.
  this->new_poa_var_->set_servant_manager (this->servant_locator_
                                           TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Try to create a reference with user created ID in new_poa
  // which uses ServantLocator.

  PortableServer::ObjectId_var second_foo_oid_var =
    PortableServer::string_to_ObjectId ("Simple_Server");

  this->new_manager_ior_ =
    new_poa_var_->create_reference_with_id (second_foo_oid_var.in (),
                                            "IDL:Simple_Server:1.0" TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}


int
Manager::make_iors_register (TAO_ENV_SINGLE_ARG_DECL)
{
  // First  server
  CORBA::Object_var object_primary =
    this->orb_->string_to_object (first_ior
                                  TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  //Second server
  CORBA::Object_var object_secondary =
    this->orb_->string_to_object (second_ior
                                  TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (third_ior == 0)
    ACE_DEBUG ((LM_DEBUG,
                "Here is the culprit \n"));
  // Third Server
  CORBA::Object_var object_tertiary =
    this->orb_->string_to_object (third_ior
                                  TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Get an object reference for the ORBs IORManipultion object!
  CORBA_Object_ptr IORM =
    this->orb_->resolve_initial_references (TAO_OBJID_IORMANIPULATION,
                                            0
                                            TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  TAO_IOP::TAO_IOR_Manipulation_ptr iorm =
    TAO_IOP::TAO_IOR_Manipulation::_narrow (IORM TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);


  // Create the list
  TAO_IOP::TAO_IOR_Manipulation::IORList iors (3);
  iors.length(3);
  iors [0] = CORBA::Object::_duplicate (object_primary.in ());
  iors [1] = CORBA::Object::_duplicate (object_secondary.in ());
  iors [2] = CORBA::Object::_duplicate (this->new_manager_ior_.in ());

  // Create a merged set 1;
  CORBA_Object_var merged_set1 =
    iorm->merge_iors (iors TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (object_secondary.in () == 0)
    ACE_DEBUG ((LM_DEBUG,
                " There is a problem 1 "));

  if (object_tertiary.in () == 0)
    ACE_DEBUG ((LM_DEBUG,
                " There is a problem \n"));

  TAO_IOP::TAO_IOR_Manipulation::IORList iors_again (3);
  iors_again.length(3);
  iors_again [0] = CORBA::Object::_duplicate (object_secondary.in ());
  iors_again [1] = CORBA::Object::_duplicate (object_tertiary.in ());
  iors_again [2] = CORBA::Object::_duplicate (this->new_manager_ior_.in ());

  // Create merged set 2
  CORBA_Object_var merged_set2 =
    iorm->merge_iors (iors_again TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CORBA::String_var iorref1 =
    this->orb_->object_to_string (merged_set1.in ());

  if (ior_output_file != 0)
    {
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                          1);
      ACE_OS::fprintf (output_file, "%s", iorref1.in ());
      ACE_OS::fclose (output_file);
    }

  this->servant_locator_->set (merged_set2);

  return 0;
}


int
Manager::run (TAO_ENV_SINGLE_ARG_DECL)
{
  this->orb_->run (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}
