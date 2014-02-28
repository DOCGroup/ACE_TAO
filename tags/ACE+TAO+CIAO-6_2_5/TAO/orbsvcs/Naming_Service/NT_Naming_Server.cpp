// -*- C++ -*-

//=============================================================================
/**
 *  @file    NT_Naming_Server.cpp
 *
 *  $Id$
 *
 *  Driver program that runs the TAO Naming Service as a Windows NT
 *  Service.
 *
 *
 *  @author John Tucker <jtucker@infoglide.com> and Mike Vitalo <mvitalo@infoglide.com>
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
#include "ace/Init_ACE.h"

#include "winreg.h"
#include "NT_Naming_Service.h"

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
  Options (void);
  ~Options (void);

  int run (int argc, ACE_TCHAR *argv[]);

private:
  void parse_args (int argc,
                   ACE_TCHAR *argv[]);
  void print_usage_and_die (void);

private:
  ACE_TCHAR progname[128];

  int opt_install;
  int opt_remove;
  int opt_start;
  int opt_kill;
  int opt_type;
  int opt_debug;

  int opt_startup;
};

typedef ACE_Singleton<Options, ACE_Mutex> OPTIONS;

Options::Options (void)
  : opt_install (0),
    opt_remove (0),
    opt_start (0),
    opt_kill (0),
    opt_type (0),
    opt_debug (0),
    opt_startup (0)
{
  ACE_OS::strcpy (progname,
                  ACE_TEXT("service"));
  ACE::init ();
}

Options::~Options (void)
{
  ACE::fini ();
}

void
Options::print_usage_and_die (void)
{
  ORBSVCS_DEBUG ((LM_INFO,
              ACE_TEXT("Usage: %s")
              ACE_TEXT(" -in -r -s -k -tn -d\n")
              ACE_TEXT("  -i: Install this program as an NT service, with specified startup\n")
              ACE_TEXT("  -r: Remove this program from the Service Manager\n")
              ACE_TEXT("  -s: Start the service\n")
              ACE_TEXT("  -k: Kill the service\n")
              ACE_TEXT("  -t: Set startup for an existing service\n")
              ACE_TEXT("  -d: Debug; run as a regular application\n"),
              progname,
              0));
  ACE_OS::exit (1);
}

void
Options::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("i:rskt:d"));
  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
      {
      case 'i':
        opt_install = 1;
        opt_startup = ACE_OS::atoi (get_opt.opt_arg ());
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
        opt_startup = ACE_OS::atoi (get_opt.opt_arg ());
        if (opt_startup <= 0)
          print_usage_and_die ();
        break;
      case 'd':
        //opt_debug = 1;
        break;
      default:
        // -i can also be given without a value - if so, it defaults
        // to defined value.
        if (ACE_OS::strcmp (get_opt.argv ()[get_opt.opt_ind () - 1], ACE_TEXT ("-i")) == 0)
          {
            opt_install = 1;
            opt_startup = DEFAULT_SERVICE_INIT_STARTUP;
          }
        else
          this->print_usage_and_die ();
        break;
      }
}

// Define a function to handle Ctrl+C to cleanly shut this down.

static BOOL __stdcall
ConsoleHandler (DWORD /* ctrlType */)
{
  SERVICE::instance ()->handle_control (SERVICE_CONTROL_STOP);
  return TRUE;
}

ACE_NT_SERVICE_DEFINE (service,
                       TAO_NT_Naming_Service,
                       ACE_TEXT ("TAO NT Naming Service"));

int
Options::run (int argc, ACE_TCHAR* argv[])
{
  SERVICE::instance ()->name (ACE_TEXT ("TAO_NT_Naming_Service"),
                              ACE_TEXT ("TAO NT Naming Service"));

  this->parse_args (argc, argv);

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
      ACE_NT_SERVICE_RUN (service,
                          SERVICE::instance (),
                          ret);
      if (ret == 0)
        ORBSVCS_ERROR ((LM_ERROR,
                    "%p\n",
                    "Couldn't start service"));
    }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  return OPTIONS::instance ()->run (argc, argv);
}

#endif /* !ACE_WIN32 || ACE_LACKS_WIN32_SERVICES */
