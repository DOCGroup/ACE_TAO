// $Id$

// ============================================================================
//
// = LIBRARY
//    examples/NT_Service
//
// = FILENAME
//    main.cpp
//
// = DESCRIPTION
//    This is the main program - it just hands control off to the
//    process instance to figure out what to do.  This program only
//    runs on Win32.
//
// = AUTHOR
//    Gonzalo Diethelm <gonzo@cs.wustl.edu> 
//    and Steve Huston <shuston@riverace.com>
//
// ============================================================================

#include "ace/Get_Opt.h"
#include "ntsvc.h"

// Default for the -i (install) option
#define DEFAULT_SERVICE_INIT_STARTUP     SERVICE_AUTO_START

class Process
{
public:
  Process (void);
  ~Process (void);

  int run(int argc, char* argv[]);

private:
  void parse_args (int argc,
                   char* argv[]);
  void print_usage_and_die (void);

private:
  char progname[128];

  int opt_install;
  int opt_remove;
  int opt_start;
  int opt_kill;
  int opt_type;
  int opt_debug;

  int opt_startup;
};

typedef ACE_Singleton<Process, ACE_Mutex> PROCESS;

Process::Process (void)
  : opt_install (0),
    opt_remove (0),
    opt_start (0),
    opt_kill (0),
    opt_type (0),
    opt_debug (0),
    opt_startup (0)
{
  ACE_OS::strcpy (progname,
                  "service");
  ACE::init ();
}

Process::~Process (void)
{
  ACE::fini ();
}

void 
Process::print_usage_and_die (void)
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
              progname,
              0));
  ACE_OS::exit(1);
}

void 
Process::parse_args (int argc, char* argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "i:rskt:d");
  int c; 

  while ((c = get_opt ()) != -1)
    switch (c)
    {
    case 'i':
      opt_install = 1;
      opt_startup = ACE_OS::atoi (get_opt.optarg);
      if (opt_startup <= 0)
         print_usage_and_die ();
      break;
    case 'r':
      opt_remove = 1;
      break;
    case 's':
      opt_start = 1;
      break;
    case 'k':
      opt_kill = 1;
      break;
    case 't':
      opt_type = 1;
      opt_startup = ACE_OS::atoi (get_opt.optarg);
      if (opt_startup <= 0)
         print_usage_and_die ();
      break;
    case 'd':
      opt_debug = 1;
      break;
    default:
      // -i can also be given without a value - if so, it defaults
      // to defined value.
      if (ACE_OS::strcmp (get_opt.argv_[get_opt.optind-1], "-i") == 0)
      {
        opt_install = 1;
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

static BOOL __stdcall
ConsoleHandler (DWORD ctrlType)
{
  SERVICE::instance ()->handle_control (SERVICE_CONTROL_STOP);
  return TRUE;
}

ACE_NT_SERVICE_DEFINE (Beeper,
                       Service,
                       "Annoying Beeper Service");

int 
Process::run (int argc, char* argv[])
{
  SERVICE::instance ()->name ("Beeper",
                              "Annoying Beeper Service");

  parse_args (argc, argv);

  if (opt_install && !opt_remove)
    return SERVICE::instance ()->insert (opt_startup);

  if (opt_remove && !opt_install)
    return SERVICE::instance ()->remove ();

  if (opt_start && opt_kill)
    print_usage_and_die ();

  if (opt_start)
    return SERVICE::instance ()->start_svc ();

  if (opt_kill)
    return SERVICE::instance ()->stop_svc ();

  if (opt_type)
    return SERVICE::instance ()->startup (opt_startup);

  // If we get here, we either run the app in debug mode (-d) or are
  // being called from the service manager to start the service.

  if (opt_debug)
    {
      SetConsoleCtrlHandler (&ConsoleHandler, 1);
      SERVICE::instance ()->svc ();
    }
  else
    {
      ACE_NT_SERVICE_RUN (Beeper,
                          SERVICE::instance (),
                          ret);
      if (ret == 0)
        ACE_ERROR ((LM_ERROR,
                    "%p\n",
                    "Couldn't start service"));
    }
  
  return 0;
}

int 
main (int argc, char* argv[])
{
  return PROCESS::instance ()->run (argc, argv);
}
