// $Id$

#include "MessengerC.h"
#include <iostream>
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

const ACE_TCHAR *ior = ACE_TEXT("file://Messenger.ior");
int seconds_between_requests = 4;
// Number of times we try to invoke the operation
int number_of_tries = 2;
// Number of times the invocation should succeed
int number_of_succeed = 2;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:d:t:s:"));
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

      case 't':
        number_of_tries = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 's':
        number_of_succeed = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-d <seconds> (Delay between requests) "
                           "-t <number> (Number of times we try the invocation) "
                           "-s <number> (Number of times the invocation should succeed) "
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

    int try_count = 0;
    int succeeded = 0;
    for (; try_count < number_of_tries; ++try_count)
    {
      ACE_DEBUG ((LM_INFO,
                  "(%P|%t) - Sending message <%d> to server\n", try_count));

      try {
        messenger->send_message( "TAO User", "Test 1", message.inout() );

        ACE_DEBUG ((LM_INFO,
            "(%P|%t) - Successfully received response for message <%d> to server\n", try_count));
        ++succeeded;

        // Force server to abort to verify it will be brought
        // back up when send_message() is called.
        messenger->abort(2);
        ACE_OS::sleep(seconds_between_requests);
      }
      catch (const CORBA::Exception&)
      {
        // Swallow
      }
    }

    if (succeeded == number_of_succeed)
    {
      ACE_DEBUG ((LM_INFO,
            "(%P|%t) - <%d> Messages where send to the server, <%d> succeeded\n", try_count, succeeded));
    }
    else
    {
      ACE_ERROR ((LM_INFO,
            "(%P|%t) - ERROR: <%d> Messages where send to the server, <%d> succeeded, should be <%d>\n", try_count, succeeded, number_of_succeed));
    }

    orb->destroy ();
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "Client main() Caught CORBA::Exception: " << ex << std::endl;
    return 1;
  }

  return 0;
}
