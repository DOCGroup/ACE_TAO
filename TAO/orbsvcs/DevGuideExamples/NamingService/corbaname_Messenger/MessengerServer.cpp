// $Id$

#include "Messenger_i.h"
#include "orbsvcs/CosNamingC.h"
#include <iostream>
#include <fstream>
#include "ace/Get_Opt.h"

const ACE_TCHAR *output_file = ACE_TEXT ("MessengerServer.ready");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <outputfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    if (parse_args (argc, argv) != 0)
      return 1;

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
    PortableServer::Servant_var<Messenger_i> servant = new Messenger_i;
    PortableServer::ObjectId_var oid = poa->activate_object(servant.in());
    obj = poa->id_to_reference(oid.in());
    Messenger_var messenger = Messenger::_narrow(obj.in());
    root->rebind(name, messenger.in());

    std::cout << "Messenger object bound in Naming Service" << std::endl;

    // Write a file to let the run_test.pl script know we are ready.
    std::ofstream iorFile( ACE_TEXT_ALWAYS_CHAR(output_file) );
    iorFile << "Ready" << std::endl;
    iorFile.close();

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
