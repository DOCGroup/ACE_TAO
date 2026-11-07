
//=============================================================================
/**
 *  @file    main.cpp
 *
 *  This is the main program - it just hands control off to the
 *  process instance to figure out what to do.  This program only
 *  runs on Win32.
 *
 *  @author Gonzalo Diethelm <gonzo@cs.wustl.edu> and Steve Huston <shuston@riverace.com>
 */
//=============================================================================


#include "ace/Get_Opt.h"
#include "ace/Init_ACE.h"
#include "ntsvc.h"

#if defined (ACE_WIN32) && !defined (ACE_LACKS_WIN32_SERVICES)
// FUZZ: disable check_for_streams_include
#include "ace/streams.h"
#include "ace/OS_NS_errno.h"

#include <memory>

// Default for the -i (install) option
#define DEFAULT_SERVICE_INIT_STARTUP     SERVICE_AUTO_START

class Process
{
public:
  Process ();
  Process (Process const &) = delete;
  Process (Process &&) = delete;
  Process &operator= (Process const &) = delete;
  Process &operator= (Process &&) = delete;
  ~Process ();

  int run(int argc, ACE_TCHAR* argv[]);

private:
  void parse_args (int argc,
                   ACE_TCHAR* argv[]);
  void print_usage_and_die ();

private:
  static constexpr char progname[] = "service";

  bool opt_install {};
  bool opt_remove {};
  bool opt_start {};
  bool opt_kill {};
  bool opt_type {};
  bool opt_debug {};

  int opt_startup;
};

using PROCESS = ACE_Singleton<Process, ACE_Mutex>;

Process::Process ()
  : opt_startup (0)
{
  ACE::init ();
}

Process::~Process ()
{
  ACE::fini ();
}

void
Process::print_usage_and_die ()
{
  ACE_DEBUG ((LM_INFO,
              "Usage: %s"
              " -in -r -s -k -tn -d\n"
              "  -i: Install this program as an NT service, with specified startup\n"
              "  -r: Remove this program from the Service Manager\n"
              "  -s: Start the service\n"
              "  -k: Kill the service\n"
              "  -t: Set startup for an existing service\n"
              "  -d: Debug; run as a regular application\n",
              progname));
  ACE_OS::exit(1);
}

void
Process::parse_args (int argc, ACE_TCHAR* argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("i:rskt:d"));
  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
    {
    case 'i':
      opt_install = true;
      opt_startup = ACE_OS::atoi (get_opt.opt_arg ());
      if (opt_startup <= 0)
         print_usage_and_die ();
      break;
    case 'r':
      opt_remove = true;
      break;
    case 's':
      opt_start = true;
      break;
    case 'k':
      opt_kill = true;
      break;
    case 't':
      opt_type = true;
      opt_startup = ACE_OS::atoi (get_opt.opt_arg ());
      if (opt_startup <= 0)
         print_usage_and_die ();
      break;
    case 'd':
      opt_debug = true;
      break;
    default:
      // -i can also be given without a value - if so, it defaults
      // to defined value.
      if (ACE_OS::strcmp (get_opt.argv ()[get_opt.opt_ind () - 1], ACE_TEXT ("-i")) == 0)
      {
        opt_install = true;
        opt_startup = DEFAULT_SERVICE_INIT_STARTUP;
      }
      else
      {
        print_usage_and_die ();
      }
      break;
  }
}

// Define a function to handle Ctrl+C to cleanly shut this down.

static BOOL WINAPI
ConsoleHandler (DWORD /*ctrlType*/)
{
  SERVICE::instance ()->handle_control (SERVICE_CONTROL_STOP);
  return TRUE;
}

ACE_NT_SERVICE_DEFINE (Beeper,
                       Service,
                       ACE_TEXT ("Annoying Beeper Service"));

int
Process::run (int argc, ACE_TCHAR* argv[])
{
  SERVICE::instance ()->name (ACE_TEXT ("Beeper"),
                              ACE_TEXT ("Annoying Beeper Service"));

  parse_args (argc, argv);

  if (opt_install && !opt_remove)
    {
      if (-1 == SERVICE::instance ()->insert (opt_startup))
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("insert")));
          return -1;
        }
      return 0;
    }

  if (opt_remove && !opt_install)
    {
      if (-1 == SERVICE::instance ()->remove ())
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("remove")));
          return -1;
        }
      return 0;
    }

  if (opt_start && opt_kill)
    print_usage_and_die ();

  if (opt_start)
    {
      if (-1 == SERVICE::instance ()->start_svc ())
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("start")));
          return -1;
        }
      return 0;
    }

  if (opt_kill)
    {
      if (-1 == SERVICE::instance ()->stop_svc ())
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("stop")));
          return -1;
        }
      return 0;
    }

  if (opt_type)
    {
      if (-1 == SERVICE::instance ()->startup (opt_startup))
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("set startup")));
          return -1;
        }
      return 0;
    }

  // If we get here, we either run the app in debug mode (-d) or are
  // being called from the service manager to start the service.

  if (opt_debug)
    {
      SetConsoleCtrlHandler (&ConsoleHandler, 1);
      SERVICE::instance ()->svc ();
    }
  else
    {
      std::unique_ptr<ofstream> output_file = std::make_unique<ofstream> ("ntsvc.log", ios::out);
      if (output_file && output_file->rdstate() == ios::goodbit)
        {
          ACE_LOG_MSG->msg_ostream (output_file.get (), true);
          output_file.release ();
        }
      ACE_LOG_MSG->open(argv[0],
                        ACE_Log_Msg::STDERR | ACE_Log_Msg::OSTREAM,
                        nullptr);
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%T (%t): Starting service.\n")));

      ACE_NT_SERVICE_RUN (Beeper,
                          SERVICE::instance (),
                          ret);
      if (ret == 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("%p\n"),
                    ACE_TEXT ("Couldn't start service")));
      else
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%T (%t): Service stopped.\n")));
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  return PROCESS::instance ()->run (argc, argv);
}

#else

#include "ace/OS_main.h"

int
ACE_TMAIN (int, ACE_TCHAR*[])
{
  // This program needs Windows services.
  return 0;
}

#endif /* ACE_WIN32 && !ACE_LACKS_WIN32_SERVICES */
