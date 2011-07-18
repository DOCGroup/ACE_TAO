// $Id$

#include "orbsvcs/CosNamingC.h"
#include "Messenger_i.h"
#include <iostream>
#include <fstream>
#include <fstream>

int
ACE_TMAIN (int argc, ACE_TCHAR *argv [])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

      CORBA::Object_var rootObj = orb->resolve_initial_references("NameService");
      CosNaming::NamingContext_var rootNC =
      CosNaming::NamingContext::_narrow(rootObj.in());

      CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
      PortableServer::POA_var poa = PortableServer::POA::_narrow(obj.in());
      PortableServer::POAManager_var mgr = poa->the_POAManager();
      mgr->activate();

      Messenger_i messenger_servant (orb.in ());
      PortableServer::ObjectId_var oid = poa->activate_object (&messenger_servant);
      CORBA::Object_var messenger_obj = poa->id_to_reference (oid.in ());
      CORBA::String_var str = orb->object_to_string (messenger_obj.in());

      std::ofstream iorFile ("Messenger.ior");
      iorFile << str.in () << std::endl;
      iorFile.close ();
      std::cout << "IOR written to file Messenger.ior " << std::endl;

      orb->run ();
      orb->destroy ();
    }

  catch(const CORBA::Exception& ex)
  {
      std::cerr << "Caught exception: " << ex << std::endl;
      return 1;
  }

  return 0;
}
