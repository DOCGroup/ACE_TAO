//$Id$:

#include "SenderC.h"
#include "ace/Get_Opt.h"

// IOR file of the Sender
const ACE_TCHAR * ior = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c = 0;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'k':
          ior = get_opts.opt_arg ();
          break;

        case '?':  // display help for use of the server.
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Usage:  %s\n"
                            "-k <Sender IOR> (default is file://Sender.ior)\n",
                            argv [0]),
                            -1);
          break;
        }
    }

  if (ior  == 0)
    {
      ior = ACE_TEXT("file://Sender.ior");
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        {
          return -1;
        }

      CORBA::Object_var obj = orb->string_to_object (ior);

      Thread::Sender_var sender = Thread::Sender::_narrow (obj.in ());

      if (CORBA::is_nil (sender.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to acquire Sender's objref\n"),
                            -1);
        }

      sender->start ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Unknown exception\n");
      return -1;
    }

  return 0;
}
