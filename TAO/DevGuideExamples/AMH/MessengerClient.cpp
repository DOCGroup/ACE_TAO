#include "amh_pch.h"
#include "MessengerC.h"

#include "ace/Get_Opt.h"

#include <iostream>

struct Client_Options
{
  ACE_TString ior {ACE_TEXT ("file://test.ior")};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int option;

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        std::cerr << "usage: " << argv[0] << " -k <ior>" << std::endl;
        return -1;
      }
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

      CORBA::Object_var const object (orb->string_to_object (options.ior.c_str ()));
      if (CORBA::is_nil (object.in ()))
        {
          std::cerr << "Could not get Messenger IOR." << std::endl;
          return 1;
        }

      DevGuide::Messenger_var const messenger (
        DevGuide::Messenger::_narrow (object.in ()));
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
