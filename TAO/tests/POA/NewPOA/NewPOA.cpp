#include "ace/streams.h"
#include "tao/corba.h"

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

  PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj.in(), env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }
  
  // CORBA::PolicyList policies (2);
  PortableServer::PolicyList policies (2);
  policies.length (2);  
  policies[0] = root_poa->create_thread_policy (PortableServer::ORB_CTRL_MODEL, env);
  policies[1] = root_poa->create_lifespan_policy (PortableServer::TRANSIENT, env);
  
  ACE_CString name = "firstPOA";
  PortableServer::POA_var first_poa = root_poa->create_POA (name.c_str (),
                                                            PortableServer::POAManager::_nil(), 
                                                            policies,
                                                            env);  
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_POA");
      return -1;
    }

  name += TAO_POA::name_separator ();
  name += "secondPOA";
  PortableServer::POA_var second_poa = root_poa->create_POA (name.c_str (),
                                                             PortableServer::POAManager::_nil(), 
                                                             policies,
                                                             env);  
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_POA");
      return -1;
    }

  name = "thirdPOA";
  name += TAO_POA::name_separator ();
  name += "forthPOA";
  name += TAO_POA::name_separator ();
  name += "fifthPOA";

  PortableServer::POA_var fifth_poa = root_poa->create_POA (name.c_str (),
                                                            PortableServer::POAManager::_nil(), 
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

  CORBA::String_var root_poa_name = root_poa->the_name (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }
  
  CORBA::String_var first_poa_name = first_poa->the_name (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }
  
  CORBA::String_var second_poa_name = second_poa->the_name (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }
  
  CORBA::String_var fifth_poa_name = fifth_poa->the_name (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }
  
  cout << root_poa_name.in () << endl;
  cout << first_poa_name.in () << endl;
  cout << second_poa_name.in () << endl;
  cout << fifth_poa_name.in () << endl;

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
