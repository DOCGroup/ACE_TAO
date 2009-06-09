// $Id$

// LoggerServer.cpp

#include "Logger_i.h"
#include <iostream>
#include <fstream>
int
ACE_TMAIN (int argc, ACE_TCHAR *argv [])
{
  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    // Get a reference to Root POA.
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj.in());

    // Activate the POA manager.
    PortableServer::POAManager_var mgr = poa->the_POAManager();
    mgr->activate();

    // Create a Logger_i servant.
    PortableServer::Servant_var<Logger_i> logger_servant = new Logger_i;

    // Register the servant with the RootPOA, obtain its object reference,
    // stringify it, and write it to a file.
    PortableServer::ObjectId_var oid = poa->activate_object(logger_servant.in());
    CORBA::Object_var logger_obj = poa->id_to_reference(oid.in());
    CORBA::String_var str = orb->object_to_string(logger_obj.in());
    std::ofstream iorFile("Logger.ior");
    iorFile << str.in() << std::endl;
    iorFile.close();
    std::cout << "IOR written to file Logger.ior" << std::endl;

    // Accept requests from clients.
    orb->run();

    // Release resources.
    orb->destroy();
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "Caught a CORBA exception:" << ex << std::endl;
    return 1;
  }
  return 0;
}
