#include "MessengerC.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Get_Opt.h"

#include <iostream>

int
parse_args (int const argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior)
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
                           ACE_TEXT ("usage: %s [-k <ior>]\n"),
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
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      ACE_TCHAR const *ior = ACE_TEXT ("file://Messenger.ior");
      if (parse_args (argc, argv, ior) != 0)
        return 1;

      CORBA::Object_var const obj = orb->string_to_object (ior);
      if (CORBA::is_nil (obj.in ()))
        {
          std::cerr << "Nil Messenger reference" << std::endl;
          return 1;
        }

      Messenger_var const messenger = Messenger::_narrow (obj.in ());
      if (CORBA::is_nil (messenger.in ()))
        {
          std::cerr << "Argument is not a Messenger reference" << std::endl;
          return 1;
        }

      CORBA::String_var message = CORBA::string_dup ("Where can I get TAO?");
      for (int i {}; i < 3; ++i)
        messenger->send_message ("person@company.com",
                                 "OCI's Distribution of TAO",
                                 message.inout ());
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "Caught a CORBA exception: " << ex << std::endl;
      return 1;
    }

  return 0;
}
