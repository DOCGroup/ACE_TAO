// $Id$

#include "TestC.h"
#include "ace/High_Res_Timer.h"
#include "ace/Get_Opt.h"
#include "tao/Strategies/advanced_resource.h"

ACE_RCSID(Throughput, client, "$Id$")

const char *ior = "file://test.ior";
int message_size  = 2048;
int message_count = 10 * 1024;
int test_runs   = 6;
int do_shutdown = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:b:i:n:x");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'b':
        message_size = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'i':
        message_count = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'n':
        test_runs = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'x':
        do_shutdown = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-b <message_size> "
                           "-i <message_count> "
                           "-n <test_repetitions> "
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
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Receiver_Factory_var receiver_factory =
        Test::Receiver_Factory::_narrow(tmp.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (receiver_factory.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil receiver factory reference <%s>\n",
                             ior),
                            1);
        }

      ACE_UINT32 gsf =
        ACE_High_Res_Timer::global_scale_factor ();

      Test::Message message;

      for (int j = 0; j != test_runs; ++j)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Testing with %d bytes per message\n",
                      message_size));

          message.the_payload.length (message_size);

          Test::Receiver_var receiver =
            receiver_factory->create_receiver (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_hrtime_t start = ACE_OS::gethrtime ();
          for (int i = 0; i != message_count; ++i)
            {
              message.message_id = i;
              receiver->receive_data (message ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }

          receiver->done (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
          ACE_hrtime_t elapsed_time = ACE_OS::gethrtime () - start;

          // convert to microseconds
#if !defined ACE_LACKS_LONGLONG_T
          ACE_UINT32 usecs = ACE_UINT32(elapsed_time / gsf);
#else  /* ! ACE_LACKS_LONGLONG_T */
          ACE_UINT32 usecs = elapsed_time / gsf;
#endif /* ! ACE_LACKS_LONGLONG_T */

          double bytes =
            (1000000.0 * message_count * message_size) / usecs;
          double kbytes = bytes / 1024;
          double mbytes = kbytes / 1024;
          double mbits  = bytes * 8 / 10000000;

          ACE_DEBUG ((LM_DEBUG,
                      "Sender[%d] %f (bytes/sec), "
                      "%f (Kb/sec)\n"
                      "Sender[%d] %f (Mb/sec), %f Mbits\n",
                      message_size, bytes, kbytes,
                      message_size, mbytes, mbits));

          message_size *= 2;
        }

      if (do_shutdown)
        {
          receiver_factory->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
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
