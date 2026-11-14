#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

#include "orbsvcs/Notify/MonitorControl/NotificationServiceMCC.h"

#include <algorithm>
#include <cstring>
#include <string>
#include <vector>

static ACE_TCHAR const* monitor_ior {};
constexpr char shutdown_cmd[] = "shutdown";
constexpr char rm_consumer[] = "remove_consumer";
constexpr char rm_supplier[] = "remove_supplier";
constexpr char rm_consumeradmin[] = "remove_consumeradmin";
constexpr char rm_supplieradmin[] = "remove_supplieradmin";

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'k':
          monitor_ior = get_opts.opt_arg ();
          break;
        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "usage: %s "
                             "-k <ior> "
                             "\n",
                             argv [0]),
                            -1);
        }
    }

  if (monitor_ior == nullptr)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "usage: %s -k <ior>\n",
                         argv [0]),
                        -1);
    }

  return 0;
}

static bool
process_command (CosNotification::NotificationServiceMonitorControl_ptr nsm,
                 std::string const& input,
                 char const* command)
{
  size_t const command_length = std::strlen (command);
  if (input.compare (0, command_length, command) == 0)
    {
      std::string const name = input.substr (command_length);
      size_t const argument_start = name.find_first_not_of (" \t\n\r\f\v");
      if (argument_start != std::string::npos)
        {
          std::string const argument = name.substr (argument_start);
          try
            {
              if (std::strcmp (command, shutdown_cmd) == 0)
                {
                  nsm->shutdown_event_channel (argument.c_str ());
                }
              else if (std::strcmp (command, rm_consumer) == 0)
                {
                  nsm->remove_consumer (argument.c_str ());
                }
              else if (std::strcmp (command, rm_supplier) == 0)
                {
                  nsm->remove_supplier (argument.c_str ());
                }
              else if (std::strcmp (command, rm_consumeradmin) == 0)
                {
                  nsm->remove_consumeradmin (argument.c_str ());
                }
              else if (std::strcmp (command, rm_supplieradmin) == 0)
                {
                  nsm->remove_supplieradmin (argument.c_str ());
                }
            }
          catch (CORBA::Exception const& ex)
            {
              std::string const error_prefix = input + ": ";
              ex._tao_print_exception (error_prefix.c_str ());
            }

          return true;
        }
    }

  return false;
}

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  int status {};

  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        {
          return 1;
        }

      CORBA::Object_var obj = orb->string_to_object (
        ACE_TEXT_ALWAYS_CHAR (monitor_ior));
      CosNotification::NotificationServiceMonitorControl_var nsm =
        CosNotification::NotificationServiceMonitorControl::_narrow (obj.in ());

      if (CORBA::is_nil (nsm.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to locate the "
                             "Notification Service Monitor\n"),
                            1);
        }

      bool done {};
      constexpr size_t line_size = 1024;
      std::string previous_line;

      while (!done)
        {
          ACE_OS::printf ("NotifyService> ");
          ACE_OS::fflush (stdout);

          char line_buffer[line_size] {};
          if (ACE_OS::fgets (line_buffer, line_size, stdin) == nullptr)
            {
              done = true;
              ACE_DEBUG ((LM_DEBUG, "\n"));
              continue;
            }

          std::string line (line_buffer);
          size_t const first = line.find_first_not_of (" \t\n\r\f\v");
          if (first == std::string::npos)
            {
              line.clear ();
            }
          else
            {
              size_t const last = line.find_last_not_of (" \t\n\r\f\v");
              line = line.substr (first, last - first + 1);
            }

          if (line.empty () || line == "!!")
            {
              line = previous_line;
            }
          previous_line = line;

          if (line.empty ())
            {
              continue;
            }

          if (line == "quit")
            {
              done = true;
            }
          else if (line == "help")
            {
              ACE_DEBUG ((LM_DEBUG,
                          "names                - Get a list of "
                          "currently available statistic names.\n"
                          "quit                 - Exit the monitor.\n"
                          "remove_consumer      - Remove a consumer "
                          "with the given name.\n"
                          "remove_supplier      - Remove a supplier "
                          "with the given name.\n"
                          "remove_consumeradmin - Remove a consumer "
                          "admin with the given name.\n"
                          "remove_supplieradmin - Remove a supplier "
                          "admin with the given name.\n"
                          "shutdown             - Shut down an "
                          "event channel with the given name.\n"
                          "<statistic name>     - Return the "
                          "information for the specified statistic.\n"));
            }
          else if (line == "names")
            {
              try
                {
                  Monitor::NameList_var names = nsm->get_statistic_names ();
                  CORBA::ULong const length = names->length ();
                  ACE_DEBUG ((LM_DEBUG, "Statistic names\n"));

                  // It's much easier to read once it's sorted
                  std::vector<char const*> sorted_names;
                  sorted_names.reserve (length);

                  for (CORBA::ULong i = 0; i < length; ++i)
                    {
                      sorted_names.push_back (names[i].in ());
                    }

                  std::sort (sorted_names.begin (),
                             sorted_names.end (),
                             [] (char const* left, char const* right)
                             {
                               return std::strcmp (left, right) < 0;
                             });

                  for (char const* name : sorted_names)
                    {
                      ACE_DEBUG ((LM_DEBUG, "  %s\n", name));
                    }
                }
              catch (CORBA::Exception const& ex)
                {
                  ex._tao_print_exception ("names: ");
                }
            }
          else
            {
              if (process_command (nsm.in (), line, shutdown_cmd))
                {
                  continue;
                }
              else if (process_command (nsm.in (), line, rm_consumer))
                {
                  continue;
                }
              else if (process_command (nsm.in (), line, rm_supplier))
                {
                  continue;
                }
              else if (process_command (nsm.in (), line, rm_consumeradmin))
                {
                  continue;
                }
              else if (process_command (nsm.in (), line, rm_supplieradmin))
                {
                  continue;
                }

              try
                {
                  Monitor::Data_var data =
                    nsm->get_statistic (line.c_str ());
                  ACE_DEBUG ((LM_DEBUG, "%s => ", line.c_str ()));

                  if (data->data_union._d () == Monitor::DATA_NUMERIC)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                  "Last: %g Average: %g\n",
                                  data->data_union.num().dlist[0].value,
                                  data->data_union.num().average));
                    }
                  else
                    {
                      Monitor::NameList list = data->data_union.list ();
                      CORBA::ULong const length = list.length ();

                      for (CORBA::ULong i = 0; i < length; ++i)
                        {
                          ACE_DEBUG ((LM_DEBUG, "%s ", list[i].in ()));
                        }

                      ACE_DEBUG ((LM_DEBUG, "\n"));
                    }
                }
              catch (CORBA::Exception const& ex)
                {
                  std::string const error_prefix = line + ": ";
                  ex._tao_print_exception (error_prefix.c_str ());
                }
            }
        }

      orb->destroy ();
    }
  catch (const CORBA::UserException& ex)
    {
      ex._tao_print_exception ("Notification Service Monitor: ");
      ++status;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Notification Service Monitor: ");
      ++status;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR,
                  "Notification Service Monitor: "
                  "unexpected exception type\n"));
      ++status;
    }

  return status;
}
