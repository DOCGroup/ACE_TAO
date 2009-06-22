// $Id$

#include "simpleI.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {

      CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

      CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var poa = PortableServer::POA::_narrow(obj.in());

      PortableServer::POAManager_var man = poa->the_POAManager();

      man->activate();

      simple_i simp;

      PortableServer::ObjectId_var objId = poa->activate_object(&simp);

      orb->run();
    }

  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception in main():");
      return 1;
    }

  return 0;
}
