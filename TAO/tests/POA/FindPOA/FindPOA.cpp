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
  
  TAO_Adapter_Activator activator_impl;
  PortableServer::AdapterActivator_var activator = activator_impl._this (env);
  if (env.exception () != 0)
    {
      env.print_exception ("TAO_Adapter_Activator::_this");
      return -1;
    }
  
  root_poa->the_activator (activator.in (), env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::the_activator");
      return -1;
    }
  
  ACE_CString name = "firstPOA";
  PortableServer::POA_var first_poa = root_poa->find_POA (name.c_str (),
                                                          CORBA::B_TRUE,
                                                          env);  
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::find_POA");
      return -1;
    }

  name += TAO_POA::name_separator ();
  name += "secondPOA";
  PortableServer::POA_var second_poa = root_poa->find_POA (name.c_str (),
                                                           CORBA::B_TRUE,
                                                           env);  
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::find_POA");
      return -1;
    }

  name = "thirdPOA";
  name += TAO_POA::name_separator ();
  name += "forthPOA";
  name += TAO_POA::name_separator ();
  name += "fifthPOA";

  PortableServer::POA_var fifth_poa = root_poa->find_POA (name.c_str (),
                                                          CORBA::B_TRUE,
                                                          env);  
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::find_POA");
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
