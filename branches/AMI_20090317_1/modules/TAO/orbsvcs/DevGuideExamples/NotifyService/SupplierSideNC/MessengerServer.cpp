// $Id$

#include "orbsvcs/CosNamingC.h"
#include "Messenger_i.h"
#include <iostream>
#include <fstream>
int
ACE_TMAIN (int argc, ACE_TCHAR *argv [])
{
  try
  {
    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    // Find the Naming Service.
    CORBA::Object_var rootObj =  orb->resolve_initial_references("NameService");
    CosNaming::NamingContext_var rootNC =
      CosNaming::NamingContext::_narrow(rootObj.in());

    // Get the  Root POA.
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj.in());

    // Activate POA manager
    PortableServer::POAManager_var mgr = poa->the_POAManager();
    mgr->activate();

    // Create our Messenger servant.
    Messenger_i messenger_servant(orb.in());

    // Register it with the RootPOA.
    PortableServer::ObjectId_var oid =
      poa->activate_object( &messenger_servant );
    CORBA::Object_var messenger_obj = poa->id_to_reference( oid.in() );

    // Bind it in the Naming Service.
    CosNaming::Name name;
    name.length (1);
    name[0].id = CORBA::string_dup("MessengerService");
    rootNC->rebind(name, messenger_obj.in());

    CORBA::String_var str = orb->object_to_string (messenger_obj.in());
    std::ofstream iorFile ("Messenger.ior");
    iorFile << str.in () << std::endl;
    iorFile.close ();
    std::cout << "IOR written to file Messenger.ior " << std::endl;

    // Accept requests
    orb->run();
    orb->destroy();

  }
  catch(const CORBA::Exception& ex) {
    std::cerr << ex << std::endl;
    return 1;
  }
  return 0;

}


