// $Id$

#include "Test_impl.h"
#include "ace/OS_NS_stdio.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
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

        Server_impl* server_obj = new Server_impl(orb.in());
        PortableServer::ServantBase_var owner (server_obj);
        server_var server = server_obj->_this();
        CORBA::String_var ior = orb->object_to_string (server.in ());

        FILE *output_file = ACE_OS::fopen ("server.ior", "w");
        ACE_OS::fprintf (output_file, "%s", ior.in ());
        ACE_OS::fclose (output_file);

        orb->run ();
        orb->destroy ();
    }
  catch(...)
    {
        return 1;
    }

    return 0;
}
