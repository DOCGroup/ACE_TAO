// $Id$

// ===========================================================================================
// = LIBRARY
//    TAO/tests/POA/Forwarding
//
// = FILENAME
//    server.cpp
//
// = DESCRIPTION
//
// = AUTHOR
//    Irfan Pyarali
// ===========================================================================================

#include "MyFooServant.h"

static char *forward_to_IOR = 0;

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "f:O:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f':
        forward_to_IOR = get_opts.optarg;
        break;
      }

  // Indicates successful parsing of command line.
  return 0;
}

int
main (int argc, char **argv)
{
  int result = parse_args (argc, argv);
  if (result == -1)
    return -1;

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

  // Policies for the childPOA to be created.
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

  // Create the childPOA under the RootPOA.
  ACE_CString name = "childPOA";
  PortableServer::POA_var child_poa =
    root_poa->create_POA (name.c_str (),
                          poa_manager.in (),
                          policies,
                          env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_POA");
      return -1;
    }

  // Creation of childPOA is over. Destroy the Policy objects.
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

  
  CORBA::Object_var forward_to;

  if (forward_to_IOR != 0)
    {
      forward_to = orb->string_to_object (forward_to_IOR, env);
      
      if (env.exception () != 0)
        {
          env.print_exception ("ORB::string_to_object");
          return -1;
        }
    }

  // Create MyFooServant
  MyFooServant foo_impl (child_poa.in (), 
                         27,
                         forward_to.in ());

  // Create ObjectId and use that ObjectId to activate the foo_impl
  // object.
  //
  // Operation Used :
  //  void activate_object_with_id( in ObjectId oid, in Servant p_servant)
  //       raises (ObjectAlreadyActive, ServantAlreadyActive, WrongPolicy);
  PortableServer::ObjectId_var oid =
    PortableServer::string_to_ObjectId ("Foo");

  child_poa->activate_object_with_id (oid.in (),
                                      &foo_impl,
                                      env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::activate_object_with_id");
      return -1;
    }

  // Get Object reference for foo_impl object.
  Foo_var foo = foo_impl._this (env);
  if (env.exception () != 0)
    {
      env.print_exception ("POA_Foo::_this");
      return -1;
    }

  // Stringyfy the object reference and print it out.
  CORBA::String_var ior =
    orb->object_to_string (foo.in (), env);

  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG, "%s\n", ior.in ()));

  poa_manager->activate (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POAManager::activate");
      return -1;
    }

  if (orb->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "CORBA::ORB::run"), -1);

  // Destroy RootPOA. (Also destroys childPOA)
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
