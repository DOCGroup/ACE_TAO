// $Id$

// ================================================================
//
// = LIBRARY
//    TAO/tests/POA/Explicit_Activation
//
// = FILENAME
//    server.cpp
//
// = DESCRIPTION
//    Explicit creation of servants.
//
// = AUTHOR
//    Irfan Pyarali
//
// ================================================================

#include "ace/Get_Opt.h"
#include "ace/SString.h"
#include "test_i.h"
#include "ace/OS_NS_stdio.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("ior");

static int
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("f:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f':
        ior_output_file = get_opts.opt_arg ();
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
write_iors_to_file (const char *first_ior,
                    const char *second_ior,
                    const char *third_ior)
{
  ACE_TCHAR ior_output_file_1[BUFSIZ];
  ACE_TCHAR ior_output_file_2[BUFSIZ];
  ACE_TCHAR ior_output_file_3[BUFSIZ];

  ACE_OS::sprintf (ior_output_file_1, ACE_TEXT("%s_1"), ior_output_file);
  ACE_OS::sprintf (ior_output_file_2, ACE_TEXT("%s_2"), ior_output_file);
  ACE_OS::sprintf (ior_output_file_3, ACE_TEXT("%s_3"), ior_output_file);

  FILE *output_file_1 = ACE_OS::fopen (ior_output_file_1, "w");
  FILE *output_file_2 = ACE_OS::fopen (ior_output_file_2, "w");
  FILE *output_file_3 = ACE_OS::fopen (ior_output_file_3, "w");

  if (output_file_1 == 0 ||
      output_file_2 == 0 ||
      output_file_3 == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Cannot open output files for writing IORs: %s, %s %s\n",
                       ior_output_file_1,
                       ior_output_file_2,
                       ior_output_file_3),
                      -1);

  u_int result = 0;

  result = ACE_OS::fprintf (output_file_1,
                            "%s",
                            first_ior);
  if (result != ACE_OS::strlen (first_ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_OS::fprintf failed while writing %C to %s\n",
                       first_ior,
                       ior_output_file_1),
                      -1);

  result = ACE_OS::fprintf (output_file_2,
                            "%s",
                            second_ior);
  if (result != ACE_OS::strlen (second_ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_OS::fprintf failed while writing %C to %s\n",
                       second_ior,
                       ior_output_file_2),
                      -1);

  result = ACE_OS::fprintf (output_file_3,
                            "%s",
                            third_ior);
  if (result != ACE_OS::strlen (third_ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_OS::fprintf failed while writing %C to %s\n",
                       third_ior,
                       ior_output_file_3),
                      -1);

  ACE_OS::fclose (output_file_1);
  ACE_OS::fclose (output_file_2);
  ACE_OS::fclose (output_file_3);

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

      int result = parse_args (argc, argv);
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
      ACE_CString name = "firstPOA";
      PortableServer::POA_var first_poa =
        root_poa->create_POA (name.c_str (),
                              poa_manager.in (),
                              policies);

      policies[2]->destroy ();

      // Threading policy
      policies[2] =
        root_poa->create_thread_policy (PortableServer::SINGLE_THREAD_MODEL);

      // Create the secondPOA under the firstPOA.
      name = "secondPOA";
      PortableServer::POA_var second_poa =
        first_poa->create_POA (name.c_str (),
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
      /*
        PortableServer::ObjectId_var third_oid =
        PortableServer::string_to_ObjectId ("thirdtest");
      */

      // This will test how the POA handles a user given ID
      PortableServer::ObjectId_var third_oid =
        PortableServer::string_to_ObjectId ("third test");

      third_oid[5] = (CORBA::Octet) '\0';

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

      int write_result =
        write_iors_to_file (first_ior.in (),
                            second_ior.in (),
                            third_ior.in ());
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
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
