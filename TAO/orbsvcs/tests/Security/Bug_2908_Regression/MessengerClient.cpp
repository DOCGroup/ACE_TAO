#include "MessengerC.h"
#include "ace/Get_Opt.h"

struct Client_Options
{
  ACE_TString ior {ACE_TEXT ("file://server.ior")};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s -k <ior>\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
    Client_Options options;

    if (parse_args (argc, argv, options) != 0)
      return 1;

    CORBA::Object_var obj = orb->string_to_object (options.ior.c_str ());

    // Narrow the Messenger object reference
    Messenger_var messenger = Messenger::_narrow (obj.in ());
    if (CORBA::is_nil (messenger.in ()))
    {
      ACE_ERROR ((LM_ERROR, " ERROR: Client cannot get a Messenger reference\n"));
      return 1;
    }

    CORBA::String_var message = CORBA::string_dup ("Hello!");

    ACE_DEBUG ((LM_DEBUG, "CLIENT: Start sending message\n"));

    // Send a message
    messenger->send_message ("user", "TAO Test", message.inout ());

    ACE_DEBUG ((LM_DEBUG, "CLIENT: Message was sent\n"));

    messenger->shutdown ();

    orb->destroy ();
  }
  catch (CORBA::Exception const &ex)
  {
    ex._tao_print_exception ("Caught exception:");
    return 1;
  }

  return 0;
}
