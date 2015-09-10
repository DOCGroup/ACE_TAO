#include "MessengerC.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT("file://server.ior");

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

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
  {
    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv );

    if (parse_args (argc, argv) != 0)
      return 1;

    CORBA::Object_var obj =
      orb->string_to_object (ior);

    // Narrow the Messenger object reference
    Messenger_var messenger = Messenger::_narrow(obj.in());
    if (CORBA::is_nil(messenger.in())) {
      ACE_ERROR((LM_ERROR, " ERROR: Client cannot get a Messenger reference\n"));
      return 1;
    }

    CORBA::String_var message = CORBA::string_dup("Hello!");

    ACE_DEBUG((LM_DEBUG, "CLIENT: Start sending message\n"));

    // Send a message
    messenger->send_message("user", "TAO Test", message.inout());

    ACE_DEBUG((LM_DEBUG, "CLIENT: Message was sent\n"));

    messenger->shutdown ();

    orb->destroy();
  }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
