// $Id$

#include "Client_Task.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Muxing, client, "$Id$")

const char *ior = "file://test.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
	ior = get_opts.optarg;
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
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Test::Payload_Receiver_var payload_receiver =
        Test::Payload_Receiver::_narrow(tmp.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (payload_receiver.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil coordinator reference <%s>\n",
                             ior),
                            1);
        }


      Client_Task task0 (ACE_Thread_Manager::instance (),
                         payload_receiver.in (),
                         1000,
                         32768,
                         orb.in (),
                         Messaging::SYNC_WITH_TARGET);
      Client_Task task1 (ACE_Thread_Manager::instance (),
                         payload_receiver.in (),
                         1000,
                         32768,
                         orb.in (),
                         Messaging::SYNC_WITH_TRANSPORT);
      Client_Task task2 (ACE_Thread_Manager::instance (),
                         payload_receiver.in (),
                         1000,
                         32768,
                         orb.in (),
                         Messaging::SYNC_NONE);

      if (task0.activate (THR_NEW_LWP | THR_JOINABLE, 2, 1) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating client task\n"));
        }
      if (task1.activate (THR_NEW_LWP | THR_JOINABLE, 2, 1) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating client task\n"));
        }
      if (task2.activate (THR_NEW_LWP | THR_JOINABLE, 2, 1) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating client task\n"));
        }

      ACE_Time_Value tv (240, 0);
      orb->run (tv, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_Thread_Manager::instance ()->wait ();

      CORBA::Long count =
        payload_receiver->get_message_count (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "(%P) - Payload_Receiver got %d messages\n",
                  count));

      orb->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
