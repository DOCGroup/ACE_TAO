// $Id$

#include "common.h"
#include "tao/PortableServer/PortableServer.h"

CORBA::ORB_ptr
initialize_orb_and_poa(int & argc, ACE_TCHAR * argv[])
{
  CORBA::ORB_var orb =
    CORBA::ORB_init (argc, argv);

  CORBA::Object_var poa_object =
    orb->resolve_initial_references("RootPOA");

  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (poa_object.in ());

  PortableServer::POAManager_var poa_manager =
    root_poa->the_POAManager ();

  poa_manager->activate ();

  return orb._retn();
}

void report_exception()
{
  try
  {
    throw;
  }
  catch(CORBA::Exception & ex)
  {
    ex._tao_print_exception ("CORBA Exception raised:");
  }
  catch(char const * msg)
  {
    ACE_ERROR ((LM_ERROR, "Exception (char const*) raised: %s\n",
            msg));
  }
  catch(...)
  {
    ACE_ERROR ((LM_ERROR, "Unknown exception raised\n"));
  }
}
