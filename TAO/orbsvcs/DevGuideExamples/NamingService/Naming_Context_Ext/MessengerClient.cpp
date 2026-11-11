#include "MessengerC.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Get_Opt.h"
#include <iostream>

struct Options
{
  ACE_TCHAR const *ior_input_file {ACE_TEXT ("file://url.ior")};
};

int parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior_input_file = get_opts.opt_arg ();
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -k <ior>\n"),
                           argv[0]),
                          -1);
      }

  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      Options options {};
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var object = orb->string_to_object (options.ior_input_file);
      Messenger_var messenger = Messenger::_narrow (object.in ());
      if (CORBA::is_nil (messenger.in ()))
        {
          std::cerr << "Not a Messenger reference" << std::endl;
          return 1;
        }

      CORBA::String_var message = CORBA::string_dup ("Hello!");
      messenger->send_message ("TAO User", "TAO Test", message.inout ());
      std::cout << "Message was sent" << std::endl;
      std::cout << "Now try the same thing with the simple name." << std::endl;

      object = orb->resolve_initial_references ("NameService");
      CosNaming::NamingContextExt_var root =
        CosNaming::NamingContextExt::_narrow (object.in ());
      object = root->resolve_str ("Simple/Messenger");
      messenger = Messenger::_narrow (object.in ());
      messenger->send_message ("ACE User", "TAO Test", message.inout ());
      std::cout << "Message was sent" << std::endl;
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "Caught a CORBA::Exception: " << ex << std::endl;
      return 1;
    }

  return 0;
}
