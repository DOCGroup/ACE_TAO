// $Id$

#include "Server_Manager.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Loader, Server_Manager, "$Id$")

Server_i::Server_i(void)
  : ior_output_file_ (0),
    policies_ (4)
{
}

Server_i::~Server_i(void)
{
}

// This method parses the input.

int
Server_i::parse_args (int argc,
                      char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "f:");
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

  char ior_output_file_1[BUFSIZ];
  char ior_output_file_2[BUFSIZ];

  ACE_OS::sprintf (ior_output_file_1,
                   "%s_1",
                   ior_output_file_);
  ACE_OS::sprintf (ior_output_file_2,
                   "%s_2",
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
      || ACE_static_cast (size_t,result) != ACE_OS::strlen (first_ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_OS::fprintf failed while writing %s to %s\n",
                       first_ior,
                       ior_output_file_1),
                      -1);

  result = ACE_OS::fprintf (output_file_2,
                            "%s",
                            second_ior);
  if (result <= 0
      || ACE_static_cast (size_t,result) != ACE_OS::strlen (second_ior))
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
Server_i::init (int argc, char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Initialize the ORB.
      orb_ = CORBA::ORB_init (argc,
                              argv,
                              0
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      int result = parse_args (argc, argv);
      if (result != 0)
        return result;

      // Obtain the RootPOA.
      CORBA::Object_var obj =
        orb_->resolve_initial_references ("RootPOA"
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Narrow the Object reference to a POA reference
      root_poa_ = PortableServer::POA::_narrow (obj.in ()
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get the POAManager of RootPOA
      poa_manager_ = root_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Server_i:init_poa ()");
      return 1;
    }
  ACE_ENDTRY;

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

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      policies_.length (4);

      // ID Assignment Policy.
      policies_[0] =
        root_poa_->create_id_assignment_policy
        (PortableServer::USER_ID
         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Lifespan Policy.
      policies_[1] =
        root_poa_->create_lifespan_policy
        (PortableServer::PERSISTENT
         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Request Processing Policy.
      policies_[2] =
        root_poa_->create_request_processing_policy
        (PortableServer::USE_SERVANT_MANAGER
         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Servant Retention Policy.
      if (servant_retention_policy == 1)
        policies_[3] =
          root_poa_->create_servant_retention_policy
          (PortableServer::RETAIN
           ACE_ENV_ARG_PARAMETER);

      if (servant_retention_policy == 0)
        policies_[3] =
          root_poa_->create_servant_retention_policy
          (PortableServer::NON_RETAIN
           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create myPOA as the child of RootPOA with the above
      // policies_. myPOA will use SERVANT_ACTIVATOR or
      // SERVANT_LOCATOR depending upon the servant retention policy
      // being RETAIN or NONRETAIN respectively.
      my_poa = root_poa_->create_POA (name,
                                      poa_manager_.in (),
                                      policies_
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Destroy the policy objects as they have been passed to
      // create_POA and no longer needed.
      for (CORBA::ULong i = 0;
           i < policies_.length ();
           ++i)
        {
          CORBA::Policy_ptr policy = policies_[i];
          policy->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"Server_i:create_poa ()");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  return my_poa;
}

// The Servant Activator object is created and initialised.

int
Server_i::create_activator (PortableServer::POA_var first_poa)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // An Servant Activator object is created which will activate
      // the servant on demand.
      PortableServer::ServantManager_ptr temp_servant_activator;
      ACE_NEW_RETURN (temp_servant_activator,
                      ServantActivator_i (orb_.in (),
                                          "Generic_Servant",
                                          "supply_servant",
                                          "destroy_servant"),
                      0);

      // Set ServantActivator_i object as the servant_manager of
      // firstPOA.
      this->servant_activator_ = temp_servant_activator;
      first_poa->set_servant_manager (this->servant_activator_.in ()
                                      ACE_ENV_ARG_PARAMETER);
      // For the code above, we're using the CORBA 3.0 servant manager
      // semantics supported by TAO.  For CORBA 2.x ORBs you'd need to
      // use the following code in place of the previous lines:
      //
      // this->servant_activator_ = temp_servant_activator->_this ();
      //
      // first_poa->set_servant_manager (this->servant_activator_.in ()
      //                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create a reference with user created ID in firstPOA which
      // uses the MyFooServantActivator.
      PortableServer::ObjectId_var first_foo_oid =
        PortableServer::string_to_ObjectId ("firstFoo");

      first_foo_ = first_poa->create_reference_with_id (first_foo_oid.in (),
                                                        "IDL:Foo:1.0"
                                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"Server_i:create_activator ()");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

// The Servant Locator object is created and initialised.

int
Server_i::create_locator (PortableServer::POA_var second_poa)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // An Servant Locator object is created which will activate
      // the servant on demand.
      PortableServer::ServantManager_ptr temp_servant_locator;
      ACE_NEW_RETURN (temp_servant_locator,
                      ServantLocator_i (orb_.in (),
                                        "Generic_Servant",
                                        "supply_servant",
                                        "destroy_servant"),
                      0);
      // Set ServantLocator_i object as the servant Manager of
      // secondPOA.
      this->servant_locator_ = temp_servant_locator;
      second_poa->set_servant_manager (this->servant_locator_.in ()
                                       ACE_ENV_ARG_PARAMETER);
      // For the code above, we're using the CORBA 3.0 servant manager
      // semantics supported by TAO.  For CORBA 2.x ORBs you'd need to
      // use the following code in place of the previous lines:
      //
      // this->servant_locator_ = temp_servant_locator->_this ();
      //
      // Set ServantLocator_i object as the servant Manager of
      // secondPOA.
      // second_poa->set_servant_manager (this->servant_locator_.in ()
      //                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Try to create a reference with user created ID in second_poa
      // which uses MyFooServantLocator.
      PortableServer::ObjectId_var second_foo_oid =
        PortableServer::string_to_ObjectId ("secondFoo");

      second_foo_ =
        second_poa->create_reference_with_id (second_foo_oid.in (),
                                              "IDL:Foo:1.0"
                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"Server_i:create_locator ()");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

// The execution process of the server.

int
Server_i::run (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Invoke object_to_string on the references created in firstPOA
      // and secondPOA.

      CORBA::String_var first_foo_ior =
        orb_->object_to_string (first_foo_.in ()
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var second_foo_ior =
        orb_->object_to_string (second_foo_.in ()
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Print the ior's of first_foo and second_foo.

      ACE_DEBUG ((LM_DEBUG,"%s\n%s\n",
                  first_foo_ior.in (),
                  second_foo_ior.in ()));

      int write_result =
        this->write_iors_to_file (first_foo_ior.in (),
                                  second_foo_ior.in ());
      if (write_result != 0)
        return write_result;

      // Set the poa_manager state to active, ready to process
      // requests.
      poa_manager_->activate (ACE_ENV_SINGLE_ARG_PARAMETER);

      ACE_TRY_CHECK;

      // Run the ORB.
      orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"Server_i:run ()");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
