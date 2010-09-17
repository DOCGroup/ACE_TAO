/* -*- C++ -*- $Id$ */

#include "Messenger_i.h"
#include <iostream>
#include <fstream>
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("Messenger.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
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

    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    if (parse_args (argc, argv) != 0)
      return 1;

    CORBA::Object_var obj =
      orb->resolve_initial_references( "RootPOA" );

    PortableServer::POA_var poa =
      PortableServer::POA::_narrow( obj.in() );

    PortableServer::POAManager_var mgr =
      poa->the_POAManager();
    mgr->activate();

    obj =
      orb->resolve_initial_references ("SSLIOPCurrent");

    SSLIOP::Current_var ssliop_current =
      SSLIOP::Current::_narrow (obj.in ());

    PortableServer::Servant_var<Messenger_i> messenger_servant
      = new Messenger_i(orb.in(), ssliop_current.in());

    PortableServer::ObjectId_var oid =
      poa->activate_object( messenger_servant.in() );

    CORBA::Object_var messenger_obj = poa->id_to_reference( oid.in() );

    CORBA::String_var str = orb->object_to_string( messenger_obj.in() );

    std::ofstream iorFile( ACE_TEXT_ALWAYS_CHAR(ior_output_file) );

    iorFile << str.in() << std::endl;

    iorFile.close();

    std::cout << "IOR written to file " << ACE_TEXT_ALWAYS_CHAR(ior_output_file) << std::endl;

    orb->run();
    poa->destroy (1, 1);
    orb->destroy ();

    std::cout << "Messenger Server is shut down!"
         << std::endl;
    std::cout << std::endl;

  }
  catch(const CORBA::Exception& ex) {
    ex._tao_print_exception("Server Error: main block");
    return 1;
  }

  return 0;
}
