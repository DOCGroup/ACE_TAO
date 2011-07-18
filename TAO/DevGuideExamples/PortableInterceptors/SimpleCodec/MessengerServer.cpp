// $Id$

#include "Messenger_i.h"
#include "MessengerC.h"
#include "ServerInitializer.h"
#include "tao/ORBInitializer_Registry.h"
// Ensure that the PI_Server library is linked in when building statically
#include "tao/PI_Server/PI_Server.h"
#include <iostream>
#include <fstream>

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      ServerInitializer *temp_initializer = 0;

      temp_initializer = new ServerInitializer;

      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

      //Get reference to Root POA
      CORBA::Object_var obj = orb->resolve_initial_references( "RootPOA" );
      PortableServer::POA_var poa = PortableServer::POA::_narrow( obj.in() );

      // Activate POA Manager
      PortableServer::POAManager_var mgr = poa->the_POAManager();
      mgr->activate();

      // Create an object
      Messenger_i messenger_servant;

      // Register the servant with the RootPOA, obtain its object
      // reference, stringify it, and write it to a file.
      PortableServer::ObjectId_var oid =
        poa->activate_object( &messenger_servant );
      CORBA::Object_var messenger_obj = poa->id_to_reference( oid.in() );
      CORBA::String_var str = orb->object_to_string( messenger_obj.in() );
      std::ofstream iorFile( "Messenger.ior" );
      iorFile << str.in() << std::endl;
      iorFile.close();
      std::cout << "IOR written to file Messenger.ior" << std::endl;

      // Accept requests
      orb->run();
      orb->destroy();
    }

  catch(const CORBA::Exception& ex)
    {
      std::cerr << "Exception in MessengerServer: " << ex << std::endl;
      return 1;
    }

  return 0;
}
