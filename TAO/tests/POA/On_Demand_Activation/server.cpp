
//=============================================================================
/**
 *  @file     server.cpp
 *
 *   Server to test the Servant Activator and Servant Locator for a POA.
 *
 *  @author  Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================

#include "ace/streams.h"
#include "ace/Get_Opt.h"
#include "Servant_Activator.h"
#include "Servant_Locator.h"
#include "ace/OS_NS_stdio.h"

#include <cstring>
#include <memory>
#include <string>

struct Server_Options
{
  ACE_TCHAR const *ior_output_file { ACE_TEXT ("ior") };
};

static int
parse_args (int argc, ACE_TCHAR **argv, Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("f:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'f':
        options.ior_output_file = get_opts.opt_arg ();
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
write_iors_to_file (ACE_TCHAR const *ior_output_file,
                    char const *first_ior,
                    char const *second_ior)
{
  std::basic_string<ACE_TCHAR> ior_output_file_1 (ior_output_file);
  std::basic_string<ACE_TCHAR> ior_output_file_2 (ior_output_file);
  ior_output_file_1 += ACE_TEXT ("_1");
  ior_output_file_2 += ACE_TEXT ("_2");

  std::unique_ptr<FILE, int (*) (FILE *)> output_file_1 (
    ACE_OS::fopen (ior_output_file_1.c_str (), "w"), ACE_OS::fclose);
  std::unique_ptr<FILE, int (*) (FILE *)> output_file_2 (
    ACE_OS::fopen (ior_output_file_2.c_str (), "w"), ACE_OS::fclose);

  if (!output_file_1 || !output_file_2)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open output files for writing IORs: %C, %C\n",
                       ACE_TEXT_ALWAYS_CHAR (ior_output_file_1.c_str ()),
                       ACE_TEXT_ALWAYS_CHAR (ior_output_file_2.c_str ())),
                      -1);

  int result = ACE_OS::fprintf (output_file_1.get (), "%s", first_ior);
  if (result <= 0
      || static_cast<size_t> (result) != std::strlen (first_ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_OS::fprintf failed while writing %C to %s\n",
                       first_ior,
                       ACE_TEXT_ALWAYS_CHAR (ior_output_file_1.c_str ())),
                      -1);

  result = ACE_OS::fprintf (output_file_2.get (), "%s", second_ior);
  if (result <= 0
      || static_cast<size_t> (result) != std::strlen (second_ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_OS::fprintf failed while writing %C to %s\n",
                       second_ior,
                       ACE_TEXT_ALWAYS_CHAR (ior_output_file_2.c_str ())),
                      -1);

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      Server_Options options;
      int const result = parse_args (argc, argv, options);
      if (result != 0)
        return result;

      // Obtain the RootPOA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      // Narrow the Object reference to a POA reference
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      // Get the POAManager of RootPOA

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      CORBA::PolicyList policies (4);
      policies.length (4);

      // ID Assignment Policy
      policies[0] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID);

      // Lifespan Policy
      policies[1] =
        root_poa->create_lifespan_policy (PortableServer::PERSISTENT);

      // Request Processing Policy
      policies[2] =
        root_poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER);

      PortableServer::POA_var first_poa;
      {
        // Servant Retention Policy
        policies[3] =
          root_poa->create_servant_retention_policy (PortableServer::RETAIN);

        // Create firstPOA as the child of RootPOA with the above policies
        // firstPOA will use SERVANT_ACTIVATOR because of RETAIN policy.
        first_poa = root_poa->create_POA ("firstPOA",
                                          poa_manager.in (),
                                          policies);
      }

      PortableServer::POA_var second_poa;
      {
        // Servant Retention Policy
        policies[3] =
          root_poa->create_servant_retention_policy (PortableServer::NON_RETAIN);

        // Create secondPOA as child of RootPOA with the above policies
        // secondPOA will use a SERVANT_LOCATOR because of NON_RETAIN
        // policy.
        second_poa = root_poa->create_POA ("secondPOA",
                                           poa_manager.in (),
                                           policies);
      }

      // Destroy the policy objects as they have been passed to
      // create_POA and no longer needed.
      for (CORBA::ULong i = 0;
           i < policies.length ();
           ++i)
        {
          CORBA::Policy_ptr const policy = policies[i];
          policy->destroy ();
        }

      // Allocate the servant activator.
      ServantActivator activator (orb.in ());

      // Set ServantActivator object as the servant_manager of
      // firstPOA.
      first_poa->set_servant_manager (&activator);
      // For the code above, we're using the CORBA 3.0 servant manager
      // semantics supported by TAO.  For CORBA 2.x ORBs you'd need to
      // use the following code in place of the previous line:
      //
      // PortableServer::ServantManager_var servant_activator =
      //   activator->_this ();
      //
      // first_poa->set_servant_manager (servant_activator.in (),
      //);

      // Create a reference with user created ID in firstPOA which uses
      // the ServantActivator.

      PortableServer::ObjectId_var first_test_oid =
        PortableServer::string_to_ObjectId ("first test");

      CORBA::Object_var first_test =
        first_poa->create_reference_with_id (first_test_oid.in (), "IDL:test:1.0");

      // Allocate the servant locator.
      ServantLocator locator (orb.in ());

      // Set ServantLocator object as the servant Manager of
      // secondPOA.
      second_poa->set_servant_manager (&locator);
      // For the code above, we're using the CORBA 3.0 servant manager
      // semantics supported by TAO.  For CORBA 2.x ORBs you'd need to
      // use the following code in place of the previous line:
      //
      // PortableServer::ServantManager_var servant_locator =
      //   locator->_this ();
      //
      // second_poa->set_servant_manager (servant_locator.in (),
      //);

      // Try to create a reference with user created ID in second_poa
      // which uses ServantLocator.

      PortableServer::ObjectId_var second_test_oid =
        PortableServer::string_to_ObjectId ("second test");

      CORBA::Object_var second_test =
        second_poa->create_reference_with_id (second_test_oid.in (),
                                              "IDL:test:1.0");

      // Invoke object_to_string on the references created in firstPOA and
      // secondPOA.

      CORBA::String_var first_test_ior =
        orb->object_to_string (first_test.in ());

      CORBA::String_var second_test_ior =
        orb->object_to_string (second_test.in ());

      // Print the ior's of first_test and second_test.

      ACE_DEBUG ((LM_DEBUG, "%C\n%C\n",
                 first_test_ior.in (),
                 second_test_ior.in ()));

      int const write_result = write_iors_to_file (options.ior_output_file,
                                                   first_test_ior.in (),
                                                   second_test_ior.in ());
      if (write_result != 0)
        return write_result;

      // Set the poa_manager state to active, ready to process requests.
      poa_manager->activate ();

      // Run the ORB.
      orb->run ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught in main");
      return -1;
    }

  return 0;
}
