#include "Manager.h"

#include "tao/IORManipulation/IORManip_Loader.h"

#include "ace/SString.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include <memory>

int
parse_args (int argc, ACE_TCHAR *argv[], Manager_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("a:b:c:d:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'a':
        options.first_ior = get_opts.opt_arg ();
        break;
      case 'b':
        options.second_ior = get_opts.opt_arg ();
        break;
      case 'c':
        options.third_ior = get_opts.opt_arg ();
        break;
      case 'd':
        options.ior_output_file = get_opts.opt_arg ();
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
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Manager manager;
  Manager_Options options;

  try
    {
      // Initilaize the ORB, POA etc.
      manager.init (argc, argv);

      if (parse_args (argc, argv, options) == -1)
        return -1;

      manager.activate_servant ();

      manager.make_iors_register (options);

      manager.run ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught");
      return -1;
    }

  return 0;
}

int
Manager::init (int argc, ACE_TCHAR *argv[])
{
  this->orb_ = CORBA::ORB_init (argc, argv);

  // Obtain the RootPOA.
  CORBA::Object_var const obj_var =
    this->orb_->resolve_initial_references ("RootPOA");

  // Get the POA_var object from Object_var.
  PortableServer::POA_var const root_poa_var =
    PortableServer::POA::_narrow (obj_var.in ());

  // Get the POAManager of the RootPOA.
  PortableServer::POAManager_var const poa_manager_var =
    root_poa_var->the_POAManager ();

  poa_manager_var->activate ();

  // Policies for the childPOA to be created.
  CORBA::PolicyList policies (4);
  policies.length (4);

  // The next two policies are common to both
  // Id Assignment Policy
  policies[0] =
    root_poa_var->create_id_assignment_policy (PortableServer::USER_ID);

  // Lifespan policy
  policies[1] =
    root_poa_var->create_lifespan_policy (PortableServer::PERSISTENT);

  // Tell the POA to use a servant manager
  policies[2] =
    root_poa_var->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER);

  // Servant Retention Policy -> Use a locator
  policies[3] =
    root_poa_var->create_servant_retention_policy (PortableServer::NON_RETAIN);

  ACE_CString const name = "newPOA";

  new_poa_var_ =
    root_poa_var->create_POA (name.c_str (),
                              poa_manager_var.in (),
                              policies);

  // Creation of childPOAs is over. Destroy the Policy objects.
  for (CORBA::ULong i = 0;
       i < policies.length ();
       ++i)
    {
      CORBA::Policy_ptr const policy = policies[i];
      policy->destroy ();
    }

  return 0;
}

int
Manager::activate_servant ()
{
  ACE_NEW_THROW_EX (this->servant_locator_,
                    Servant_Locator (),
                    CORBA::NO_MEMORY ());

  this->servant_locator_var_ = this->servant_locator_;

  // Set ServantLocator object as the servant Manager of
  // secondPOA.
  this->new_poa_var_->set_servant_manager (this->servant_locator_var_.in ());

  // Try to create a reference with user created ID in new_poa
  // which uses ServantLocator.

  PortableServer::ObjectId_var second_foo_oid_var =
    PortableServer::string_to_ObjectId ("Simple_Server");

  this->new_manager_ior_ =
    new_poa_var_->create_reference_with_id (second_foo_oid_var.in (),
                                            "IDL:Simple_Server:1.0");

  return 0;
}

int
Manager::make_iors_register (Manager_Options const &options)
{
  // First  server
  CORBA::Object_var const object_primary =
    this->orb_->string_to_object (options.first_ior.c_str ());

  // Second server
  CORBA::Object_var const object_secondary =
    this->orb_->string_to_object (options.second_ior.c_str ());

  if (options.third_ior.empty ())
    ACE_DEBUG ((LM_DEBUG,
                "Here is the culprit\n"));
  // Third Server
  CORBA::Object_var const object_tertiary =
    this->orb_->string_to_object (options.third_ior.c_str ());

  // Get an object reference for the ORBs IORManipultion object!
  CORBA::Object_var const iorm_object =
    this->orb_->resolve_initial_references (TAO_OBJID_IORMANIPULATION,
                                            {});

  TAO_IOP::TAO_IOR_Manipulation_var const iorm =
    TAO_IOP::TAO_IOR_Manipulation::_narrow (iorm_object.in ());

  // Create the list
  TAO_IOP::TAO_IOR_Manipulation::IORList iors (3);
  iors.length (3);
  iors [0] = CORBA::Object::_duplicate (object_primary.in ());
  iors [1] = CORBA::Object::_duplicate (object_secondary.in ());
  iors [2] = CORBA::Object::_duplicate (this->new_manager_ior_.in ());

  // Create a merged set 1;
  CORBA::Object_var const merged_set1 = iorm->merge_iors (iors);

  if (object_secondary.in () == nullptr)
    ACE_DEBUG ((LM_DEBUG,
                " There is a problem 1 "));

  if (object_tertiary.in () == nullptr)
    ACE_DEBUG ((LM_DEBUG,
                " There is a problem\n"));

  TAO_IOP::TAO_IOR_Manipulation::IORList iors_again (3);
  iors_again.length (3);
  iors_again [0] = CORBA::Object::_duplicate (object_secondary.in ());
  iors_again [1] = CORBA::Object::_duplicate (object_tertiary.in ());
  iors_again [2] = CORBA::Object::_duplicate (this->new_manager_ior_.in ());

  // Create merged set 2
  CORBA::Object_var const merged_set2 = iorm->merge_iors (iors_again);

  CORBA::String_var const iorref1 = this->orb_->object_to_string (merged_set1.in ());

  if (!options.ior_output_file.empty ())
    {
      std::unique_ptr<FILE, int (*) (FILE *)> output_file (
        ACE_OS::fopen (options.ior_output_file.c_str (), ACE_TEXT ("w")),
        ACE_OS::fclose);
      if (!output_file)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                           options.ior_output_file.c_str ()),
                          1);
      ACE_OS::fprintf (output_file.get (), "%s", iorref1.in ());
    }

  this->servant_locator_->set (merged_set2.in ());

  return 0;
}

int
Manager::run ()
{
  this->orb_->run ();

  return 0;
}
