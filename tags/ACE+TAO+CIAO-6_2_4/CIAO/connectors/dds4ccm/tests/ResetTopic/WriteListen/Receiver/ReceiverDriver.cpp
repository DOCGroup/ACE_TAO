//$Id$
/*
 * @file ReceiverDriver.cpp
 *
 * @author Marcel Smit
 */


#include "WL_ResetTopic_ReceiverC.h"
#include "ace/streams.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *receiver_ior = ACE_TEXT ("file://Receiver.ior");
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
            receiver_ior = get_opts.opt_arg ();
            break;

          case 'n':
            topic_name = get_opts.opt_arg ();
            break;

          case '?':
          default:
            ACE_ERROR_RETURN ((LM_ERROR,
                              "usage:  %s\n"
                              "-k <Receiver IOR> (default is file://Receiver.ior)\n"
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
      CORBA::Object_var receiver_obj =
        orb->string_to_object (receiver_ior);

      // downcast the object reference to the appropriate type
      ::WL_ResetTopic::Receiver_var receiver =
        ::WL_ResetTopic::Receiver::_narrow (receiver_obj.in ());

      if (CORBA::is_nil (receiver.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Unable to acquire 'Receiver' objref\n"),
                            -1);
        }
      receiver->set_new_topic (topic_name);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Who is the culprit in the ReceiverDriver?\n");
      ACE_ERROR_RETURN ((LM_ERROR,
                          "Uncaught CORBA exception\n"),
                        1);
    }
  return 0;
}
