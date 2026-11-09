#include "started_pch.h"
#include "MessengerC.h"
#include "ace/Get_Opt.h"

#include <iostream>
#include <string>

struct Client_Options
{
  std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://Messenger.ior")};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
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
  try
    {
      CORBA::ORB_var const orb (CORBA::ORB_init (argc, argv));
      Client_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const obj (orb->string_to_object (options.ior.c_str ()));
      if (CORBA::is_nil (obj.in ()))
        {
          std::cerr << "Could not get Messenger IOR." << std::endl;
          return 1;
        }

      Messenger_var const messenger (Messenger::_narrow (obj.in ()));
      if (CORBA::is_nil (messenger.in ()))
        {
          std::cerr << "IOR was not a Messenger object reference." << std::endl;
          return 1;
        }

      CORBA::String_var message (CORBA::string_dup ("Hello!"));
      messenger->send_message ("TAO User", "TAO Test", message.inout ());
      std::cout << "Reply: " << message.in () << std::endl;
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "CORBA exception: " << ex << std::endl;
      return 1;
    }

  return 0;
}
