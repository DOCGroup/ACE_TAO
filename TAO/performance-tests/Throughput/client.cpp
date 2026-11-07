#include "TestC.h"
#include "ace/High_Res_Timer.h"
#include "ace/Get_Opt.h"
#include "tao/Strategies/advanced_resource.h"

struct Client_Options
{
  ACE_TCHAR const *ior = ACE_TEXT("file://test.ior");
  int message_size = 2048;
  int message_count = 10 * 1024;
  int test_runs = 6;
  bool shutdown = false;
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:b:i:n:x"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case 'b':
        options.message_size = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'i':
        options.message_count = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'n':
        options.test_runs = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'x':
        options.shutdown = true;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("usage: %s -k <ior> -b <message_size> "
                                    "-i <message_count> -n <test_repetitions> [-x]\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      Client_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var tmp = orb->string_to_object (options.ior);

      Test::Receiver_Factory_var receiver_factory =
        Test::Receiver_Factory::_narrow(tmp.in ());

      if (CORBA::is_nil (receiver_factory.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT("Nil receiver factory reference <%s>\n"),
                             options.ior),
                            1);
        }

      ACE_High_Res_Timer::global_scale_factor_type const gsf =
        ACE_High_Res_Timer::global_scale_factor ();

      Test::Message message;
      int message_size = options.message_size;

      for (int j = 0; j != options.test_runs; ++j)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Testing with %d bytes per message\n",
                      message_size));

          message.the_payload.length (static_cast<CORBA::ULong> (message_size));

          Test::Receiver_var receiver =
            receiver_factory->create_receiver ();

          ACE_hrtime_t const start = ACE_OS::gethrtime ();
          for (int i = 0; i != options.message_count; ++i)
            {
              message.message_id = i;
              receiver->receive_data (message);
            }

          receiver->done ();
          ACE_hrtime_t const elapsed_time = ACE_OS::gethrtime () - start;

          // convert to microseconds
          ACE_UINT32 const usecs = ACE_UINT32 (elapsed_time / gsf);

          double bytes =
            (1000000.0 * options.message_count * message_size) / usecs;
          double kbytes = bytes / 1024;
          double mbytes = kbytes / 1024;
          double mbits  = bytes * 8 / 1000000;

          ACE_DEBUG ((LM_DEBUG,
                      "Sender[%d] %f (bytes/sec), "
                      "%f (Kb/sec)\n"
                      "Sender[%d] %f (Mb/sec), %f Mbits\n",
                      message_size, bytes, kbytes,
                      message_size, mbytes, mbits));

          message_size *= 2;
        }

      if (options.shutdown)
        {
          receiver_factory->shutdown ();
        }

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
