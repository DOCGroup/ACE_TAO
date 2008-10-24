//$Id$:

#include "SenderC.h"
#include "ace/Get_Opt.h"

//IOR file of the Sender
const char * ior = 0;
const char * message = "starters message";


int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:m");
  int c = 0;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'k':
          ior = get_opts.opt_arg ();
          break;

        case 'm':
          message = get_opts.opt_arg ();
          break;

        case '?':  // display help for use of the server.
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                            "usage:  %s\n"
                            "-k <Sender IOR> (default is file://sender.ior)\n",
                            "-m <Message> (default is starters message)\n"
                            "\n",
                            argv [0]),
                            -1);
          break;
        }
    }

  if (ior  == 0)
    {
      ior = "file://sender.ior";
    }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv,
                                            "");

      if (parse_args (argc, argv) != 0)
        {
          return -1;
        }

      CORBA::Object_var obj =
        orb->string_to_object (ior);

      Hello::Sender_var sender = Hello::Sender::_narrow (obj.in ());

      if (CORBA::is_nil (sender.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to acquire Sender's objref\n"),
                            -1);
        }

      char *return_message;
      sender->local_message (message);
      return_message = sender->local_message ();
      ACE_DEBUG ((LM_DEBUG, "the message is %s\n", return_message));

      sender->start ();
      sender->remove ();

      ACE_DEBUG ((LM_DEBUG, "creating one more servant here\n"));
      sender->start ();
      //sender->remove ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Unknown exception \n");
      return -1;
    }

  return 0;
}
