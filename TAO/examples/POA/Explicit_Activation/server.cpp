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
//    In this example,
//     - A new POA ( firstPOA) is created, and the different functions
//       for the explicit activation of objects are demonstrated.
//     - The Foo application class objects (defined in
//       ./../Generic_Servant/MyFooServant) are used as sample objects.
//
// = AUTHOR
//    Irfan Pyarali
//
// ================================================================

#include "ace/streams.h"
#include "ace/Get_Opt.h"
#include "tao/Timeprobe.h"
#include "MyFooServant.h"

ACE_RCSID(Explicit_Activation, server, "$Id$")

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
                           "[-f ior_output_file]"
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
  if (ior_output_file == 0)
    // No filename was specified; simply return
    return 0;

  char ior_output_file_1[BUFSIZ];
  char ior_output_file_2[BUFSIZ];
  char ior_output_file_3[BUFSIZ];

  ACE_OS::sprintf (ior_output_file_1, "%s_1", ior_output_file);
  ACE_OS::sprintf (ior_output_file_2, "%s_2", ior_output_file);
  ACE_OS::sprintf (ior_output_file_3, "%s_3", ior_output_file);

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
                       "ACE_OS::fprintf failed while writing %s to %s\n",
                       first_ior,
                       ior_output_file_1),
                      -1);

  result = ACE_OS::fprintf (output_file_2,
                            "%s",
                            second_ior);
  if (result != ACE_OS::strlen (second_ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_OS::fprintf failed while writing %s to %s\n",
                       second_ior,
                       ior_output_file_2),
                      -1);

  result = ACE_OS::fprintf (output_file_3,
                            "%s",
                            third_ior);
  if (result != ACE_OS::strlen (third_ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_OS::fprintf failed while writing %s to %s\n",
                       third_ior,
                       ior_output_file_3),
                      -1);

  ACE_OS::fclose (output_file_1);
  ACE_OS::fclose (output_file_2);
  ACE_OS::fclose (output_file_3);

  return 0;
}

int
main (int argc, char **argv)
{
  CORBA::Environment env;

  // Initialize the ORB first.
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0, env);

  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB_init");
      return -1;
    }

  int result = parse_args (argc, argv);
  if (result != 0)
    return result;

  // Obtain the RootPOA.
  CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");

  // Get the POA_var object from Object_var.
  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (obj.in (), env);

  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }

  // Get the POAManager of the RootPOA.
  PortableServer::POAManager_var poa_manager =
    root_poa->the_POAManager (env);

  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::the_POAManager");
      return -1;
    }

  // Policies for the firstPOA to be created.
  CORBA::PolicyList policies (2);
  policies.length (2);

  // Id Assignment Policy
  policies[0] =
    root_poa->create_id_assignment_policy (PortableServer::USER_ID, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_id_assignment_policy");
      return -1;
    }

  // Lifespan policy
  policies[1] =
    root_poa->create_lifespan_policy (PortableServer::PERSISTENT, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_lifespan_policy");
      return -1;
    }

  // Create the firstPOA under the RootPOA.
  ACE_CString name = "firstPOA";
  PortableServer::POA_var first_poa =
    root_poa->create_POA (name.c_str (),
                          poa_manager.in (),
                          policies,
                          env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_POA");
      return -1;
    }

  // Create the secondPOA under the firstPOA.
  name = "secondPOA";
  PortableServer::POA_var second_poa =
    first_poa->create_POA (name.c_str (),
                           poa_manager.in (),
                           policies,
                           env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_POA");
      return -1;
    }

  // Creation of POAs is over. Destroy the Policy objects.
  for (CORBA::ULong i = 0;
       i < policies.length () && env.exception () == 0;
       ++i)
    {
      CORBA::Policy_ptr policy = policies[i];
      policy->destroy (env);
    }
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_POA");
      return -1;
    }

  // Create two Objects of Class MyFooServant (defined in
  // ./../GenericServant/MyFooServant.h) Create one object at RootPOA
  // and the other at firstPOA.
  MyFooServant first_foo_impl (orb.in (), root_poa.in (), 27);
  MyFooServant second_foo_impl (orb.in (), first_poa.in (), 28);

  // Do "activate_object" to activate the first_foo_impl object.  It
  // returns ObjectId for that object.  Operation Used :
  //  ObjectId activate_object( in Servant p_servant)
  //    raises (ServantAlreadyActive, WrongPolicy);
  PortableServer::ObjectId_var first_oid =
    root_poa->activate_object (&first_foo_impl, env);

  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::activate_object");
      return -1;
    }

  // Get Object Reference for the first_foo_impl object.
  Foo_var first_foo = first_foo_impl._this (env);

  if (env.exception () != 0)
    {
      env.print_exception ("POA_Foo::_this");
      return -1;
    }

  // Get ObjectId for object secondFoo and use that ObjectId to
  // activate the second_foo_impl object.
  // Operation Used :
  //  void activate_object_with_id( in ObjectId oid, in Servant p_servant)
  //       raises (ObjectAlreadyActive, ServantAlreadyActive, WrongPolicy);
  PortableServer::ObjectId_var second_oid =
    PortableServer::string_to_ObjectId ("secondFoo");

  first_poa->activate_object_with_id (second_oid.in (),
                                      &second_foo_impl,
                                      env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::activate_object_with_id");
      return -1;
    }

  // Get Object reference for second_foo_impl object.
  Foo_var second_foo = second_foo_impl._this (env);
  if (env.exception () != 0)
    {
      env.print_exception ("POA_Foo::_this");
      return -1;
    }

  // Get ObjectId for the string thirdPOA Create the object reference
  // for thirdPOA using that ObjectId.  Operation Used :
  //   Object create_reference_with_id (in ObjectId oid, in CORBA::RepositoryId intf );
  // This operation creates an object reference that encapsulates the
  // specified Object Id and interface repository Id values.
  /*
  PortableServer::ObjectId_var third_oid =
    PortableServer::string_to_ObjectId ("thirdFoo");
  */

  // This will test how the POA handles a user given ID
  PortableServer::ObjectId_var third_oid =
    PortableServer::string_to_ObjectId ("third Foo");

  third_oid[5] = (CORBA::Octet) '\0';

  CORBA::Object_var third_foo =
    second_poa->create_reference_with_id (third_oid.in (),
                                          "IDL:Foo:1.0", env);

  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_reference_with_id");
      return -1;
    }

  // Stringyfy all the object references and print them out.
  CORBA::String_var first_ior =
    orb->object_to_string (first_foo.in (), env);

  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }

  CORBA::String_var second_ior =
    orb->object_to_string (second_foo.in (), env);

  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }

  CORBA::String_var third_ior =
    orb->object_to_string (third_foo.in (), env);

  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "%s\n%s\n%s\n",
              first_ior.in (),
              second_ior.in (),
              third_ior.in ()));

  int write_result = write_iors_to_file (first_ior.in (),
                                         second_ior.in (),
                                         third_ior.in ());
  if (write_result != 0)
    return write_result;

  // Activate thirdPOA using its ObjectID.
  MyFooServant third_foo_impl (orb.in (), second_poa.in (), 29);
  second_poa->activate_object_with_id (third_oid.in (),
                                       &third_foo_impl,
                                       env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::activate_object_with_id");
      return -1;
    }

  poa_manager->activate (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POAManager::activate");
      return -1;
    }

  if (orb->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "CORBA::ORB::run"), -1);

  // Destroy RootPOA. ( Also destroys firstPOA)
  root_poa->destroy (1,
                     1,
                     env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::destroy");
      return -1;
    }

  ACE_TIMEPROBE_PRINT;

  return 0;
}
