// $Id$

//============================================================================
//
// = FILENAME
//     server.cpp
//
// = DESCRIPTION
//     Server to test the Servant Activator and Servant Locator for a POA.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
//=============================================================================

#include "ace/streams.h"
#include "Servant_Activator.h"
#include "Servant_Locator.h"

ACE_RCSID(On_Demand_Activation, server, "$Id$")

static char *ior_output_file = 0;

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "f:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f':
        ior_output_file = get_opts.optarg;
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

static int
write_iors_to_file (const char *first_ior,
                    const char *second_ior)
{
  if (ior_output_file == 0)
    // No filename was specified; simply return
    return 0;

  char ior_output_file_1[BUFSIZ];
  char ior_output_file_2[BUFSIZ];

  ACE_OS::sprintf (ior_output_file_1, "%s_1", ior_output_file);
  ACE_OS::sprintf (ior_output_file_2, "%s_2", ior_output_file);

  FILE *output_file_1 = ACE_OS::fopen (ior_output_file_1, "w");
  FILE *output_file_2 = ACE_OS::fopen (ior_output_file_2, "w");

  if (output_file_1 == 0 ||
      output_file_2 == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Cannot open output files for writing IORs: %s, %s\n",
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

int
main (int argc, char **argv)
{
  CORBA::Environment TAO_TRY_ENV;

  TAO_TRY
    {

      // Initialize the ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0, TAO_TRY_ENV); 

      TAO_CHECK_ENV;

      int result = parse_args (argc, argv);
      if (result != 0)
        return result;

      // Get an Object reference to RootPOA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      // Narrow the Object reference to a POA reference
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in (), TAO_TRY_ENV);  
      TAO_CHECK_ENV;
  
      // Get the POAManager of RootPOA

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_TRY_ENV);
  
      TAO_CHECK_ENV;

      CORBA::PolicyList policies (4);
      policies.length (4);

      // ID Assignment Policy
      policies[0] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID, TAO_TRY_ENV);
  
      TAO_CHECK_ENV;

      // Lifespan Policy
      policies[1] =
        root_poa->create_lifespan_policy (PortableServer::PERSISTENT, TAO_TRY_ENV);
  
      TAO_CHECK_ENV;
  
      // Request Processing Policy
      policies[2] =
        root_poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER, TAO_TRY_ENV);
  
      TAO_CHECK_ENV;

  
      // These are the changes we wish to implement for the POA level granularity in using the Servant_Manager.
#if 0
  
      // Request Processing Policy
      policies[i] =
        root_poa->create_servant_configurator_policy (PortableServer::USE_SERVANT_CONFIGURATOR,
                                                      dll_name,
                                                      factory_function,
                                                      TAO_TRY_ENV);
#endif


      PortableServer::POA_var first_poa;
      {
        // Servant Retention Policy
        policies[3] =
          root_poa->create_servant_retention_policy (PortableServer::RETAIN, TAO_TRY_ENV);


        TAO_CHECK_ENV;

        ACE_CString name ("firstPOA");

        // Create firstPOA as the child of RootPOA with the above policies
        // firstPOA will use SERVANT_ACTIVATOR because of RETAIN policy.
        first_poa = root_poa->create_POA (name.c_str (),
                                          poa_manager.in (),
                                          policies,
                                          TAO_TRY_ENV);

        TAO_CHECK_ENV;

      }

      PortableServer::POA_var second_poa;
      {
        // Servant Retention Policy
        policies[3] =
          root_poa->create_servant_retention_policy (PortableServer::NON_RETAIN, TAO_TRY_ENV);

        TAO_CHECK_ENV;
  
        ACE_CString name ("secondPOA");

        // Create secondPOA as child of RootPOA with the above policies
        // secondPOA will use a SERVANT_LOCATOR because of NON_RETAIN
        // policy.
        second_poa = root_poa->create_POA (name.c_str (),
                                           poa_manager.in (),
                                           policies,
                                           TAO_TRY_ENV);
        TAO_CHECK_ENV;
   
        // Destroy the policy objects as they have been passed to
        // create_POA and no longer needed.
        for (CORBA::ULong i = 0;
             i < policies.length () && TAO_TRY_ENV.exception () == 0;
             ++i)
          {
            CORBA::Policy_ptr policy = policies[i];
            policy->destroy (TAO_TRY_ENV);
          }

        TAO_CHECK_ENV;
  
        MyFooServantActivator servant_activator_impl (orb.in ());
        PortableServer::ServantActivator_var servant_activator =
          servant_activator_impl._this (TAO_TRY_ENV);

        TAO_CHECK_ENV;
 
        // Set MyFooServantActivator object as the servant_manager of
        // firstPOA.

        first_poa->set_servant_manager (servant_activator.in (), TAO_TRY_ENV);

        TAO_CHECK_ENV;
  
        // Create a reference with user created ID in firstPOA which uses
        // the MyFooServantActivator. The servant dll name as well as the
        // factory function in the dll are used in creating the objectId.

        // *done*@@ Kirthika, please make it clear that this is a TAO extension
        // and also explain that <MyFoo> is the DLL and <create_MyFoo> is
        // the factory function in the DLL that creates a Servant.
        PortableServer::ObjectId_var first_foo_oid =
          // *done*x@@ Kirthika, perhaps you should change the name of this method
          // to make it more clear that there's dynamic linking etc. going
          // on here.  For instance, you could call it
          // create_dll_object_id().  Please check with Irfan for
          // suggestions on this name.
          servant_activator_impl.create_dll_object_id ("MyFoo",
                                                       "create_MyFoo");

        CORBA::Object_var first_foo =
          first_poa->create_reference_with_id (first_foo_oid.in (), "IDL:Foo:1.0", TAO_TRY_ENV);

        TAO_CHECK_ENV;
  
        MyFooServantLocator servant_locator_impl (orb.in ());
        PortableServer::ServantLocator_var servant_locator =
          servant_locator_impl._this (TAO_TRY_ENV);

        TAO_CHECK_ENV;

   
        // Set MyFooServantLocator object as the servant Manager of
        // secondPOA.

        second_poa->set_servant_manager (servant_locator.in (), TAO_TRY_ENV);

        TAO_CHECK_ENV;

        // Try to create a reference with user created ID in second_poa
        // which uses MyFooServantLocator. The servant dll name as well as the
        // factory function in the dll are used in creating the objectId.

        PortableServer::ObjectId_var second_foo_oid =
          servant_locator_impl.create_dll_object_id ("MyFoo",
                                                     "create_MyFoo");

        CORBA::Object_var second_foo =
          second_poa->create_reference_with_id (second_foo_oid.in (),
                                                "IDL:Foo:1.0", TAO_TRY_ENV);

        TAO_CHECK_ENV;

        // Invoke object_to_string on the references created in firstPOA and
        // secondPOA.

        CORBA::String_var first_foo_ior =
          orb->object_to_string (first_foo.in (), TAO_TRY_ENV);

        TAO_CHECK_ENV;
  
        CORBA::String_var second_foo_ior =
          orb->object_to_string (second_foo.in (), TAO_TRY_ENV);

        TAO_CHECK_ENV;
  
        // Print the ior's of first_foo and second_foo.

        ACE_DEBUG((LM_DEBUG,"%s\n%s\n",
                   first_foo_ior.in (),
                   second_foo_ior.in ()));

        int write_result = write_iors_to_file (first_foo_ior.in (),
                                               second_foo_ior.in ());
        if (write_result != 0)
          return write_result;

        // Set the poa_manager state to active, ready to process requests.
        poa_manager->activate (TAO_TRY_ENV);

        TAO_CHECK_ENV;
  
        // Run the ORB.
        if (orb->run () == -1)
          ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "CORBA::ORB::run"), -1);

        // Destroy the root_poa and also first_poa and second_poa

        root_poa->destroy (1,
                           1,
                           TAO_TRY_ENV);
        TAO_CHECK_ENV;

      }
    }
  TAO_CATCHANY 
    {
      TAO_TRY_ENV.print_exception ("server:main ()");
      return 1;
    }
  TAO_ENDTRY;
  
  return 0;
}
