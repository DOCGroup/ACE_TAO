#include "ace/streams.h"
#include "Servant_Activator.h"
#include "Servant_Locator.h"

int 
main (int argc, char **argv)
{
  CORBA::Environment env;

  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0, env);
  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB_init");
      return -1;
    }

  CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");

  PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }
  
  PortableServer::POAManager_var poa_manager = root_poa->the_POAManager (env);
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
    policies[0] = root_poa->create_id_assignment_policy (PortableServer::USER_ID, env);
    policies[1] = root_poa->create_lifespan_policy (PortableServer::PERSISTENT, env);
    policies[2] = root_poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER, env);
    policies[3] = root_poa->create_servant_retention_policy (PortableServer::RETAIN, env);
    
    ACE_CString name = "firstPOA";
    first_poa = root_poa->create_POA (name.c_str (),
                                      poa_manager.in (),
                                      policies,
                                      env);  
    if (env.exception () != 0)
      {
        env.print_exception ("PortableServer::POA::create_POA");
        return -1;
      }
    
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
    policies[0] = root_poa->create_id_assignment_policy (PortableServer::USER_ID, env);
    policies[1] = root_poa->create_lifespan_policy (PortableServer::PERSISTENT, env);
    policies[2] = root_poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER, env);
    policies[3] = root_poa->create_servant_retention_policy (PortableServer::NON_RETAIN, env);
    
    ACE_CString name = "secondPOA";
    second_poa = root_poa->create_POA (name.c_str (),
                                      poa_manager.in (),
                                      policies,
                                      env);  
    if (env.exception () != 0)
      {
        env.print_exception ("PortableServer::POA::create_POA");
        return -1;
      }
    
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
  PortableServer::ServantActivator_var servant_activator = servant_activator_impl._this (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POAManager::_this");
      return -1;
    }

  first_poa->set_servant_manager (servant_activator.in (), env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POAManager::set_servant_manager");
      return -1;
    }

  PortableServer::ObjectId_var first_foo_oid = PortableServer::string_to_ObjectId ("firstFoo");
  CORBA::Object_var first_foo = first_poa->create_reference_with_id (first_foo_oid.in (), "IDL:Foo:1.0", env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_reference_with_id");
      return -1;
    }
  
  MyFooServantLocator servant_locator_impl;
  PortableServer::ServantLocator_var servant_locator = servant_locator_impl._this (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POAManager::_this");
      return -1;
    }

  second_poa->set_servant_manager (servant_locator.in (), env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POAManager::set_servant_manager");
      return -1;
    }

  PortableServer::ObjectId_var second_foo_oid = PortableServer::string_to_ObjectId ("secondFoo");
  CORBA::Object_var second_foo = second_poa->create_reference_with_id (second_foo_oid.in (), "IDL:Foo:1.0", env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_reference_with_id");
      return -1;
    }
  
  CORBA::String_var first_foo_ior = orb->object_to_string (first_foo, env);
  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }

  CORBA::String_var second_foo_ior = orb->object_to_string (second_foo, env);
  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }

  cout << first_foo_ior.in () << endl;
  cout << second_foo_ior.in () << endl;

  poa_manager->activate (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POAManager::activate");
      return -1;
    }

  if (orb->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "CORBA::ORB::run"), -1);

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
