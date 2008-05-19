// $Id$

#include "Test_impl.h"
#include "ace/OS_NS_stdio.h"

const char *ior_callback = "callback.ior";
const char *ior_server = "file://server.ior";

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int result = 0;
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
      orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
      PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
      root_poa->the_POAManager ();
      poa_manager->activate ();

      const char* uipmc_ior = "corbaloc:miop:1.0@1.0-domain-1/127.0.0.1:23232";
      CORBA::Object_var obj =orb->string_to_object (uipmc_ior);

      CORBA::Object_var tmp = orb->string_to_object (ior_server);
      server_var server = server::_narrow (tmp.in ());

      try
        {
          server->method (obj.in());

          ACE_DEBUG ((LM_DEBUG, "Test passed !!\n"));
        }
      catch (const CORBA::SystemException& marshal)
        {
          ACE_DEBUG ((LM_ERROR, "Test Failed - Regression. "
                      "Sending UIPMC object ref to server failed with:\n"));
          marshal._tao_print_exception ("Exception : \n");
          result = 1;
        }

      server->shutdown ();
    }
  catch(const CORBA::SystemException& e)
    {
        e._tao_print_exception ("Unexpected exception - not a regression.\n");
        result = 1;
    }
    return result;
}
