// $Id$

// =========================================================================
// = LIBRARY
//    TAO/tests/POA/TIE
//
// = FILENAME
//    server.cpp
//
// = DESCRIPTION
//    In this example,
//
// = AUTHOR
//    Irfan Pyarali
// =========================================================================

#include "ace/streams.h"
#include "Foo_i.h"

ACE_RCSID(TIE, server, "$Id$")

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

  // Lifespan policy
  policies[0] = 
    root_poa->create_lifespan_policy (PortableServer::PERSISTENT, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_lifespan_policy");
      return -1;
    }

  // Implicit activation policy
  policies[1] = 
    root_poa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_implicit_activation_policy");
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

  // Create first Foo_i
  Foo_i first_foo_impl (27, first_poa.in ());

  // Create second Foo_i
  Outer_i::Foo_i second_foo_impl (28, first_poa.in ());

  // Create third Foo_i
  Outer_i::Inner_i::Foo_i third_foo_impl (29, first_poa.in ());

  // Create forth Foo_i
  Simple_Foo_i forth_foo_tie (30);
  POA_Foo_tie <Simple_Foo_i> forth_foo_impl (forth_foo_tie, first_poa.in ());

#if defined (ACE_HAS_USING_KEYWORD)

  // Create fifth Foo_i
  Simple_Foo_i fifth_foo_tie (31);
  POA_Outer::Foo_tie <Simple_Foo_i> fifth_foo_impl (fifth_foo_tie, first_poa.in ());

  // Create sixth Foo_i
  Simple_Foo_i sixth_foo_tie (32);
  POA_Outer::Inner::Foo_tie <Simple_Foo_i> sixth_foo_impl (sixth_foo_tie, first_poa.in ());

#endif /* ACE_HAS_USING_KEYWORD */

  // Get Object Reference for the first_foo_impl object.
  Foo_var first_foo = first_foo_impl._this (env);

  if (env.exception () != 0)
    {
      env.print_exception ("POA_Foo::_this");
      return -1;
    }

  // Get Object Reference for the second_foo_impl object.
  Outer::Foo_var second_foo = second_foo_impl._this (env);

  if (env.exception () != 0)
    {
      env.print_exception ("POA_Foo::_this");
      return -1;
    }

  // Get Object Reference for the third_foo_impl object.
  Outer::Inner::Foo_var third_foo = third_foo_impl._this (env);

  if (env.exception () != 0)
    {
      env.print_exception ("POA_Foo::_this");
      return -1;
    }

  // Get Object Reference for the forth_foo_impl object.
  Foo_var forth_foo = forth_foo_impl._this (env);

  if (env.exception () != 0)
    {
      env.print_exception ("POA_Foo::_this");
      return -1;
    }

#if defined (ACE_HAS_USING_KEYWORD)

  // Get Object Reference for the fifth_foo_impl object.
  Outer::Foo_var fifth_foo = fifth_foo_impl._this (env);

  if (env.exception () != 0)
    {
      env.print_exception ("POA_Foo::_this");
      return -1;
    }

  // Get Object Reference for the sixth_foo_impl object.
  Outer::Inner::Foo_var sixth_foo = sixth_foo_impl._this (env);

  if (env.exception () != 0)
    {
      env.print_exception ("POA_Foo::_this");
      return -1;
    }

#endif /* ACE_HAS_USING_KEYWORD */

  // Stringyfy all the object references and print them out.
  CORBA::String_var first_ior =
    orb->object_to_string (first_foo.in (), env);

  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }

  // Stringyfy all the object references and print them out.
  CORBA::String_var second_ior =
    orb->object_to_string (second_foo.in (), env);

  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }

  // Stringyfy all the object references and print them out.
  CORBA::String_var third_ior =
    orb->object_to_string (third_foo.in (), env);

  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }

  // Stringyfy all the object references and print them out.
  CORBA::String_var forth_ior =
    orb->object_to_string (forth_foo.in (), env);

  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }

#if defined (ACE_HAS_USING_KEYWORD)

  // Stringyfy all the object references and print them out.
  CORBA::String_var fifth_ior =
    orb->object_to_string (fifth_foo.in (), env);

  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }

  // Stringyfy all the object references and print them out.
  CORBA::String_var sixth_ior =
    orb->object_to_string (sixth_foo.in (), env);

  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }

#endif /* ACE_HAS_USING_KEYWORD */

  ACE_DEBUG ((LM_DEBUG, 
              "%s\n%s\n%s\n%s\n",
              first_ior.in (),
              second_ior.in (),
              third_ior.in (),
              forth_ior.in ()));

#if defined (ACE_HAS_USING_KEYWORD)

  ACE_DEBUG ((LM_DEBUG, 
              "%s\n%s\n",
              fifth_ior.in (),
              sixth_ior.in ()));

#endif /* ACE_HAS_USING_KEYWORD */

  poa_manager->activate (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POAManager::activate");
      return -1;
    }

  if (orb->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "CORBA::ORB::run"), -1);

  // Destroy RootPOA.
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
