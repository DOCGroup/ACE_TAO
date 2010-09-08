// $Id$

#include "Client_Task.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
bool shutdown_srv = false;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:x"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'x':
        shutdown_srv = true;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "[-x]"
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
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      Test::Receiver_var receiver =
        Test::Receiver::_narrow(tmp.in ());

      if (CORBA::is_nil (receiver.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil coordinator reference <%s>\n",
                             ior),
                            1);
        }

      if (shutdown_srv)
        {
          receiver->shutdown ();
        }
      else
        {

          Client_Task client_task (receiver.in (),
#if defined (ACE_OPENVMS)
                                  750, /* test takes much longer on OpenVMS */
#else
                                  1000,
#endif
                                  32768,
                                  ACE_Thread_Manager::instance ());

          if (client_task.activate (THR_NEW_LWP | THR_JOINABLE, 4, 1) == -1)
              {
              ACE_ERROR ((LM_ERROR, "Error activating client task\n"));
              }
          ACE_Thread_Manager::instance ()->wait ();

          CORBA::Long count = receiver->get_event_count ();

          ACE_DEBUG ((LM_DEBUG, "(%P) - Receiver got %d messages\n",
                      count));
        }
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
