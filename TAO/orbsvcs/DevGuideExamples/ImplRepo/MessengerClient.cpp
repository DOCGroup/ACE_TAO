#include "MessengerC.h"
#include <iostream>
#include "ace/Get_Opt.h"

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior)
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
                           ACE_TEXT ("usage:  %s -k <ior>\n"),
                           argv[0]),
                          -1);
      }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    ACE_TCHAR const *ior = ACE_TEXT ("file://Messenger.ior");

    // Initialize orb
    CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

    if (parse_args (argc, argv, ior) != 0)
      {
        return 1;
      }

    // Destringify ior
    CORBA::Object_var const obj = orb->string_to_object (ior);
    if (CORBA::is_nil (obj.in ()))
      {
        std::cerr << "Nil Messenger reference" << std::endl;
        return 1;
      }

    // Narrow
    Messenger_var const messenger = Messenger::_narrow (obj.in ());
    if (CORBA::is_nil (messenger.in ()))
      {
        std::cerr << "Argument is not a Messenger reference" << std::endl;
        return 1;
      }

    CORBA::String_var message = CORBA::string_dup ("Hello!");
    messenger->send_message ("TAO User", "TAO Test", message.inout ());

    std::cout << "message was sent" << std::endl;
  }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "Client main() Caught CORBA::Exception: " << ex << std::endl;
      return 1;
    }

  return 0;
}
