// $Id$

#include "amh_ami_pch.h"

#include "middle_i.h"
#include "ace/OS_String.h"
#include <iostream>
#include <fstream>

int use_synch = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  int c = 0;
  while (c < argc)
    {
      if (ACE_OS::strcasecmp (argv[c], ACE_TEXT("-no_AMH")) == 0)
        use_synch = 1;
      c++;
    }

  return 1;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    parse_args(argc,argv);

    //Get reference to the RootPOA.
    CORBA::Object_var obj = orb->resolve_initial_references( "RootPOA" );
    PortableServer::POA_var poa = PortableServer::POA::_narrow( obj.in() );

    // Activate the POAManager.
    PortableServer::POAManager_var mgr = poa->the_POAManager();
    mgr->activate();

    obj = orb->string_to_object("file://inner.ior");
    Inner_var peer =
      Inner::_narrow(obj.in());
    if (CORBA::is_nil(peer.in()))
      {
        std::cerr << "Could not initialize peer object reference" << std::endl;
        ACE_OS::exit (1);
      }

    // create either a synchronous or AMH_based servant depending on command
    // line arguement.
    PortableServer::ServantBase_var servant;
    if (use_synch)
      servant = new Middle_i (peer.in());
    else
      servant = new Asynch_Middle_i(poa.in(), peer.in());

    // Register the servant with the RootPOA, obtain its object
    // reference, stringify it, and write it to a file.
    PortableServer::ObjectId_var oid = poa->activate_object(servant.in());
    obj = poa->id_to_reference( oid.in() );
    CORBA::String_var str = orb->object_to_string( obj.in() );

    ACE_CString iorname("middle.ior");
    std::ofstream iorFile (iorname.c_str());
    iorFile << str.in() << std::endl;
    iorFile.close();
    std::cout << "IOR written to " << iorname << std::endl;

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
