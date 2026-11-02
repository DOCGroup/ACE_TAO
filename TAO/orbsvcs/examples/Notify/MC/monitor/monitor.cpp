#include "ace/Get_Opt.h"
#include "ace/OS_NS_ctype.h"

#include "orbsvcs/Notify/MonitorControl/NotificationServiceMCC.h"

#include <cstring>

static ACE_TCHAR const* monitor_ior = nullptr;
static char const* shutdown_cmd = "shutdown";
static char const* rm_consumer = "remove_consumer";
static char const* rm_supplier = "remove_supplier";
static char const* rm_consumeradmin = "remove_consumeradmin";
static char const* rm_supplieradmin = "remove_supplieradmin";

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c;

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

  return 0;
}

extern "C" int
sorter (void const* a, void const* b)
{
  char const* left = *(reinterpret_cast<char const* const*> (a));
  char const* right = *(reinterpret_cast<char const* const*> (b));
  return std::strcmp (left, right);
}

bool
process_command (CosNotification::NotificationServiceMonitorControl_ptr nsm,
                 char* buf,
                 char const* command)
{
  if (std::strstr (buf, command) == buf)
    {
      char const* name = buf + std::strlen (command);
      bool space = false;
      size_t i = 0;

      while (ACE_OS::ace_isspace (name[i]))
        {
          space = true;
          ++i;
        }

      if (space)
        {
          try
            {
              char const* start = name + i;

              if (std::strcmp (command, shutdown_cmd) == 0)
                {
                  nsm->shutdown_event_channel (start);
                }
              else if (std::strcmp (command, rm_consumer) == 0)
                {
                  nsm->remove_consumer (start);
                }
              else if (std::strcmp (command, rm_supplier) == 0)
                {
                  nsm->remove_supplier (start);
                }
              else if (std::strcmp (command, rm_consumeradmin) == 0)
                {
                  nsm->remove_consumeradmin (start);
                }
              else if (std::strcmp (command, rm_supplieradmin) == 0)
                {
                  nsm->remove_supplieradmin (start);
                }
            }
          catch (const CORBA::Exception& ex)
            {
              std::strcat (buf, ": ");
              ex._tao_print_exception (buf);
            }

          return true;
        }
    }

  return false;
}

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  int status = 0;

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

      bool done = false;
      constexpr size_t lsize = 1024;
      char prev[lsize];

      while (!done)
        {
          ACE_OS::printf ("NotifyService> ");
          ACE_OS::fflush (stdout);

          char line[lsize] = "";
          char* rl = ACE_OS::fgets (line, lsize, stdin);

          if (rl != nullptr)
            {
              int const len = static_cast<int> (std::strlen (line));

              for (int i = 0; i < len && ACE_OS::ace_isspace (rl[0]); ++i)
                {
                  rl++;
                }

              for (int i = len - 1; i >= 0 && ACE_OS::ace_isspace (line[i]); --i)
                {
                  line[i] = '\0';
                }

              if (std::strlen (rl) == 0 || std::strcmp (rl, "!!") == 0)
                {
                  std::strcpy (line, prev);
                  rl = line;
                }

              std::strcpy (prev, line);
            }


          if (rl == nullptr)
            {
              done = true;
              ACE_DEBUG ((LM_DEBUG, "\n"));
            }
          else if (std::strlen (rl) == 0)
            {
            }
          else if (std::strcmp (rl, "quit") == 0)
            {
              done = true;
            }
          else if (std::strcmp (rl, "help") == 0)
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
          else if (std::strcmp (rl, "names") == 0)
            {
              try
                {
                  Monitor::NameList_var names = nsm->get_statistic_names ();
                  CORBA::ULong length = names->length ();
                  ACE_DEBUG ((LM_DEBUG, "Statistic names\n"));

                  // It's much easier to read once it's sorted
                  char const** narray = nullptr;
                  ACE_NEW_THROW_EX (narray,
                                    char const* [length],
                                    CORBA::NO_MEMORY ());

                  for (CORBA::ULong i = 0; i < length; ++i)
                    {
                      narray[i] = names[i].in ();
                    }

                  ACE_OS::qsort (narray,
                                 length,
                                 sizeof (const char*),
                                 sorter);

                  for (CORBA::ULong i = 0; i < length; ++i)
                    {
                      ACE_DEBUG ((LM_DEBUG, "  %s\n", narray[i]));
                    }

                  delete [] narray;
                }
              catch (const CORBA::Exception& ex)
                {
                  ex._tao_print_exception ("names: ");
                }
            }
          else
            {
              if (process_command (nsm.in (), rl, shutdown_cmd))
                {
                  continue;
                }
              else if (process_command (nsm.in (), rl, rm_consumer))
                {
                  continue;
                }
              else if (process_command (nsm.in (), rl, rm_supplier))
                {
                  continue;
                }
              else if (process_command (nsm.in (), rl, rm_consumeradmin))
                {
                  continue;
                }
              else if (process_command (nsm.in (), rl, rm_supplieradmin))
                {
                  continue;
                }

              try
                {
                  Monitor::Data_var data =
                    nsm->get_statistic (rl);
                  ACE_DEBUG ((LM_DEBUG, "%s => ", rl));

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
              catch (const CORBA::Exception& ex)
                {
                  std::strcat (rl, ": ");
                  ex._tao_print_exception (rl);
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
      status++;
    }

  return status;
}
