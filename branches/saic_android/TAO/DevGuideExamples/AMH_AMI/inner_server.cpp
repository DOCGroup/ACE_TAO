// $Id$

#include "amh_ami_pch.h"

#include "inner_i.h"
#include "ace/SString.h"
#include "ace/OS_String.h"
#include <iostream>
#include <fstream>

int dont_crash = 1;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  int c = 0;
  while (c < argc)
    {
      if (ACE_OS::strcasecmp (argv[c], ACE_TEXT("-crash")) == 0)
        dont_crash = 0;
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

    parse_args (argc, argv);

    //Get reference to the RootPOA.
    CORBA::Object_var obj = orb->resolve_initial_references( "RootPOA" );
    PortableServer::POA_var poa = PortableServer::POA::_narrow( obj.in() );

    // Activate the POAManager.
    PortableServer::POAManager_var mgr = poa->the_POAManager();
    mgr->activate();

    // Create a servant.
    Inner_i servant(dont_crash);

    // Register the servant with the RootPOA, obtain its object
    // reference, stringify it, and write it to a file.
    PortableServer::ObjectId_var oid = poa->activate_object( &servant );
    obj = poa->id_to_reference( oid.in() );
    CORBA::String_var str = orb->object_to_string( obj.in() );
    ACE_CString iorname ("inner.ior");
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
