//$Id$
#include "testS.h"

class foo_impl :public POA_foo
{
public:
  void bar (CORBA::VoidData something)
  {
    ACE_DEBUG ((LM_DEBUG,
		"%d\n",
		(int*)something))
  }
}

main (int argc, char* argv[])
{
  
  CORBA::ORB_var orb =
    CORBA::ORB_init (argc,
                     argv,
                     ""
                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CORBA::Object_var object =
    orb->resolve_initial_references ("RootPOA"
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (object.in ()
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  
  PortableServer::POAManager_var poa_manager =
    root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  
  foo_impl* foo_i;
  ACE_NEW_RETURN (foo_i,
		  foo_impl,
		  -1);

  PortableServer::ObjectId_var id;
  
  id = root_poa->activate_object (foo_i
				  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  CORBA::Object_var server =
    root_poa->id_to_reference (id.in ()
			       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  CORBA::String_var ior =
    orb_->object_to_string (server.in ()
			    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  ACE_DEBUG ((LM_DEBUG,
	      "IOR = %s\n",
	      ior.c_str ()));
  orb->run ();
  
}
