// $Id$

// ==================================================
// = LIBRARY
//    TAO/tests/POA/NewPOA
//
// = FILENAME
//    NewPOA.cpp
//
// = DESCRIPTION
//
//    This program demonstrates creation of new POAs using TAO
//    specific locking policies
//
// = AUTHOR
//    Irfan Pyarali
// ==================================================

#include "ace/streams.h"
#include "tao/corba.h"

int
main (int argc, char **argv)
{
  CORBA::Environment env;

  // The first step -- Initialize the ORB.
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0, env);

  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB_init");
      return -1;
    }

  // Obtain the object reference to the RootPOA.
  CORBA::Object_var obj =
    orb->resolve_initial_references ("RootPOA");

  // _narrow () the Object to get the POA object, i.e., the root_poa.
  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (obj.in (), env);

  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }

  // Policies for the new POAs
  CORBA::PolicyList policies (1);
  policies.length (1);

  // TAO specific threading policy
  policies[0] =
    root_poa->create_TAO_POA_locking_policy (PortableServer::USE_THREAD_LOCK,
                                             env);

  if (env.exception () != 0)
    {
      env.print_exception ("create_TAO_POA_locking");
      return -1;
    }

  // Creation of the firstPOA
  ACE_CString name = "firstPOA";
  PortableServer::POA_var first_poa =
    root_poa->create_POA (name.c_str (),
                          PortableServer::POAManager::_nil (),
                          policies,
                          env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_POA");
      return -1;
    }

  // TAO specific threading policy
  policies[0] =
    root_poa->create_TAO_POA_locking_policy (PortableServer::USE_NULL_LOCK, env);

  if (env.exception () != 0)
    {
      env.print_exception ("create_TAO_POA_locking");
      return -1;
    }

  // Creation of the new POA, i.e. firstPOA/secondPOA
  name += TAO_POA::name_separator ();
  name += "secondPOA";
  PortableServer::POA_var second_poa =
    root_poa->create_POA (name.c_str (),
                          PortableServer::POAManager::_nil (),
                          policies,
                          env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_POA");
      return -1;
    }

  // Creation of the new POAs over, so destroy the Policy_ptr's.
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

  // Get the names of all the POAs and print them out.

  CORBA::String_var root_poa_name =
    root_poa->the_name (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }

  CORBA::String_var first_poa_name =
    first_poa->the_name (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }

  CORBA::String_var second_poa_name =
    second_poa->the_name (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "%s\n%s\n%s\n",
              root_poa_name.in (),
              first_poa_name.in (),
              second_poa_name.in ()));
  
  // This should destroy all its children
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
