/* -*- C++ -*- $Id$ */

#include "MessengerC.h"
#include <iostream>
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://Messenger.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
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
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    if (parse_args (argc, argv) != 0)
      return 1;

    // Destringify ior
    CORBA::Object_var obj = orb->string_to_object( ior );
    if (CORBA::is_nil(obj.in())) {
      std::cerr << "Nil Messenger reference" << std::endl;
      return 1;
    }

    // Narrow
    Messenger_var messenger = Messenger::_narrow( obj.in() );
    if (CORBA::is_nil(messenger.in())) {
      std::cerr << "Argument is not a Messenger reference" << std::endl;
      return 1;
    }

    CORBA::String_var message = CORBA::string_dup(
      "Implementing security policy now!" );
    messenger->send_message( "Chief of Security",
                             "New Directive",
                             message.inout() );
    std::cout << "message was sent" << std::endl;

    orb->destroy ();
  }

  catch(const CORBA::Exception& ex) {
    std::cerr << "Caught CORBA exception: " << ex << std::endl;
    return 1;
  }

  return 0;
}
