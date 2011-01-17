// $Id$

#include "Server_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

Server_i::Server_i(void)
  : ior_output_file_ (0),
    policies_ (4),
    servant_activator_impl_ (0),
    servant_locator_impl_ (0)
{
}

Server_i::~Server_i(void)
{
  delete servant_activator_impl_;
  delete servant_locator_impl_;
}

// This method parses the input.

int
Server_i::parse_args (int argc,
                      ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("f:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f':
        ior_output_file_ = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "[-f ior_output_file] "
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

// The IORs obtained are written into files for further use.

int
Server_i::write_iors_to_file (const char *first_ior,
                              const char *second_ior)
{
  if (ior_output_file_ == 0)
    // No filename was specified; simply return
    return 0;

  ACE_TCHAR ior_output_file_1[BUFSIZ];
  ACE_TCHAR ior_output_file_2[BUFSIZ];

  ACE_OS::sprintf (ior_output_file_1,
                   ACE_TEXT("%s_1"),
                   ior_output_file_);
  ACE_OS::sprintf (ior_output_file_2,
                   ACE_TEXT("%s_2"),
                   ior_output_file_);

  FILE *output_file_1 = ACE_OS::fopen (ior_output_file_1,
                                       "w");
  FILE *output_file_2 = ACE_OS::fopen (ior_output_file_2,
                                       "w");
  if (output_file_1 == 0
      || output_file_2 == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open output files for writing IORs: %s, %s\n",
                       ior_output_file_1,
                       ior_output_file_2),
                      -1);

  int result = ACE_OS::fprintf (output_file_1,
                                "%s",
                                first_ior);
  if (result <= 0
      || static_cast<size_t> (result) != ACE_OS::strlen (first_ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_OS::fprintf failed while writing %s to %s\n",
                       first_ior,
                       ior_output_file_1),
                      -1);

  result = ACE_OS::fprintf (output_file_2,
                            "%s",
                            second_ior);
  if (result <= 0
      || static_cast<size_t> (result) != ACE_OS::strlen (second_ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_OS::fprintf failed while writing %s to %s\n",
                       second_ior,
                       ior_output_file_2),
                      -1);
  ACE_OS::fclose (output_file_1);
  ACE_OS::fclose (output_file_2);
  return 0;
}

// Initialisation of the ORB and POA.

int
Server_i::init (int argc, ACE_TCHAR **argv)
{

  try
    {
      // Initialize the ORB.
      orb_ = CORBA::ORB_init (argc, argv);

      int result = parse_args (argc, argv);
      if (result != 0)
        return result;

      // Obtain the RootPOA.
      CORBA::Object_var obj =
        orb_->resolve_initial_references ("RootPOA");

      // Narrow the Object reference to a POA reference
      root_poa_ = PortableServer::POA::_narrow (obj.in ());

      // Get the POAManager of RootPOA
      poa_manager_ = root_poa_->the_POAManager ();

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Server_i:init_poa ()");
      return 1;
    }

  return 0;
}

// This method creates an poa with 4 policies of which the servent
// retention policy decides whether the Servant Activator or the
// Servant Locator would be used by the Servant Manager.

PortableServer::POA_ptr
Server_i::create_poa (const char *name,
                      int servant_retention_policy)
{
  PortableServer::POA_ptr my_poa = 0;

  try
    {
      policies_.length (4);

      // ID Assignment Policy.
      policies_[0] =
        root_poa_->create_id_assignment_policy
        (PortableServer::USER_ID);

      // Lifespan Policy.
      policies_[1] =
        root_poa_->create_lifespan_policy
        (PortableServer::PERSISTENT);

      // Request Processing Policy.
      policies_[2] =
        root_poa_->create_request_processing_policy
        (PortableServer::USE_SERVANT_MANAGER);

      // Servant Retention Policy.
      if (servant_retention_policy == 1)
        policies_[3] =
          root_poa_->create_servant_retention_policy
          (PortableServer::RETAIN);

      if (servant_retention_policy == 0)
        policies_[3] =
          root_poa_->create_servant_retention_policy
          (PortableServer::NON_RETAIN);

      // Create myPOA as the child of RootPOA with the above
      // policies_. myPOA will use SERVANT_ACTIVATOR or
      // SERVANT_LOCATOR depending upon the servant retention policy
      // being RETAIN or NONRETAIN respectively.
      my_poa = root_poa_->create_POA (name,
                                      poa_manager_.in (),
                                      policies_);

      // Destroy the policy objects as they have been passed to
      // create_POA and no longer needed.
      for (CORBA::ULong i = 0;
           i < policies_.length ();
           ++i)
        {
          CORBA::Policy_ptr policy = policies_[i];
          policy->destroy ();
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Server_i:create_poa ()");
      return 0;
    }

  return my_poa;
}

// The Servant Activator object is created and initialised.

int
Server_i::create_activator (PortableServer::POA_var first_poa)
{
  try
    {
      // An Servant Activator object is created which will activate
      // the servant on-demand.
      ACE_NEW_RETURN (servant_activator_impl_,
                      ServantActivator_i (orb_.in ()),
                      0);

      // Set ServantActivator_i object as the servant_manager of
      // firstPOA.
      first_poa->set_servant_manager (servant_activator_impl_);
      // For the code above, we're using the CORBA 3.0 servant manager
      // semantics supported by TAO.  For CORBA 2.x ORBs you'd need to
      // use the following code in place of the previous line:
      //
      // PortableServer::ServantManager_var servant_activator =
      //   servant_activator_impl_->_this ();
      //
      // first_poa->set_servant_manager (servant_activator.in (),
      //);

      // Create a reference with user created ID in firstPOA which
      // uses the ServantActivator. The servant dll name as well as
      // the factory function in the dll are used in creating the
      // objectId.

      PortableServer::ObjectId_var first_test_oid =
        servant_activator_impl_->create_dll_object_id ("Generic_Servant",
                                                       "create_test_i");

      first_test_ = first_poa->create_reference_with_id (first_test_oid.in (),
                                                        "IDL:test:1.0");
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Server_i:create_activator ()");
      return 1;
    }

  return 0;
}

// The Servant Locator object is created and initialised.

int
Server_i::create_locator (PortableServer::POA_var second_poa)
{
  try
    {
      // An Servant Locator object is created which will activate
      // the servant on-demand.
      ACE_NEW_RETURN (servant_locator_impl_,
                      ServantLocator_i (orb_.in ()),
                      0);

      // Set ServantLocator_i object as the servant Manager of
      // secondPOA.
      second_poa->set_servant_manager (servant_locator_impl_);
      // For the code above, we're using the CORBA 3.0 servant manager
      // semantics supported by TAO.  For CORBA 2.x ORBs you'd need to
      // use the following code in place of the previous line:
      // PortableServer::ServantManager_var servant_loactor =
      //   servant_loator_impl_->_this ();
      //
      // second_poa->set_servant_manager (servant_locator_impl_,
      //);

      // Try to create a reference with user created ID in second_poa
      // which uses ServantLocator. The servant dll name as well as
      // the factory function in the dll are used in creating the
      // objectId.
      PortableServer::ObjectId_var second_test_oid =
        servant_locator_impl_->create_dll_object_id
        ("Generic_Servant",
         "create_test_i");
      second_test_ = second_poa->create_reference_with_id
        (second_test_oid.in (),
         "IDL:test:1.0");
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Server_i:create_locator ()");
      return 1;
    }

  return 0;
}

// The execution process of the server.

int
Server_i::run (void)
{
  try
    {
      // Invoke object_to_string on the references created in firstPOA
      // and secondPOA.

      CORBA::String_var first_test_ior =
        orb_->object_to_string (first_test_.in ());

      CORBA::String_var second_test_ior =
        orb_->object_to_string (second_test_.in ());

      // Print the ior's of first_test and second_test.

      ACE_DEBUG ((LM_DEBUG,"%C\n%C\n",
                  first_test_ior.in (),
                  second_test_ior.in ()));

      int write_result =
        this->write_iors_to_file (first_test_ior.in (),
                                  second_test_ior.in ());
      if (write_result != 0)
        return write_result;

      // Set the poa_manager state to active, ready to process
      // requests.
      poa_manager_->activate ();


      // Run the ORB.
      orb_->run ();

      orb_->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Server_i:run ()");
      return 1;
    }
  return 0;
}
