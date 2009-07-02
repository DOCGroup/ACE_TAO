// $Id$

#include "started_pch.h"

#include "Messenger_i.h"
#include <iostream>
#include <fstream>

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    //Get reference to the RootPOA.
    CORBA::Object_var obj = orb->resolve_initial_references( "RootPOA" );
    PortableServer::POA_var poa = PortableServer::POA::_narrow( obj.in() );

    // Activate the POAManager.
    PortableServer::POAManager_var mgr = poa->the_POAManager();
    mgr->activate();

    // Create a servant.
    PortableServer::Servant_var<Messenger_i> servant = new Messenger_i();

    // Register the servant with the RootPOA, obtain its object
    // reference, stringify it, and write it to a file.
    PortableServer::ObjectId_var oid = poa->activate_object( servant.in() );
    obj = poa->id_to_reference( oid.in() );
    CORBA::String_var str = orb->object_to_string( obj.in() );
    std::ofstream iorFile( "Messenger.ior" );
    iorFile << str.in() << std::endl;
    iorFile.close();
    std::cout << "IOR written to file Messenger.ior" << std::endl;

    // Accept requests from clients.
    orb->run();
    orb->destroy();

    return 0;
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "CORBA exception: " << ex << std::endl;
  }

  return 1;
}
