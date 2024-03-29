/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    NT_Notify_Server.cpp
 *
 *  Driver program that runs the TAO Notification Service as a Windows NT
 *  Service.
 *
 *  @author John Tucker <jtucker@infoglide.com>
 *  @author Mike Vitalo <mvitalo@infoglide.com>
 *  @author and David Robison <drrobison@openroadsconsulting.com>
 */
//=============================================================================


#include "orbsvcs/Log_Macros.h"

#if !defined (ACE_WIN32) || defined (ACE_LACKS_WIN32_SERVICES)

int
ACE_TMAIN(int, ACE_TCHAR *[])
{
  ORBSVCS_ERROR ((LM_INFO,
              "This program is only supported "
              "on Win32 platforms\n"));
  return 1;
}

#else

#include "ace/Get_Opt.h"
#include "ace/Arg_Shifter.h"
#include "ace/ARGV.h"
#include "ace/Configuration.h"
#include "ace/Init_ACE.h"

#include "winreg.h"
#include "NT_Notify_Service.h"

// Default for the -i (install) option
#define DEFAULT_SERVICE_INIT_STARTUP SERVICE_DEMAND_START

/**
 * @class Options
 *
 * @brief Keeps track of the command-line options for this program.
 */
class Options
{
public:
  Options ();
  ~Options ();

  int run (int argc, ACE_TCHAR *argv[]);

private:
  void parse_args (int argc, ACE_TCHAR *argv[]);
  void print_usage_and_die ();

private:
  ACE_TCHAR progname[128];

  int opt_setargs;
  const ACE_TCHAR *opt_args;
  int opt_install;
  int opt_remove;
  int opt_start;
  int opt_kill;
  int opt_type;
  int opt_debug;

  int opt_startup;
};

typedef ACE_Singleton<Options, ACE_Mutex> OPTIONS;

Options::Options ()
  : opt_setargs (0),
    opt_args (0),
    opt_install (0),
    opt_remove (0),
    opt_start (0),
    opt_kill (0),
    opt_type (0),
    opt_debug (0),
    opt_startup (0)
{
  ACE_OS::strcpy (progname,
                  ACE_TEXT ("service"));
  ACE::init ();
}

Options::~Options ()
{
  ACE::fini ();
}

void
Options::print_usage_and_die ()
{
  ORBSVCS_DEBUG ((LM_INFO,
              ACE_TEXT ("Usage: %s")
              ACE_TEXT (" -c [<args>] -in -r -s -k -tn -d\n")
              ACE_TEXT ("  -c: set or retrieve command line arguments for NT service\n")
              ACE_TEXT ("  -i: Install this program as an NT service, with specified startup\n")
              ACE_TEXT ("  -r: Remove this program from the Service Manager\n")
              ACE_TEXT ("  -s: Start the service\n")
              ACE_TEXT ("  -k: Kill the service\n")
              ACE_TEXT ("  -t: Set startup for an existing service\n")
              ACE_TEXT ("  -d: Debug; run as a regular application\n"),
              progname,
              0));
  ACE_OS::exit (1);
}

void
Options::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("c:i:rskt:d"));
  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
      {
      case 'c':
        opt_setargs = 1;
        opt_args = get_opt.opt_arg ();
        break;
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
        //opt_debug = 1;
        break;
      default:
        // -c and -i can also be given without a value - if so, it defaults
        // to defined value.
        const ACE_TCHAR *lastarg = get_opt.argv ()[get_opt.opt_ind () - 1];
        if (ACE_OS::strcmp (get_opt.argv_[get_opt.optind-1], ACE_TEXT ("-i")) == 0)
          {
            opt_install = 1;
            opt_startup = DEFAULT_SERVICE_INIT_STARTUP;
          }
        else if (ACE_OS::strcmp (lastarg, ACE_TEXT ("-c")) == 0)
          {
            opt_setargs = 1;
            opt_args = 0;
          }
        else
          this->print_usage_and_die ();
        break;
      }
}

// Define a function to handle Ctrl+C to cleanly shut this down.
static BOOL WINAPI
ConsoleHandler (DWORD /* ctrlType */)
{
  SERVICE::instance ()->handle_control (SERVICE_CONTROL_STOP);
  return TRUE;
}

ACE_NT_SERVICE_DEFINE (service,
                       TAO_NT_Notify_Service,
                       ACE_TEXT ("TAO NT Notify Service"));

int
Options::run (int argc, ACE_TCHAR* argv[])
{
  SERVICE::instance ()->name (ACE_TEXT ("TAO_NT_Notify_Service"),
                              ACE_TEXT ("TAO NT Notify Service"));

  this->parse_args (argc, argv);

  if (opt_install && !opt_remove)
    return SERVICE::instance ()->insert (opt_startup);

  if (opt_remove && !opt_install)
    return SERVICE::instance ()->remove ();

  if (opt_start && opt_kill)
    print_usage_and_die ();

  if (opt_setargs)
    return SERVICE::instance ()->set_args (opt_args);

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
      ACE_NT_SERVICE_RUN (service,
                          SERVICE::instance (),
                          ret);
      if (ret == 0)
        ORBSVCS_ERROR ((LM_ERROR,
                        ACE_TEXT ("%p\n"),
                        ACE_TEXT ("Couldn't start service")));
    }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  return OPTIONS::instance ()->run (argc, argv);
}

#endif /* !ACE_WIN32 || ACE_LACKS_WIN32_SERVICES */
