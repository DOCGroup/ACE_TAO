// $Id$

#include "SenderC.h"
#include "ReceiverC.h"
#include "ace/Get_Opt.h"

static const char *sender_ior = "file://sender.ior";
static const char *receiver_ior = "file://receiver.ior";
static int iterations = 1;
static int corba_priority = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "s:r:n:p:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
        // sender ior
      case 's':
        sender_ior = get_opts.opt_arg ();
        break;

        // receiver ior
      case 'r':
        receiver_ior = get_opts.opt_arg ();
        break;

        // number of itarations
      case 'n':
        iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

        // corba priority
      case 'p':
        corba_priority = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s\n"
                           "\t-s <sender ior> [default is %s]\n"
                           "\t-r <receiver ior> [default is %s]\n"
                           "\t-n <number of invocations> [defaults to %d]\n"
                           "\t-p <corba priority> [defaults to %d]\n"
                           "\n",
                           argv [0],
                           sender_ior,
                           receiver_ior,
                           iterations,
                           corba_priority),
                          -1);
      }

  return 0;
}

int
main (int argc, char *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "");

      int result =
        parse_args (argc, argv);
      if (result != 0)
        return result;

      CORBA::Object_var object =
        orb->string_to_object (sender_ior);

      DiffServ::Sender_var sender =
        DiffServ::Sender::_narrow (object.in ());

      object =
        orb->string_to_object (receiver_ior);

      DiffServ::Receiver_var receiver =
        DiffServ::Receiver::_narrow (object.in ());

      sender->start (iterations,
                     corba_priority);

      sender->shutdown ();

      receiver->shutdown ();

      return 0;
    }
  catch (CORBA::Exception &exception)
    {
      ACE_ERROR ((LM_ERROR,
                  "Unexpected exception caught by client: %s (%s)\n",
                  exception._name (),
                  exception._rep_id ()));

      return -1;
    }
}
