
//=============================================================================
/**
 *  @file    server.cpp
 *
 *  Explicit creation of servants.
 *
 *  @author Irfan Pyarali
 */
//=============================================================================

#include "ace/Get_Opt.h"
#include "test_i.h"
#include "ace/OS_NS_stdio.h"

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
                           "-f ior_file"
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

static int
write_ior_to_file (std::basic_string<ACE_TCHAR> const &file_name,
                   char const *ior)
{
  std::unique_ptr<FILE, int (*) (FILE *)> output_file (
    ACE_OS::fopen (file_name.c_str (), "w"), ACE_OS::fclose);
  if (!output_file)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open output file for writing IOR: %s\n",
                       ACE_TEXT_ALWAYS_CHAR (file_name.c_str ())),
                      -1);

  int const result = ACE_OS::fprintf (output_file.get (), "%s", ior);
  if (result <= 0 || static_cast<size_t> (result) != ACE_OS::strlen (ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_OS::fprintf failed while writing %C to %s\n",
                       ior,
                       ACE_TEXT_ALWAYS_CHAR (file_name.c_str ())),
                      -1);

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      Server_Options options;
      int const result = parse_args (argc, argv, options);
      if (result != 0)
        return result;

      // Obtain the RootPOA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      // Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      // Policies for the firstPOA to be created.
      CORBA::PolicyList policies (3);
      policies.length (3);

      // Id Assignment Policy
      policies[0] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID);

      // Lifespan policy
      policies[1] =
        root_poa->create_lifespan_policy (PortableServer::PERSISTENT);

      // Threading policy
      policies[2] =
        root_poa->create_thread_policy (PortableServer::ORB_CTRL_MODEL);

      // Create the firstPOA under the RootPOA.
      PortableServer::POA_var first_poa =
        root_poa->create_POA ("firstPOA",
                              poa_manager.in (),
                              policies);

      policies[2]->destroy ();

      // Threading policy
      policies[2] =
        root_poa->create_thread_policy (PortableServer::SINGLE_THREAD_MODEL);

      // Create the secondPOA under the firstPOA.
      PortableServer::POA_var second_poa =
        first_poa->create_POA ("secondPOA",
                               poa_manager.in (),
                               policies);

      // Creation of POAs is over. Destroy the Policy objects.
      for (CORBA::ULong i = 0;
           i < policies.length ();
           ++i)
        {
          policies[i]->destroy ();
        }

      // Create two Objects of Class test_i (defined in
      // ./../GenericServant/test_i.h) Create one object at RootPOA
      // and the other at firstPOA.
      test_i first_servant (orb.in (),
                            root_poa.in ());
      test_i second_servant (orb.in (),
                             first_poa.in ());

      // Do "activate_object" to activate the first_servant object.  It
      // returns ObjectId for that object.  Operation Used :
      //  ObjectId activate_object(in Servant p_servant)
      //    raises (ServantAlreadyActive, WrongPolicy);
      PortableServer::ObjectId_var first_oid =
        root_poa->activate_object (&first_servant);

      // Get Object Reference for the first_servant object.
      test_var first_test =
        first_servant._this ();

      // Get ObjectId for object secondtest and use that ObjectId to
      // activate the second_servant object.
      // Operation Used :
      //  void activate_object_with_id(in ObjectId oid, in Servant p_servant)
      //       raises (ObjectAlreadyActive, ServantAlreadyActive, WrongPolicy);
      PortableServer::ObjectId_var second_oid =
        PortableServer::string_to_ObjectId ("second test");

      first_poa->activate_object_with_id (second_oid.in (),
                                          &second_servant);

      // Get Object reference for second_servant object.
      test_var second_test =
        second_servant._this ();

      // Get ObjectId for the string thirdPOA Create the object reference
      // for thirdPOA using that ObjectId.  Operation Used :
      //   Object create_reference_with_id (in ObjectId oid, in CORBA::RepositoryId intf );
      // This operation creates an object reference that encapsulates the
      // specified Object Id and interface repository Id values.
      // This will test how the POA handles a user given ID
      PortableServer::ObjectId_var third_oid =
        PortableServer::string_to_ObjectId ("third test");

      third_oid[5] = static_cast<CORBA::Octet> ('\0');

      CORBA::Object_var third_test =
        second_poa->create_reference_with_id (third_oid.in (),
                                              "IDL:test:1.0");

      // Stringyfy all the object references and print them out.
      CORBA::String_var first_ior =
        orb->object_to_string (first_test.in ());

      CORBA::String_var second_ior =
        orb->object_to_string (second_test.in ());

      CORBA::String_var third_ior =
        orb->object_to_string (third_test.in ());

      ACE_DEBUG ((LM_DEBUG,
                  "%C\n%C\n%C\n",
                  first_ior.in (),
                  second_ior.in (),
                  third_ior.in ()));

      std::basic_string<ACE_TCHAR> ior_file_1 (options.ior_output_file);
      std::basic_string<ACE_TCHAR> ior_file_2 (options.ior_output_file);
      std::basic_string<ACE_TCHAR> ior_file_3 (options.ior_output_file);
      ior_file_1 += ACE_TEXT ("_1");
      ior_file_2 += ACE_TEXT ("_2");
      ior_file_3 += ACE_TEXT ("_3");

      int write_result = write_ior_to_file (ior_file_1, first_ior.in ());
      if (write_result == 0)
        write_result = write_ior_to_file (ior_file_2, second_ior.in ());
      if (write_result == 0)
        write_result = write_ior_to_file (ior_file_3, third_ior.in ());
      if (write_result != 0)
        return write_result;

      // Activate third servant using its ObjectID.
      test_i third_servant (orb.in (),
                            second_poa.in ());
      second_poa->activate_object_with_id (third_oid.in (),
                                           &third_servant);

      poa_manager->activate ();

      orb->run ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
