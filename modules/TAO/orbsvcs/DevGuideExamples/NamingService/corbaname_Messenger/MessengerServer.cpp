// $Id$

#include "Messenger_i.h"
#include "orbsvcs/CosNamingC.h"
#include <iostream>
int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    //Get reference to Root POA
    CORBA::Object_var obj = orb->resolve_initial_references( "RootPOA" );
    PortableServer::POA_var poa = PortableServer::POA::_narrow( obj.in() );

    // Activate POA Manager
    PortableServer::POAManager_var mgr = poa->the_POAManager();
    mgr->activate();

    // Find the Naming Service
    obj = orb->resolve_initial_references("NameService");
    CosNaming::NamingContext_var root =
      CosNaming::NamingContext::_narrow(obj.in());
    if (CORBA::is_nil(root.in())) {
      std::cerr << "Nil Naming Context reference" << std::endl;
      return 1;
    }

    // Bind the example Naming Context, if necessary
    CosNaming::Name name;
    name.length( 1 );
    name[0].id = CORBA::string_dup("example");
    try {
      CORBA::Object_var dummy = root->resolve(name);
    }
    catch(const CosNaming::NamingContext::NotFound&) {
      CosNaming::NamingContext_var dummy = root->bind_new_context( name );
    }

    // Bind the Messenger object
    name.length( 2 );
    name[1].id = CORBA::string_dup("Messenger");

    // Create an object
    Messenger_i servant;
    PortableServer::ObjectId_var oid = poa->activate_object(&servant);
    obj = poa->id_to_reference(oid.in());
    Messenger_var messenger = Messenger::_narrow(obj.in());
    root->rebind(name, messenger.in());

    std::cout << "Messenger object bound in Naming Service" << std::endl;

    // Accept requests
    orb->run();
    orb->destroy();
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "server:Caught a CORBA::Exception: " << ex << std::endl;
    return 1;
  }

  return 0;
}
