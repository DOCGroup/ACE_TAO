// $Id$

#include "SenderC.h"
#include "ReceiverC.h"
#include "tao/ORB_Constants.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_string.h"


static const ACE_TCHAR *sender_ior = ACE_TEXT("file://sender.ior");
static const ACE_TCHAR *distributor_ior = ACE_TEXT("file://distributor.ior");
static const ACE_TCHAR *receiver_ior = ACE_TEXT("file://receiver.ior");
static int shutdown_sender = 0;
static int shutdown_distributor = 0;
static int shutdown_receiver = 0;
static CORBA::ULong iterations = 5;
static CORBA::ULong invocation_rate = 5;
static int count_missed_end_deadlines = 0;
static int do_dump_history = 0;
static int print_missed_invocations = 0;
static CORBA::ULong message_size = 100;
static const char *test_protocol = "IIOP";
static int print_statistics = 1;
static int number_of_connection_attempts = 20;
static int enable_diffserv_code_points = 0;
static int corba_priority = 0;
static const ACE_TCHAR *test_type = ACE_TEXT("PACED");

static int
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("a:b:c:d:e:i:m:p:r:s:t:u:v:w:x:y:z:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'a':
        test_type = get_opts.opt_arg ();
        break;

      case 'b':
        enable_diffserv_code_points = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'c':
        corba_priority = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'd':
        do_dump_history = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'e':
        count_missed_end_deadlines = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'i':
        iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'm':
        print_missed_invocations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'p':
        test_protocol = get_opts.opt_arg ();
        break;

      case 'r':
        invocation_rate = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 's':
        message_size = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 't':
        print_statistics = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'u':
        sender_ior = get_opts.opt_arg ();
        break;

      case 'v':
        distributor_ior = get_opts.opt_arg ();
        break;

      case 'w':
        receiver_ior = get_opts.opt_arg ();
        break;

      case 'x':
        shutdown_sender = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'y':
        shutdown_distributor = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'z':
        shutdown_receiver = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s\n"
                           "\t-a <test type> (defaults to %s [valid values are PACED, THROUGHPUT, and LATENCY)\n"
                           "\t-b <enable diffserv code points> (defaults to %d)\n"
                           "\t-c <corba priority> (defaults to %d)\n"
                           "\t-d <show history> (defaults to %d)\n"
                           "\t-e <count missed end deadlines> (defaults to %d)\n"
                           "\t-h <help: shows options menu>\n"
                           "\t-i <iterations> (defaults to %d)\n"
                           "\t-m <print missed invocations for paced workers> (defaults to %d)\n"
                           "\t-p <test protocol> (defaults to %s [valid values are IIOP, DIOP, and SCIOP])\n"
                           "\t-r <invocation rate> (defaults to %d)\n"
                           "\t-s <message size> (defaults to %d)\n"
                           "\t-t <print stats> (defaults to %d)\n"
                           "\t-u <sender ior> (defaults to %s)\n"
                           "\t-v <distributor ior> (defaults to %s)\n"
                           "\t-w <receiver ior> (defaults to %s)\n"
                           "\t-x <shutdown sender> (defaults to %d)\n"
                           "\t-y <shutdown distributor> (defaults to %d)\n"
                           "\t-z <shutdown receiver> (defaults to %d)\n"
                           "\n",
                           argv[0],
                           test_type,
                           enable_diffserv_code_points,
                           corba_priority,
                           do_dump_history,
                           count_missed_end_deadlines,
                           iterations,
                           print_missed_invocations,
                           test_protocol,
                           invocation_rate,
                           message_size,
                           print_statistics,
                           sender_ior,
                           distributor_ior,
                           receiver_ior,
                           shutdown_sender,
                           shutdown_distributor,
                           shutdown_receiver),
                          -1);
      }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      int result =
        parse_args (argc, argv);
      if (result != 0)
        return result;

      CORBA::Object_var object =
        orb->string_to_object (sender_ior);

      Protocols::Sender_var sender =
        Protocols::Sender::_narrow (object.in ());

      object =
        orb->string_to_object (receiver_ior);

      CORBA::ULong test_protocol_tag = IOP::TAG_INTERNET_IOP;
      if (ACE_OS::strcmp (test_protocol, "DIOP") == 0)
        test_protocol_tag = TAO_TAG_DIOP_PROFILE;
      else if (ACE_OS::strcmp (test_protocol, "SCIOP") == 0)
        test_protocol_tag = TAO_TAG_SCIOP_PROFILE;

      Protocols::Sender_Controller::Test_Type test_type_tag = Protocols::Sender_Controller::PACED;
      if (ACE_OS::strcmp (test_type, ACE_TEXT("THROUGHPUT")) == 0)
        test_type_tag = Protocols::Sender_Controller::THROUGHPUT;
      else if (ACE_OS::strcmp (test_type, ACE_TEXT("LATENCY")) == 0)
        test_type_tag = Protocols::Sender_Controller::LATENCY;

      sender->start (iterations,
                     invocation_rate,
                     count_missed_end_deadlines,
                     do_dump_history,
                     print_missed_invocations,
                     message_size,
                     test_protocol_tag,
                     print_statistics,
                     number_of_connection_attempts,
                     enable_diffserv_code_points,
                     corba_priority,
                     test_type_tag);

      if (shutdown_sender)
        sender->shutdown ();

      if (shutdown_distributor)
        {
          Protocols::Receiver_var distributor =
            Protocols::Receiver::_narrow (object.in ());

          distributor->shutdown ();
        }

      if (shutdown_receiver)
        {
          Protocols::Receiver_var receiver =
            Protocols::Receiver::_narrow (object.in ());

          receiver->shutdown ();
        }

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
