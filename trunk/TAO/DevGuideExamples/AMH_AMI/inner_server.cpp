// $Id$

#include "amh_ami_pch.h"

#include "inner_i.h"
#include "ace/SString.h"
#include "ace/Get_Opt.h"
#include <iostream>
#include <fstream>

int dont_crash = 1;

const ACE_TCHAR *ior_output_file = ACE_TEXT ("inner.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("c:o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'c':
          dont_crash = 0;
        break;
      case 'o':
          ior_output_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile> "
                           "-c "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    if (parse_args (argc, argv) != 0)
            return 1;

    //Get reference to the RootPOA.
    CORBA::Object_var obj = orb->resolve_initial_references( "RootPOA" );
    PortableServer::POA_var poa = PortableServer::POA::_narrow( obj.in() );

    // Activate the POAManager.
    PortableServer::POAManager_var mgr = poa->the_POAManager();
    mgr->activate();

    // Create a servant.
    PortableServer::Servant_var<Inner_i> servant = new Inner_i(dont_crash);

    // Register the servant with the RootPOA, obtain its object
    // reference, stringify it, and write it to a file.
    PortableServer::ObjectId_var oid = poa->activate_object( servant.in() );
    obj = poa->id_to_reference( oid.in() );
    CORBA::String_var str = orb->object_to_string( obj.in() );
    ACE_CString iorname (ACE_TEXT_ALWAYS_CHAR(ior_output_file));
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
