#include "ace/streams.h"
#include "MyFooServant.h"

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
  
  // CORBA::PolicyList policies (2);
  PortableServer::PolicyList policies (2);
  policies.length (2);  
  policies[0] = root_poa->create_id_assignment_policy (PortableServer::USER_ID, env);
  policies[1] = root_poa->create_lifespan_policy (PortableServer::PERSISTENT, env);
  
  ACE_CString name = "firstPOA";
  PortableServer::POA_var first_poa = root_poa->create_POA (name.c_str (),
                                                            poa_manager.in (),
                                                            policies,
                                                            env);  
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_POA");
      return -1;
    }

  MyFooServant first_foo_impl (root_poa, 27);
  MyFooServant second_foo_impl (first_poa, 28);

  PortableServer::ObjectId_var first_oid = root_poa->activate_object (&first_foo_impl, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::activate_object");
      return -1;
    }
  
  Foo_var first_foo = first_foo_impl._this (env);
  if (env.exception () != 0)
    {
      env.print_exception ("POA_Foo::_this");
      return -1;
    }
  
  PortableServer::ObjectId_var second_oid = PortableServer::string_to_ObjectId ("secondFoo");
  first_poa->activate_object_with_id (second_oid.in (), 
                                      &second_foo_impl, 
                                      env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::activate_object_with_id");
      return -1;
    }
  
  Foo_var second_foo = second_foo_impl._this (env);
  if (env.exception () != 0)
    {
      env.print_exception ("POA_Foo::_this");
      return -1;
    }
  
  PortableServer::ObjectId_var third_oid = PortableServer::string_to_ObjectId ("thirdFoo");
  CORBA::Object_var third_foo = first_poa->create_reference_with_id (third_oid.in (), "IDL:Foo:1.0", env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_reference_with_id");
      return -1;
    }
  
  CORBA::String_var first_ior = orb->object_to_string (first_foo, env);
  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }

  CORBA::String_var second_ior = orb->object_to_string (second_foo, env);
  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }

  CORBA::String_var third_ior = orb->object_to_string (third_foo, env);
  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }

  cout << first_ior.in () << endl;
  cout << second_ior.in () << endl;
  cout << third_ior.in () << endl;

  MyFooServant third_foo_impl (first_poa, 29);
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
