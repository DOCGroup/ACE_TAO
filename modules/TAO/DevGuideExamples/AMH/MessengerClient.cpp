// $Id$

#include "amh_pch.h"

#include "ace/Get_Opt.h"
#include "MessengerC.h"
#include <iostream>

ACE_TString ior = ACE_TEXT ("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
  {
      case 'k':
        ior = get_opts.optarg;
        break;

      case '?':
      default:
        std::cerr << "usage: " << argv[0] << " -k <ior>" << std::endl;
        return -1;
        break;
  }
  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    if (parse_args(argc, argv) != 0) {
      return 1;
    }

    // Read and destringify the Messenger object's IOR.
    CORBA::Object_var obj = orb->string_to_object(ior.c_str());
    if( CORBA::is_nil( obj.in() ) ) {
      std::cerr << "Could not get Messenger IOR." << std::endl;
      return 1;
    }

    // Narrow the IOR to a Messenger object reference.
    DevGuide::Messenger_var messenger =
      DevGuide::Messenger::_narrow( obj.in() );
    if( CORBA::is_nil( messenger.in() ) ) {
      std::cerr << "IOR was not a Messenger object reference." << std::endl;
      return 1;
    }

    // Send a message the the Messenger object.
    CORBA::String_var msg = CORBA::string_dup( "Hello!" );
    messenger->send_message( "TAO User", "TAO Test", msg.inout() );

    // Print the Messenger's reply.
    std::cout << "Reply: " << msg.in() << std::endl;

    return 0;
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "CORBA exception: " << ex << std::endl;
  }

  return 1;
}
