/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    NT_ImplRepo_Server.cpp
//
//
// = DESCRIPTION
//    Driver program that runs the TAO ImplRepo Service as a Windows NT
//    Service. 
//
// = AUTHORS
//    John Tucker <jtucker@infoglide.com> and
//    Mike Vitalo <mvitalo@infoglide.com>
//      modified by
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#include "ace/OS.h"
#include "ace/Get_Opt.h"

#include "winreg.h"
#include "NT_ImplRepo_Service.h"

ACE_RCSID(ImplRepo_Service, NT_ImplRepo_Server, "$Id$")

// Default for the -i (install) option.
#define DEFAULT_SERVICE_INIT_STARTUP SERVICE_DEMAND_START

class NTS_Options
{
  // = TITLE
  //   Keeps track of the command-line options for this program.
public:
  NTS_Options (void);
  ~NTS_Options (void);

  int run (int argc, char *argv[]);

private:
  void parse_args (int argc,
                   char *argv[]);

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

typedef ACE_Singleton<NTS_Options, ACE_Mutex> NTS_OPTIONS;

NTS_Options::NTS_Options (void)
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

NTS_Options::~NTS_Options (void)
{
  ACE::fini ();
}

void
NTS_Options::print_usage_and_die (void)
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
  ACE_OS::exit (1);
}

void
NTS_Options::parse_args (int argc, 
                         char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "i:rskt:d");
  int c; 

  while ((c = get_opt ()) != -1)
    switch (c)
      {
      case 'i':
        this->opt_install = 1;
        this->opt_startup = ACE_OS::atoi (get_opt.optarg);

        if (this->opt_startup <= 0)
          {
            print_usage_and_die ();
          }

        break;
      case 'r':
        this->opt_remove = 1;
        break;
      case 's':
        this->opt_start = 1;
        break;
      case 'k':
        this->opt_kill = 1;
        break;
      case 't':
        this->opt_type = 1;
        this->opt_startup = ACE_OS::atoi (get_opt.optarg);

        if (this->opt_startup <= 0)
          {
            print_usage_and_die ();
          }

        break;
      case 'd':
        //opt_debug = 1;
        break;
      default:
        // -i can also be given without a value - if so, it defaults
        // to defined value.
        if (ACE_OS::strcmp (get_opt.argv_[get_opt.optind - 1], "-i") == 0)
          {
            this->opt_install = 1;
            this->opt_startup = DEFAULT_SERVICE_INIT_STARTUP;
          }
        else
          {
            this->print_usage_and_die ();
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

ACE_NT_SERVICE_DEFINE (service,
                       TAO_NT_ImplRepo_Service,
                       "TAO NT ImplRepo Service");

int
NTS_Options::run (int argc, 
                  char* argv[])
{
  SERVICE::instance ()->name ("TAO_NT_ImplRepo_Service",
                              "TAO NT ImplRepo Service");

  this->parse_args (argc, 
                    argv);

  if (this->opt_install && !this->opt_remove)
    {
      return SERVICE::instance ()->insert (this->opt_startup);
    }

  if (this->opt_remove && !this->opt_install)
    {
      return SERVICE::instance ()->remove ();
    }

  if (this->opt_start && this->opt_kill)
    {
      print_usage_and_die ();
    }

  if (this->opt_start)
    {
      return SERVICE::instance ()->start_svc ();
    }

  if (this->opt_kill)
    {
      return SERVICE::instance ()->stop_svc ();
    }

  if (this->opt_type)
    {
      return SERVICE::instance ()->startup (this->opt_startup);
    }

  // If we get here, we either run the app in debug mode (-d) or are
  // being called from the service manager to start the service.

  if (this->opt_debug)
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
        {
          ACE_ERROR ((LM_ERROR,
                      "%p\n",
                      "Couldn't start service"));
        }
    }
  
  return 0;
}

int
main (int argc, char *argv[])
{	
  return NTS_OPTIONS::instance ()->run (argc, argv);
}

