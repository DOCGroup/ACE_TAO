// $Id$

#include "MessengerC.h"

#include "ace/Log_Msg.h"
#include "ace/SString.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT("file://messenger.ior");

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

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var obj = orb->string_to_object (ior);
      Messenger_var messenger = Messenger::_narrow (obj.in ());
      if (CORBA::is_nil (messenger.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            ACE_TEXT ("Unable to get a Messenger reference.\n")),
                            -1);
        }

      CORBA::String_var message = CORBA::string_dup("Hello!");
      for (int i(0); i < 6; ++i)
        {
          try
            {
              messenger->send_message("TAO User", "TAO Test",
                                      message.inout ());
              ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Message was sent\n")
                                    ACE_TEXT("Client got reply: %C\n"),
                          message.in()));
            }
          catch (CORBA::Exception &ex)
            {
              ACE_CString str = ex._info ();
              ACE_ERROR_RETURN ((LM_ERROR,
                                ACE_TEXT ("Client send_message () caught: %C\n"),
                                 str.c_str ()), -1);
            }
        }
      return 0;
    }
  catch (CORBA::Exception &ex)
    {
      ACE_CString str = ex._info ();
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Client main() caught: %C\n"),
                         str.c_str ()), -1);
    }
  return -1;
}
