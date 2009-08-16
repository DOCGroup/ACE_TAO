// $Id$

#include "Messenger_i.h"
#include "MessengerS.h"
#include "ServerInitializer.h"

#include "tao/ORBInitializer_Registry.h"
// Ensure that the PI_Server library is linked in when building statically
#include "tao/PI_Server/PI_Server.h"
#include <iostream>
#include <fstream>
#include <fstream>

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      PortableInterceptor::ORBInitializer_var orb_initializer =
        new ServerInitializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "Server ORB");

      //Get reference to Root POA
      CORBA::Object_var obj = orb->resolve_initial_references( "RootPOA" );
      PortableServer::POA_var poa = PortableServer::POA::_narrow( obj.in() );

      // Activate POA Manager
      PortableServer::POAManager_var mgr = poa->the_POAManager();
      mgr->activate();

      // Create an object
      PortableServer::Servant_var<Messenger_i> messenger_servant = new Messenger_i;

      PortableServer::ObjectId_var oid =
        poa->activate_object( messenger_servant.in() );
      CORBA::Object_var messenger_obj = poa->id_to_reference( oid.in() );

      CORBA::String_var str = orb->object_to_string( messenger_obj.in() );
      std::ofstream iorFile ( "Messenger.ior" );
      iorFile << str.in() << std::endl;
      iorFile.close();
      std::cout << std::endl << "IOR written to file Messenger.ior" << std::endl;

      // Accept requests
      orb->run();
      orb->destroy();
    }

  catch(const CORBA::Exception& ex)
    {
      std::cerr << "server Caught CORBA exception: " << ex << std::endl;
      return 1;
    }

  return 0;
}
