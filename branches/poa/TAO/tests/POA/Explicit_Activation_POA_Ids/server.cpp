#include "ace/streams.h"
#include "MyFooServant.h"

int 
main (int argc, char **argv)
{
  CORBA::Environment env;

  CORBA::ORB_ptr orb = CORBA::ORB_init (argc, argv, 0, env);
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
  
  MyFooServant afoo (rootPOA, 27);
  PortableServer::ObjectId_var oid = rootPOA->activate_object (&afoo, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::activate_object");
      return -1;
    }
  
  Foo_var foo = afoo._this (env);
  if (env.exception () != 0)
    {
      env.print_exception ("POA_Foo::_this");
      return -1;
    }
  
  CORBA::String_var ior = orb->object_to_string (foo, env);
  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }

  cout << (char *) ior << endl;

  rootPOA->the_POAManager (env)->activate (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POAManager::activate");
      return -1;
    }

  if (orb->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "CORBA::ORB::run"), -1);

  CORBA::release (orb);

  return 0;
}
