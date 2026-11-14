/* -*- C++ -*-  */

#include <iostream>

#include "MessengerC.h"
#include "ace/Get_Opt.h"

static ACE_TCHAR const *ior = ACE_TEXT ("file://Messenger.ior");

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
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
                           argv[0]),
                          -1);
      }
  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var const object = orb->string_to_object (ior);
      if (CORBA::is_nil (object.in ()))
        {
          std::cerr << "Nil Messenger reference" << std::endl;
          return 1;
        }

      Messenger_var const messenger = Messenger::_narrow (object.in ());
      if (CORBA::is_nil (messenger.in ()))
        {
          std::cerr << "Argument is not a Messenger reference" << std::endl;
          return 1;
        }

      CORBA::String_var message = CORBA::string_dup ("Implementing security policy now!");
      messenger->send_message ("Chief of Security", "New Directive", message.inout ());
      std::cout << "Message was sent" << std::endl;
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "Caught a CORBA exception: " << ex << std::endl;
      return 1;
    }

  return 0;
}
