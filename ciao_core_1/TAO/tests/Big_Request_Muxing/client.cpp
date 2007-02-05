// $Id$

#include "Client_Task.h"
#include "ace/Get_Opt.h"
#include "tao/Messaging/Messaging.h"

ACE_RCSID(Muxing, client, "$Id$")

const char *ior = "file://test.ior";

// 3 clients with 2 threads each send this many messages.
// so the server should expect NUM_MSGS * 6 total.
static const int NUM_MSGS = 100;
static const int NUM_THRDS = 2;
static const int MSG_SIZE = 4096;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
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
        "-k <ior>"
        "\n",
        argv [0]),
        -1);
  }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_DEBUG ((LM_DEBUG, "(%P) Starting client\n"));

  try
  {
    CORBA::ORB_var orb =
      CORBA::ORB_init (argc, argv);

    if (parse_args (argc, argv) != 0)
      return 1;

    CORBA::Object_var tmp =
      orb->string_to_object(ior);

    Test::Payload_Receiver_var payload_receiver =
      Test::Payload_Receiver::_narrow(tmp.in ());

    if (CORBA::is_nil (payload_receiver.in ()))
    {
      ACE_ERROR_RETURN ((LM_DEBUG,
        "Nil coordinator reference <%s>\n",
        ior),
        1);
    }

    Client_Task task0 (ACE_Thread_Manager::instance (),
      payload_receiver.in (),
      NUM_MSGS,
      MSG_SIZE,
      orb.in (),
      Messaging::SYNC_WITH_TARGET,
      ACE_CString("Sync_With_Target"));
    Client_Task task1 (ACE_Thread_Manager::instance (),
      payload_receiver.in (),
      NUM_MSGS,
      MSG_SIZE,
      orb.in (),
      Messaging::SYNC_WITH_TRANSPORT,
      ACE_CString("Sync_With_Transport"));
    Client_Task task2 (ACE_Thread_Manager::instance (),
      payload_receiver.in (),
      NUM_MSGS,
      MSG_SIZE,
      orb.in (),
      Messaging::SYNC_NONE,
      ACE_CString("Sync_None"));

    ACE_DEBUG ((LM_DEBUG, "(%P) Activating threads in client\n"));
    if (task0.activate (THR_NEW_LWP | THR_JOINABLE, NUM_THRDS, 1) == -1)
    {
      ACE_ERROR ((LM_ERROR, "Error activating client task\n"));
    }
    if (task1.activate (THR_NEW_LWP | THR_JOINABLE, NUM_THRDS, 1) == -1)
    {
      ACE_ERROR ((LM_ERROR, "Error activating client task\n"));
    }
    if (task2.activate (THR_NEW_LWP | THR_JOINABLE, NUM_THRDS, 1) == -1)
    {
      ACE_ERROR ((LM_ERROR, "Error activating client task\n"));
    }

    ACE_Time_Value end_time = ACE_OS::gettimeofday() + ACE_Time_Value(10);
    while (ACE_OS::gettimeofday() < end_time)
    {
      ACE_Time_Value tv (0, 100 * 1000);
      orb->run (tv);
      if (task0.done() && task1.done() && task2.done())
        break;
    }

    ACE_Thread_Manager::instance ()->wait ();
    ACE_DEBUG ((LM_DEBUG, "(%P) Threads finished\n"));

    while (orb->work_pending())
    {
      ACE_Time_Value tv(0, 100 * 1000);
      orb->run(tv);
    }

    orb->destroy ();
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("Exception caught:");
    return 1;
  }

  ACE_DEBUG ((LM_DEBUG, "(%P) Ending client\n"));

  return 0;
}
