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

  PortableServer::POA_var rootPOA = PortableServer::POA::_narrow (obj, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }
  
  // CORBA::PolicyList policies (2);
  PortableServer::PolicyList policies (2);

  policies[0] = rootPOA->create_thread_policy (PortableServer::ORB_CTRL_MODEL, env);
  policies[1] = rootPOA->create_lifespan_policy (PortableServer::TRANSIENT, env);
  policies.length (2);  
  
  PortableServer::POA_ptr childPOA = rootPOA->create_POA ("my_little_poa",
                                                          PortableServer::POAManager::_nil(), 
                                                          policies,
                                                          env);  
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_POA");
      return -1;
    }

  for (int i = 0; i < policies.length () && env.exception == 0; i++)
    {
      // const CORBA::Policy_ptr policy = policies[i];
      const PortableServer::Policy_ptr policy = policies[i];
      policy->destroy (env);
    }  
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_POA");
      return -1;
    }

  CORBA::String_var rootPOA_name = rootPOA->the_name (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }
  
  CORBA::String_var childPOA_name = childPOA->the_name (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }
  
  cout << rootPOA_name.in () << endl;
  cout << childPOA_name.in () << endl;

  childPOA->destroy (CORBA::B_TRUE,
                     CORBA::B_TRUE,
                     env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::destroy");
      return -1;
    }

  return 0;
}
