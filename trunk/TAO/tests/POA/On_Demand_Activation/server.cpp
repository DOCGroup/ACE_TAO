// $Id$

//============================================================================
//
//  =FILENAME
//     server.cpp
//
//  =DESCRIPTION
//     Server to test the Servant Activator and Servant Locator for a POA.
//
//  =AUTHOR
//     Irfan Pyarali
//
//=============================================================================

#include "ace/streams.h"
#include "Servant_Activator.h"
#include "Servant_Locator.h"

int
main (int argc, char **argv)
{
  CORBA::Environment env;

  // Initialize the ORB.
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0, env);

  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB_init");
      return -1;
    }

  // Get an Object reference to RootPOA.
  CORBA::Object_var obj =
    orb->resolve_initial_references ("RootPOA");

  // Narrow the Object reference to a POA reference
  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (obj.in (), env);

  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }

  // Get the POAManager of RootPOA
  PortableServer::POAManager_var poa_manager =
    root_poa->the_POAManager (env);

  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::the_POAManager");
      return -1;
    }

  PortableServer::POA_var first_poa;
  {
    // CORBA::PolicyList policies (4);
    PortableServer::PolicyList policies (4);
    policies.length (4);
    policies[0] =
      root_poa->create_id_assignment_policy (PortableServer::USER_ID, env);
    policies[1] =
      root_poa->create_lifespan_policy (PortableServer::PERSISTENT, env);
    policies[2] =
      root_poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER, env);
    policies[3] =
      root_poa->create_servant_retention_policy (PortableServer::RETAIN, env);

    ACE_CString name = "firstPOA";

    // Create firstPOA as the child of RootPOA with the above policies
    // firstPOA will use SERVANT_ACTIVATOR because of RETAIN policy.
    first_poa = root_poa->create_POA (name.c_str (),
                                      poa_manager.in (),
                                      policies,
                                      env);
    if (env.exception () != 0)
      {
        env.print_exception ("PortableServer::POA::create_POA");
        return -1;
      }

    // Destroy the policy objects as they have been passed to
    // create_POA and no longer needed.

    for (CORBA::ULong i = 0;
         i < policies.length () && env.exception () == 0;
         ++i)
      {
        PortableServer::Policy_ptr policy = policies[i];
        policy->destroy (env);
      }

    if (env.exception () != 0)
      {
        env.print_exception ("PortableServer::POA::create_POA");
        return -1;
      }
  }

  PortableServer::POA_var second_poa;
  {
    // CORBA::PolicyList policies (4);
    PortableServer::PolicyList policies (4);
    policies.length (4);
    policies[0] =
      root_poa->create_id_assignment_policy (PortableServer::USER_ID, env);
    policies[1] =
      root_poa->create_lifespan_policy (PortableServer::PERSISTENT, env);
    policies[2] =
      root_poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER, env);
    policies[3] =
      root_poa->create_servant_retention_policy (PortableServer::NON_RETAIN, env);

    ACE_CString name = "secondPOA";

    // Create secondPOA as child of RootPOA with the above policies
    // secondPOA will use a SERVANT_LOCATOR because of NON_RETAIN
    // policy.
    second_poa = root_poa->create_POA (name.c_str (),
                                      poa_manager.in (),
                                      policies,
                                      env);
    if (env.exception () != 0)
      {
        env.print_exception ("PortableServer::POA::create_POA");
        return -1;
      }

    // Destroy the policy objects as they have been passed to
    // create_POA and no longer needed.

    for (CORBA::ULong i = 0;
         i < policies.length () && env.exception () == 0;
         ++i)
      {
        PortableServer::Policy_ptr policy = policies[i];
        policy->destroy (env);
      }

    if (env.exception () != 0)
      {
        env.print_exception ("PortableServer::POA::create_POA");
        return -1;
      }
  }

  MyFooServantActivator servant_activator_impl;
  PortableServer::ServantActivator_var servant_activator =
    servant_activator_impl._this (env);

  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POAManager::_this");
      return -1;
    }

  // Set MyFooServantActivator object as the servant_manager of
  // firstPOA.

  first_poa->set_servant_manager (servant_activator.in (), env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POAManager::set_servant_manager");
      return -1;
    }

  // Create a reference with user created ID in firstPOA which uses
  // the MyFooServantActivator.

  PortableServer::ObjectId_var first_foo_oid =
    PortableServer::string_to_ObjectId ("firstFoo");

  CORBA::Object_var first_foo =
    first_poa->create_reference_with_id (first_foo_oid.in (), "IDL:Foo:1.0", env);

  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_reference_with_id");
      return -1;
    }

  MyFooServantLocator servant_locator_impl;
  PortableServer::ServantLocator_var servant_locator =
    servant_locator_impl._this (env);

  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POAManager::_this");
      return -1;
    }

  // Set MyFooServantLocator object as the servant Manager of
  // secondPOA.

  second_poa->set_servant_manager (servant_locator.in (), env);

  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POAManager::set_servant_manager");
      return -1;
    }

  // Try to create a reference with user created ID in second_poa
  // which uses MyFooServantLocator.

  PortableServer::ObjectId_var second_foo_oid =
    PortableServer::string_to_ObjectId ("secondFoo");

  CORBA::Object_var second_foo =
    second_poa->create_reference_with_id (second_foo_oid.in (),
                                          "IDL:Foo:1.0", env);

  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_reference_with_id");
      return -1;
    }

  // Invoke object_to_string on the references created in firstPOA and
  // secondPOA.

  CORBA::String_var first_foo_ior = 
    orb->object_to_string (first_foo.in (), env);

  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }

  CORBA::String_var second_foo_ior = 
    orb->object_to_string (second_foo.in (), env);

  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }

  // Print the ior's of first_foo and second_foo.

  ACE_DEBUG((LM_DEBUG,"%s\n%s\n",
	     first_foo_ior.in (),
	     second_foo_ior.in ()));

  // Set the poa_manager state to active, ready to process requests.
  poa_manager->activate (env);

  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POAManager::activate");
      return -1;
    }

  // Run the ORB.
  if (orb->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "CORBA::ORB::run"), -1);

  // Destroy the root_poa and also first_poa and second_poa

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

