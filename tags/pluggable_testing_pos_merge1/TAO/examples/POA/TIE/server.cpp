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

  // Create A_i
  A_i a_impl (27, first_poa.in ());

  // Create B_i
  Outer_i::B_i b_impl (28, first_poa.in ());

  // Create C_i
  Outer_i::Inner_i::C_i c_impl (29, first_poa.in ());

  // Create A tie
  Tie_i a_tie_i (30);
  POA_A_tie <Tie_i> a_tie_impl (a_tie_i, first_poa.in ());

#if defined (ACE_HAS_USING_KEYWORD)

  // Create B tie
  Tie_i b_tie_i (31);
  POA_Outer::B_tie <Tie_i> b_tie_impl (b_tie_i, first_poa.in ());

  // Create C tie
  Tie_i c_tie_i (32);
  POA_Outer::Inner::C_tie <Tie_i> c_tie_impl (c_tie_i, first_poa.in ());

#endif /* ACE_HAS_USING_KEYWORD */

  // Get Object Reference for the a_impl object.
  A_var a = a_impl._this (env);

  if (env.exception () != 0)
    {
      env.print_exception ("POA_A::_this");
      return -1;
    }

  // Get Object Reference for the b_impl object.
  Outer::B_var b = b_impl._this (env);

  if (env.exception () != 0)
    {
      env.print_exception ("POA_Outer::B::_this");
      return -1;
    }

  // Get Object Reference for the c_impl object.
  Outer::Inner::C_var c = c_impl._this (env);

  if (env.exception () != 0)
    {
      env.print_exception ("POA_Outer::Inner::C::_this");
      return -1;
    }

  // Get Object Reference for the a_tie_impl object.
  A_var a_tie = a_tie_impl._this (env);

  if (env.exception () != 0)
    {
      env.print_exception ("POA_A::_this");
      return -1;
    }

#if defined (ACE_HAS_USING_KEYWORD)

  // Get Object Reference for the a_tie_impl object.
  Outer::B_var b_tie = b_tie_impl._this (env);

  if (env.exception () != 0)
    {
      env.print_exception ("POA_Outer::B::_this");
      return -1;
    }

  // Get Object Reference for the c_tie_impl object.
  Outer::Inner::C_var c_tie = c_tie_impl._this (env);

  if (env.exception () != 0)
    {
      env.print_exception ("POA_Outer::Inner::C::_this");
      return -1;
    }

#endif /* ACE_HAS_USING_KEYWORD */

  // Stringyfy all the object references and print them out.
  CORBA::String_var first_ior =
    orb->object_to_string (a.in (), env);

  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }

  // Stringyfy all the object references and print them out.
  CORBA::String_var second_ior =
    orb->object_to_string (b.in (), env);

  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }

  // Stringyfy all the object references and print them out.
  CORBA::String_var third_ior =
    orb->object_to_string (c.in (), env);

  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }

  // Stringyfy all the object references and print them out.
  CORBA::String_var forth_ior =
    orb->object_to_string (a_tie.in (), env);

  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }

#if defined (ACE_HAS_USING_KEYWORD)

  // Stringyfy all the object references and print them out.
  CORBA::String_var fifth_ior =
    orb->object_to_string (b_tie.in (), env);

  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }

  // Stringyfy all the object references and print them out.
  CORBA::String_var sixth_ior =
    orb->object_to_string (c_tie.in (), env);

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
  root_poa->destroy (1,
                     1,
                     env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::destroy");
      return -1;
    }

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class POA_A_tie<Tie_i>;
#if defined (ACE_HAS_USING_KEYWORD)
template class POA_Outer::B_tie <Tie_i>;
template class POA_Outer::Inner::C_tie <Tie_i>;
#endif /* ACE_HAS_USING_KEYWORD */
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate POA_A_tie<Tie_i>
#if defined (ACE_HAS_USING_KEYWORD)
#pragma instantiate POA_Outer::B_tie <Tie_i>
#pragma instantiate POA_Outer::Inner::C_tie <Tie_i>
#endif /* ACE_HAS_USING_KEYWORD */
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
