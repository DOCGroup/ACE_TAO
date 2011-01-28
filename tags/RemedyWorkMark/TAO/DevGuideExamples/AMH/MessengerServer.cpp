// $Id$

#include "amh_pch.h"

#include "ace/Get_Opt.h"
#include "AMH_Messenger_i.h"
#include <iostream>
#include <fstream>

ACE_TString ior_output_file;

int
parse_args(int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts(argc, argv, ACE_TEXT("o:"));
  int c;

  while((c = get_opts()) != -1)
    switch(c)
  {
      case 'o':
        ior_output_file = get_opts.optarg;
        break;
        break;
      case '?':
      default:
        std::cerr << "usage: " << argv[0] << "-o <iorfile>" << std::endl;
        return -1;
        break;
  }
  // Indicates successful parsing of the command line
  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    if (parse_args(argc, argv) != 0) {
      return 1;
    }

    //Get reference to the RootPOA.
    CORBA::Object_var obj = orb->resolve_initial_references( "RootPOA" );
    PortableServer::POA_var poa = PortableServer::POA::_narrow( obj.in() );

    // Activate the POAManager.
    PortableServer::POAManager_var mgr = poa->the_POAManager();
    mgr->activate();

    // Create a servant.
    PortableServer::Servant_var<AMH_Messenger_i> servant = new AMH_Messenger_i;

    // Register the servant with the RootPOA, obtain its object
    // reference, stringify it, and write it to a file.
    PortableServer::ObjectId_var oid = poa->activate_object( servant.in() );
    obj = poa->id_to_reference( oid.in() );
    CORBA::String_var str = orb->object_to_string( obj.in() );
    std::ofstream iorFile(ACE_TEXT_ALWAYS_CHAR(ior_output_file.c_str()));
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
