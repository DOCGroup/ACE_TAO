#include "ace/streams.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Get_Opt.h"

#include "tao/Strategies/advanced_resource.h"
#include "tao/Monitor/Monitor.h"

struct Monitor_Client_Options
{
  ACE_TCHAR const * monitor_ior {ACE_TEXT ("file://monitor.ior")};
  Monitor::NameList monitor_points;
  bool clear {};
  int iterations {1};
  ACE_Time_Value sleep_time {1};
  ACE_TCHAR const * filter {ACE_TEXT ("")};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Monitor_Client_Options& options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:p:ci:s:f:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f':
        options.filter = get_opts.opt_arg ();
        break;
      case 'k':
        options.monitor_ior = get_opts.opt_arg ();
        break;
      case 'p':
        {
          CORBA::ULong const length = options.monitor_points.length ();
          options.monitor_points.length (length + 1);
          options.monitor_points[length] =
            CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ()));
          break;
        }
      case 'c':
        options.clear = true;
        break;
      case 'i':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 's':
        options.sleep_time.set (ACE_OS::atoi (get_opts.opt_arg ()), 0);
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-p <point> "
                           "-i <iterations> "
                           "-c clear "
                           "-s <sleeptime> "
                           "-f <filter> "
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      Monitor_Client_Options options;

      if (parse_args (argc, argv, options) != 0)
        {
          return 1;
        }

      /// Get the MC object reference that the client has exposed.
      CORBA::Object_var obj =
        orb->string_to_object (options.monitor_ior);

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

      if (options.monitor_points.length () != 0)
        {
          /// Access the monitor's value a few times and watch it grow.
          for (int i {}; i < options.iterations; ++i)
            {
              Monitor::DataList_var data;

              if (options.clear)
                {
                  data = monitor->get_and_clear_statistics (options.monitor_points);
                }
              else
                {
                  data = monitor->get_statistics (options.monitor_points);
                }

              for (CORBA::ULong index {}; index < data->length (); ++index)
                {
                  ACE_DEBUG ((LM_DEBUG, "MP <%C>:\n", data[index].itemname.in ()));
                  Monitor::Data const &dlist = data[index];

                  if (dlist.data_union._d() == Monitor::DATA_TEXT)
                  {
                    for (CORBA::ULong valueindex {};
                         valueindex < dlist.data_union.num().dlist.length ();
                         ++valueindex)
                    {
                      Monitor::DataValue const &d =
                        dlist.data_union.num ().dlist[valueindex];
                      ACE_DEBUG ((LM_DEBUG, "\t value <%A>:\n", d.value));
                    }
                  }
                }

              ACE_OS::sleep (options.sleep_time);
            }
        }
      else
        {
          Monitor::NameList_var list = monitor->get_statistic_names (ACE_TEXT_ALWAYS_CHAR (options.filter));

          for (CORBA::ULong index {}; index < list->length (); ++index)
            {
              ACE_DEBUG ((LM_DEBUG, "MP: <%C>\n", list[index].in ()));
            }
        }

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Monitor_client: Exception caught:");
      return 1;
    }

  return 0;
}
