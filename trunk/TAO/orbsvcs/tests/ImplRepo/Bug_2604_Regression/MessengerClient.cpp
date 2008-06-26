// $Id$

#include "MessengerC.h"

#include "ace/Log_Msg.h"
#include "ace/SString.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var obj = orb->string_to_object ("file://messenger.ior");
      Messenger_var messenger = Messenger::_narrow (obj.in ());
      if (CORBA::is_nil (messenger.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Unable to get a Messenger "
                                                 "reference.\n")), -1);
        }

      CORBA::String_var message = CORBA::string_dup("Hello!");
      for (int i(0); i < 6; ++i)
        {
          try
            {
              messenger->send_message("TAO User", "TAO Test",
                                      message.inout ());
              ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Message was sent\n"
                                             "Client got reply: %s\n"),
                          message.in()));
            }
          catch (CORBA::Exception &ex)
            {
              ACE_CString str = ex._info ();
              ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Client send_message () "
                                                     "caught: %s\n"),
                                 ACE_TEXT_CHAR_TO_TCHAR (str.c_str ())), -1);
            }
        }
      return 0;
    }
  catch (CORBA::Exception &ex)
    {
      ACE_CString str = ex._info ();
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Client main() caught: %s\n"),
                         ACE_TEXT_CHAR_TO_TCHAR (str.c_str ())), -1);
    }
  return -1;
}
