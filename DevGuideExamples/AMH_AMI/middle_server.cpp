// $Id$

#include "amh_ami_pch.h"

#include "middle_i.h"
#include "ace/Get_Opt.h"
#include <iostream>
#include <fstream>

int use_synch = 0;

const ACE_TCHAR *ior_output_file = ACE_TEXT ("middle.ior");
const ACE_TCHAR *ior_input_file = ACE_TEXT ("file://inner.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:i:n"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
          ior_output_file = get_opts.opt_arg ();
          break;
      case 'i':
          ior_input_file = get_opts.opt_arg ();
          break;
      case 'n':
          use_synch = 1;
          break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <ior_output_file> "
                           "-i <ior_input_file> "
                           "-n"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
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

    obj = orb->string_to_object(ior_input_file);
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

    ACE_CString iorname(ACE_TEXT_ALWAYS_CHAR(ior_output_file));
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
