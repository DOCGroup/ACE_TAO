// $Id$

#include "TestC.h"
#include "ace/High_Res_Timer.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/Monitor/Monitor.h"

ACE_RCSID (Throughput,
           client,
           "$Id$")

const char *test_ior = "file://test.ior";
const char *monitor_ior = "monitor.ior";
int message_size  = 2048;
int message_count = 10 * 1024;
int test_runs   = 6;
bool do_shutdown = false;
bool expose_monitors = false;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:b:i:n:mx");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        test_ior = get_opts.opt_arg ();
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
        
      case 'm':
        expose_monitors = true;

      case 'x':
        do_shutdown = true;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-b <message_size> "
                           "-i <message_count> "
                           "-n <test_repetitions> "
                           "-m "
                           "-x "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

/// Runs the ORB in a separate thread so we can listen
/// for calls on our MC interface without blocking.
class ORB_Runner : public ACE_Task_Base
{
private:
  CORBA::ORB_ptr orb_;
  
public:
  ORB_Runner (CORBA::ORB_ptr orb)
    : orb_ (orb)
  {}
  
  virtual int svc (void)
  {
    this->orb_->run ();
    return 0;
  }
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        {
          return 1;
        }
        
      ORB_Runner orb_runner (orb.in ());

      if (expose_monitors)
        {
          /// Here we take on aspects of a server role, exposing the
          /// Monitor interface of our ORB so the monitoring client
          /// can access it remotely. No need to activate the POA
          /// manager etc here - it is done in the Object_Loader code
          /// in the Monitor lib.
          CORBA::Object_var obj =
            orb->resolve_initial_references ("Monitor");
          Monitor::MC_var monitor = Monitor::MC::_narrow (obj.in ());
          
          CORBA::String_var mon_ior_str =
            orb->object_to_string (monitor.in ());
          
	        FILE *output_file = ACE_OS::fopen (monitor_ior, "w");

	        if (output_file == 0)
            {
	            ACE_ERROR_RETURN ((LM_ERROR,
			                            "Can't open output file for writing IOR: %s",
			                            monitor_ior),
			                          1);
            }

	        ACE_OS::fprintf (output_file, 
                            "%s", 
                            mon_ior_str.in ());
	        ACE_OS::fclose (output_file);
    	    
	        /// Call orb->run() in a separate thread so the MC interface can
	        /// get remote calls.
	        orb_runner.activate ();
	      }

      CORBA::Object_var tmp =
        orb->string_to_object (test_ior);

      Test::Receiver_Factory_var receiver_factory =
        Test::Receiver_Factory::_narrow (tmp.in ());

      if (CORBA::is_nil (receiver_factory.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil receiver factory reference <%s>\n",
                             test_ior),
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
            receiver_factory->create_receiver ();

          ACE_hrtime_t start = ACE_OS::gethrtime ();
          
          for (int i = 0; i != message_count; ++i)
            {
              message.message_id = i;
              receiver->receive_data (message);
            }

          receiver->done ();
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
          double mbits  = bytes * 8 / 1000000;

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
          receiver_factory->shutdown ();
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
