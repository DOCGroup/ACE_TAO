#include "TM_DaemonC.h"
#include "ace/Get_Opt.h"
#include "utils/RT.h"
#include "utils/Timer.h"
namespace CIAO
{
  namespace TM_Daemon
  {

    const char *ior = "file://daemon.ior";
    const char *output = "latency.txt";
    size_t duration = 5;
    size_t iterations = 100;

    int
    parse_args (int argc, char *argv[])
    {
      ACE_Get_Opt get_opts (argc, argv, "k:s:i:o:");
      int c;

      while ((c = get_opts ()) != -1)
        switch (c)
          {
            case 'k':
              ior = get_opts.opt_arg ();
              break;

            case 's':
              duration = (size_t) strtod (get_opts.opt_arg (), 0);
              break;

            case 'i':
              iterations = (size_t) strtod (get_opts.opt_arg (), 0);
              break;

            case 'o':
              output = get_opts.opt_arg ();
              break;

            case '?':
            default:
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "usage:  %s "
                                 "-k <ior> "
                                 "-s <period> "
                                 "-i <numer of iterations> "
                                 "-o <output file to record e-2-e latency> "
                                 "\n",
                                 argv [0]),
                                -1);
          }
      // Indicates sucessful parsing of the command line
      return 0;
    }

    int
    run_main (int argc, char *argv[])
    {
      try
        {
          CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

          if (parse_args (argc, argv) != 0)
            {
              return 1;
            }

          utils::set_priority ();

          CORBA::Object_var tmp = orb->string_to_object(ior);

          Daemon_var daemon = Daemon::_narrow(tmp.in ());

          if (CORBA::is_nil (daemon.in ()))
            {
              ACE_ERROR_RETURN ((LM_DEBUG,
                                 "Nil Daemon reference <%s>\n",
                                 ior),
                                1);
            }
          utils::Timer latency (output);
          for (size_t i = 0; i < iterations; ++i)
            {
              for (size_t j = 0; j < 100; ++j)
                {
//                  ACE_DEBUG ((LM_DEBUG, "%d Invoking get_snapshot ... ", i));
                  latency.start ();
                  ::Deployment::Domain_var domain = daemon->getSnapshot ();
                  latency.stop ();
                  latency.dump ();
//                  ACE_DEBUG ((LM_DEBUG, "done!\n"));
                }
              sleep (duration);
            }
          orb->destroy ();
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("CIAO_TM_Daemon::Client::main\n");
          return -1;
        }
      return 0;
    }
  }
}

int
main (int argc, char *argv[])
{
  return CIAO::TM_Daemon::run_main (argc, argv);
}
