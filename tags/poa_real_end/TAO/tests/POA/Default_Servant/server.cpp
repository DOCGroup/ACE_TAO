#include "ace/streams.h"
#include "File_i.h"

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

  PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj.in (), env);
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
  
  // CORBA::PolicyList policies (5);
  PortableServer::PolicyList policies (5);
  policies.length (5);  
  policies[0] = root_poa->create_id_assignment_policy (PortableServer::USER_ID, env);
  policies[1] = root_poa->create_lifespan_policy (PortableServer::PERSISTENT, env);
  policies[2] = root_poa->create_request_processing_policy (PortableServer::USE_DEFAULT_SERVANT, env);
  policies[3] = root_poa->create_servant_retention_policy (PortableServer::RETAIN, env);
  policies[4] = root_poa->create_id_uniqueness_policy (PortableServer::MULTIPLE_ID, env);

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

  FileImpl::System file_system_impl (first_poa.in ());

  PortableServer::ObjectId_var file_system_oid = PortableServer::string_to_ObjectId ("FileSystem");
  first_poa->activate_object_with_id (file_system_oid.in (), 
                                      &file_system_impl, 
                                      env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::activate_object_with_id");
      return -1;
    }
  
  CORBA::Object_var file_system = first_poa->id_to_reference (file_system_oid, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::id_to_reference");
      return -1;
    }
  
  CORBA::String_var file_system_ior = orb->object_to_string (file_system.in (), env);
  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }

  cout << file_system_ior.in () << endl;

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
