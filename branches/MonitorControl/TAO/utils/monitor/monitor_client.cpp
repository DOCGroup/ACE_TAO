// $Id$

#include "ace/streams.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Get_Opt.h"

#include "tao/Strategies/advanced_resource.h"
#include "tao/Monitor/Monitor.h"

const char *monitor_ior = "file://monitor.ior";
::Monitor::MC::NameList* monitor_point = 0;
bool mp_clear = false;
int n_iterations = 1;
ACE_Time_Value sleep_time = ACE_Time_Value (1);


int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:p:ci:s:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        monitor_ior = get_opts.opt_arg ();
        break;
      case 'p':
        {
          if (monitor_point == 0)
            {
              ACE_NEW_THROW_EX (monitor_point,
                                ::Monitor::MC::NameList,
                                CORBA::NO_MEMORY ());
            }
          monitor_point->length (monitor_point->length() + 1);
          (*monitor_point)[monitor_point->length() - 1] = CORBA::string_dup (get_opts.opt_arg ());
          break;
        }
      case 'c':
        mp_clear = true;
        break;
      case 'i':
        n_iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 's':
        sleep_time.set(ACE_OS::atoi (get_opts.opt_arg ()), 0);
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
              Monitor::MC::DataListList_var data;
              if (mp_clear)
                {
                  data = monitor->get_and_clear_statistics (*monitor_point);
                }
              else
                {
                  data = monitor->get_statistics (*monitor_point);
                }

              for (CORBA::ULong index = 0; index < data->length(); index ++)
                {
                  ACE_DEBUG ((LM_DEBUG, "MP <%s>:\n", data[index].itemname));
                  Monitor::MC::DataItem dlist = data[index];
                  for (CORBA::ULong valueindex = 0; valueindex < dlist.dlist.length(); valueindex++)
                  {
                    Monitor::MC::Data d = dlist.dlist[valueindex];
                    ACE_DEBUG ((LM_DEBUG, "\t value <%A>:\n", d.value));
                  }
                }

              ACE_OS::sleep (sleep_time);
            }
        }
      else
        {
          Monitor::MC::NameList_var list = monitor->get_statistic_names ();
          for (CORBA::ULong index = 0; index < list->length(); ++index)
            {
              ACE_DEBUG ((LM_DEBUG, "MP: <%s>\n", list[index].in()));
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
