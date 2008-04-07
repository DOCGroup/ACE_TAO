// $Id$

#include "ace/streams.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Get_Opt.h"

#include "tao/Monitor/Monitor.h"

const char *monitor_ior = "file://monitor.ior";
const char *monitor_point = 0;
bool mp_clear = false;
int n_iterations = 1;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:p:ci:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        monitor_ior = get_opts.opt_arg ();
        break;
      case 'p':
        monitor_point = get_opts.opt_arg ();
        break;
      case 'c':
        mp_clear = true;
        break;
      case 'i':
        n_iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-p <point> "
                           "-i <iterations> "
                           "-c clear"
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
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc,  argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      /// Get the MC object reference that the client has exposed.
      CORBA::Object_var obj =
        orb->string_to_object (monitor_ior);

      if (CORBA::is_nil (obj.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Monitor client - string_to_object failed\n"),
                             -1);
        }

      Monitor::MC_var monitor = Monitor::MC::_narrow (obj.in ());

      if (CORBA::is_nil (monitor.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Monitor client - narrow failed\n"),
                             -1);
        }

      if (monitor_point != 0)
        {
          /// Access the monitor's value a few times and watch it grow.
          for (int i = 0; i < n_iterations; ++i)
            {
              Monitor::MC::DataList_var data;
              Monitor::MC::NameList seq;
              seq.length (1);
              seq[0] = CORBA::string_dup (monitor_point);
              if (mp_clear)
                {
                  data = monitor->get_and_clear_statistics (seq);
                }
              else
                {
                  data = monitor->get_statistics (seq);
                }

              cout << "client marshal buffer size = " << data[0].value
                   << " bytes" << endl;

              ACE_OS::sleep (1);
            }
        }
      else
        {
          Monitor::MC::NameList_var list = monitor->get_statistic_names ();
          for (CORBA::ULong index = 0; index < list->length(); ++index)
            {
              ACE_DEBUG ((LM_DEBUG, "MP: %s\n", list[index].in()));
            }
        }

      orb->destroy ();
    }
  catch (CORBA::Exception &ex)
    {
      ex._tao_print_exception ("Monitor_client: Exception caught:");
      return 1;
    }

  return 0;
}
