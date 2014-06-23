// $Id$

#include "MessengerC.h"
#include <iostream>
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

const ACE_TCHAR *ior = ACE_TEXT("file://Messenger.ior");
int seconds_between_requests = 4;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:d:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'd':
        seconds_between_requests = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-d <seconds> (Delay between requests) "
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

  // Detection of closed on read currently not working certain platforms.
#if defined (sun) || defined (AIX) || defined (__FreeBSD_version)
  return 2;
#endif

  try {
    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

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

    CORBA::String_var message = CORBA::string_dup( "Hello!" );

    messenger->send_message( "TAO User", "Test 1", message.inout() );

    // Force server to abort to verify it will be brought
    // back up when send_message() is called.
    messenger->abort(2);
    ACE_OS::sleep(seconds_between_requests);

    ACE_DEBUG ((LM_INFO,
                "(%P|%t) - Sending another message after abort of server\n"));

    messenger->send_message( "TAO User", "Test 2", message.inout() );

    std::cout << "messages were sent" << std::endl;
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "Client main() Caught CORBA::Exception: " << ex << std::endl;
    return 1;
  }

  return 0;
}
