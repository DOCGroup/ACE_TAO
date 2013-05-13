//$Id$
/*
 * @file SenderDriver.cpp
 *
 * @author Marcel Smit
 */


#include "WL_ResetTopic_SenderC.h"
#include "ace/streams.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *sender_ior = ACE_TEXT ("file://Sender.ior");
const ACE_TCHAR *topic_name = ACE_TEXT ("");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:n:"));
  int c;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
          case 'k':
            sender_ior = get_opts.opt_arg ();
            break;

          case 'n':
            topic_name = get_opts.opt_arg ();
            break;

          case '?':
          default:
            ACE_ERROR_RETURN ((LM_ERROR,
                              "usage:  %s\n"
                              "-k <Sender IOR> (default is file://Sender.ior)\n"
                              "-n <new topic>\n"
                              "\n",
                              argv [0]),
                              -1);
        }
    }

  return 0;
}


int ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  try
    {
      // initialize the ORB
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);
      if (parse_args (argc, argv) != 0)
        {
          return -1;
        }

      // create the factory object reference,
      CORBA::Object_var sender_obj =
        orb->string_to_object (sender_ior);

      // downcast the object reference to the appropriate type
      ::WL_ResetTopic::Sender_var sender =
        ::WL_ResetTopic::Sender::_narrow (sender_obj.in ());

      if (CORBA::is_nil (sender.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Unable to acquire 'Sender' objref\n"),
                            -1);
        }
      sender->set_new_topic (topic_name);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Who is the culprit in the SenderDriver?\n");
      ACE_ERROR_RETURN ((LM_ERROR,
                          "Uncaught CORBA exception\n"),
                        1);
    }
  return 0;
}
