// $Id$

// ===========================================================================================
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
//       ./../Generic_Servant/MyFooSerVant) are used as sample objects. 
//
// = AUTHOR
//    Irfan Pyarali
// ===========================================================================================

#include "ace/streams.h"
#include "MyFooServant.h"

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

  // Creation of firstPOA is over. Destroy the Policy objects.
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
  MyFooServant first_foo_impl (root_poa.in (), 27);
  MyFooServant second_foo_impl (first_poa.in (), 28);

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
    first_poa->create_reference_with_id (third_oid.in (),
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

  cout << first_ior.in () << endl;
  cout << second_ior.in () << endl;
  cout << third_ior.in () << endl;

  // Activate thirdPOA using its ObjectID.
  MyFooServant third_foo_impl (first_poa.in (), 29);
  first_poa->activate_object_with_id (third_oid.in (),
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
  root_poa->destroy (CORBA::B_TRUE,
                     CORBA::B_TRUE,
                     env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::destroy");
      return -1;
    }

  return 0;
}
