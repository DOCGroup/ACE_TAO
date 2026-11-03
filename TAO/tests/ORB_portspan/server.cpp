#include "simpleI.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var const obj = orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const poa = PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var const man = poa->the_POAManager ();

      man->activate ();

      // Keep the stack servant alive while the ORB is running.
      simple_i simp;

      PortableServer::ObjectId_var const obj_id = poa->activate_object (&simp);

      orb->run ();
    }
  catch (CORBA::Exception const&)
    {
      ACE_DEBUG ((LM_DEBUG, "server failed to start\n"));
      return 1;
    }

  return 0;
}
