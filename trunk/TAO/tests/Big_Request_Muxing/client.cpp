// $Id$

#include "Client_Task.h"
#include "ace/Get_Opt.h"
#include "tao/Messaging/Messaging.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

namespace
{
  // There are 3 clients so the server should expect:
  // 3 * NUM_MSGS * NUM_THRDS total messages.
  const int NUM_MSGS = 100;
  const int NUM_THRDS = 2;
  const int MSG_SIZE = 4096;
}

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
        "-k <ior>"
        "\n",
        argv [0]),
        -1);
  }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ACE_DEBUG ((LM_DEBUG, "(%P) Starting client\n"));

  try
  {
    CORBA::ORB_var orb =
      CORBA::ORB_init (argc, argv);

    if (parse_args (argc, argv) != 0)
      {
        return 1;
      }

    CORBA::Object_var tmp =
      orb->string_to_object(ior);

    Test::Payload_Receiver_var payload_receiver =
      Test::Payload_Receiver::_narrow(tmp.in ());

    if (CORBA::is_nil (payload_receiver.in ()))
    {
      ACE_ERROR_RETURN ((LM_DEBUG,
        "(%P) Nil coordinator reference <%s>\n",
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

    if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P) Client: Activating threads\n"));
    }
    if (task0.activate (THR_NEW_LWP | THR_JOINABLE, NUM_THRDS, 1) == -1)
    {
      ACE_ERROR ((LM_ERROR, "(%P) Client: Error activating %s task\n", task0.ID ()));
    }
    if (task1.activate (THR_NEW_LWP | THR_JOINABLE, NUM_THRDS, 1) == -1)
    {
      ACE_ERROR ((LM_ERROR, "(%P) Client: Error activating %s task\n", task1.ID ()));
    }
    if (task2.activate (THR_NEW_LWP | THR_JOINABLE, NUM_THRDS, 1) == -1)
    {
      ACE_ERROR ((LM_ERROR, "(%P) Client: Error activating %s task\n", task2.ID ()));
    }

    ACE_Time_Value end_time = ACE_OS::gettimeofday() + ACE_Time_Value(30);
    while (ACE_OS::gettimeofday() < end_time)
    {
      ACE_Time_Value tv (0, 100 * 1000);
      orb->run (tv);
      if (task0.done() && task1.done() && task2.done())
        break;
    }

    ACE_Thread_Manager::instance ()->wait ();
    if (TAO_debug_level > 0)
      {
        ACE_DEBUG ((LM_DEBUG, "(%P) Client: Threads finished posting\n"));
      }

    while (orb->work_pending())
    {
      ACE_Time_Value tv(0, 100 * 1000);
      orb->run(tv);
    }

    if (TAO_debug_level > 0)
      {
        ACE_DEBUG ((LM_DEBUG, "(%P) Client: work finished\n"));
      }

    // Allow time for the server to process messages at the other end
    // of the tcpip link before we destroy the sockets and rip down
    // any pending messages it may have buffered up. Note this is ONLY
    // for the benifit of the SYNC_NONE and SYNC_WITH_TRANSPORT messages.
    ACE_Time_Value tv (4, 0);
    orb->run (tv);
    orb->destroy ();
  }
  catch (const CORBA::Exception& ex)
  {
    ACE_DEBUG ((LM_DEBUG, "(%P) Client: "));
    ex._tao_print_exception ("CORBA Exception caught:");
    return 1;
  }
  catch (...)
  {
    ACE_DEBUG ((LM_DEBUG, "(%P) Client caught unknown exception\n"));
    return 1;
  }

  ACE_DEBUG ((LM_DEBUG, "(%P) Ending client (result 0)\n"));

  return 0;
}
