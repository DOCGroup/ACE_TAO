//=============================================================================
/**
 *  @file    Options.cpp
 *
 *  $Id$
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#include "Options.h"
#include "NT_Service.h"
#include "ace/Get_Opt.h"
#include "ace/Configuration.h"


#define REGISTRY_KEY_ROOT HKEY_LOCAL_MACHINE
#define TAO_REGISTRY_SUBKEY "SOFTWARE\\ACE\\TAO"
#define TAO_IMPLREPO_SERVICE_OPTS_NAME "TaoImplRepoServiceOptions"


/**
 * Default Constructor.  Assigns default values to all the member variables.
 */
Options::Options ()
: service_ (0),
  debug_ (1),
  ior_output_file_ (0),
  config_ (0),
  startup_timeout_ (5),
  ping_interval_ (0, 200)
{
}


/**
 * Destructor.  Just delete this->config_.
 */
Options::~Options ()
{
  delete this->config_;
}


/**
 * @todo Use the ACE_Arg_Shifter class here instead.
 * @todo Implement -l file option.
 *
 * @retval 0  Success parsing args
 * @retval -1 Error parsing args
 * @retval 1  Success but we should exit.
 */
int
Options::parse_args (int argc, ACE_TCHAR *argv[])
{
  int ran_command = 0;
  ACE_TRY_NEW_ENV 
    {
      this->orb_ = CORBA::ORB_init (argc, argv, 0, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "Error: Cannot initialize ORB\n"));
      return -1;
    }
  ACE_ENDTRY;

  ACE_Get_Opt get_opts (argc, argv, "?c:d:hl:o:p:rst:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'c': // Run service command
        if (this->run_service_command (get_opts.optarg) != 0)
          return -1;
        ran_command = 1;
        break;
      case 'd': // Debug flag.
        this->debug_ = ACE_OS::atoi (get_opts.optarg);
        break;
      case 'o': // Output the IOR to a file.
        this->ior_output_file_ = ACE_OS::fopen (get_opts.optarg, "w");
        if (this->ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Error: Unable to open %s for writing: %p\n",
                             get_opts.optarg), -1);
        break;
      case 'p':
        if (this->initialize_file_persistence (get_opts.optarg) != 0)
          return -1;
        break;
      case 'r': // win32 registry implementation
        if (this->initialize_registry_persistence () != 0)
          return -1;
        break;
      case 's': // Run as a service
        this->service_ = 1;
        break;
      case 't': // Set timeout value
        this->startup_timeout_ = 
          ACE_Time_Value (ACE_OS::atoi (get_opts.optarg));
        break;
      case '?': // Display help for use of the server.
      case 'h':
        this->print_usage ();
        return 0;
      default:
        this->print_usage ();
        return -1;
    }

  // If no persistent implementation specified, use a simple heap.
  if (this->config_ == 0)
    if (this->initialize_non_persistence () != 0)
      return -1;

  if (ran_command) 
    return 1;

  // Indicates successful parsing of command line.
  return 0;
}


/**
 * Just print out the usae message to STDERR
 */
void 
Options::print_usage (void) const
{
  ACE_ERROR ((LM_ERROR, 
              "Usage:\n"
              "\n"
              "ImplRepo_Service [-c cmd] [-d lvl] [-l] [-o file] [-r|-p file]"
              " [-r] [-s] [-t secs]\n"
              "\n"
              "  -c command  Runs service commands ('install' or 'remove')\n"
              "  -d level    Sets the debug level\n"
              "  -l file     Log messages to a file\n"
              "  -o file     Outputs the ImR's IOR to a file\n"
              "  -p file     Use file for storing/loading settings\n"
              "  -r          Use the registry for storing/loading settings\n"
              "  -s          Runs as a service (NT Only)\n"
              "  -t secs     Timeout used for killing unresponsive servers\n")
             );
}


/**
 * The most portable form of persistence is file persistence.  Here
 * we assign an ACE_Configuration_Heap object using @param filename
 * as the file.
 *
 * @retval  0 Success
 * @retval -1 Failure
 */
int 
Options::initialize_file_persistence (ACE_TCHAR *filename)
{
  ACE_Configuration_Heap *heap = 0;
  ACE_NEW_RETURN (heap, ACE_Configuration_Heap, -1);
  
  if (heap->open (filename) == 0)
    {
      this->config_ = heap;
      return 0;
    }

  delete heap;
  heap = 0;

  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("Error: Opening persistent heap file '%s'\n"),
              filename));

  return -1;
}


/**
 * On Windows, we have the option of using the Registry to store the
 * server data.  Assigns a ACE_Configuration_Win32Registry to 
 * this->config_.  On non-Win32 systems, just returns an error.
 *
 * @retval  0 Success
 * @retval -1 Failure
 */
int 
Options::initialize_registry_persistence (void)
{
#if defined (ACE_WIN32)
  HKEY root = 
    ACE_Configuration_Win32Registry::resolve_key(HKEY_LOCAL_MACHINE, 
                                                 "Software\\TAO\\IR");
  ACE_NEW_RETURN (this->config_, 
                  ACE_Configuration_Win32Registry(root),
                  -1);
  return 0;
#else /* ACE_WIN32 */
  ACE_ERROR_RETURN ((LM_ERROR, "Registry not supported on this platform"), -1);
#endif /* ACE_WIN32 */
}


/**
 * In cases where persistence isn't needed, create an object of 
 * the ACE_Configuration_Heap class to be used.  Initializes
 * this->config_ to an opened ACE_Configuration_Heap.
 *
 * @retval 0  Success
 * @retval -1 Failure
 */
int 
Options::initialize_non_persistence (void)
{
  ACE_Configuration_Heap *heap = 0;
  ACE_NEW_RETURN (heap, ACE_Configuration_Heap, -1);

  if (heap->open () == 0)
    {
      this->config_ = heap;
      return 0;
    }

  delete heap;
  heap = 0;

  ACE_ERROR ((LM_ERROR, 
             ACE_TEXT ("Error: Opening ACE_Configuration heap\n")));

  return -1;
}


/**
 * Executes the various command that is useful for a NT service.  Right
 * now these include 'install' and 'remove'.  Others, such as 'start' and
 * 'stop' can be added, but the 'net' program in Windows already handles
 * these commands.
 *
 * @todo Finish implementing Options::run_service_command
 * @todo Update to unicode
 */
int 
Options::run_service_command (ACE_TCHAR *command)
{
#if defined (ACE_WIN32)
  SERVICE::instance ()->name (IMR_SERVICE_NAME, IMR_DISPLAY_NAME);

  if (ACE_OS::strcmp (command, ACE_TEXT ("install")) == 0) 
    {
      ACE_TCHAR pathname[_MAX_PATH * 2 + 3];  // +3 for the ' -s' at the end

      if (ACE_TEXT_GetModuleFileName(NULL, pathname, _MAX_PATH * 2) == 0)
        {
          ACE_ERROR ((LM_ERROR, "Error: Could not get module file name.\n"));
          return -1;
        }

      // Append the command used to tell the 
      ACE_OS::strcat (pathname, ACE_TEXT (" -s"));

      return SERVICE::instance ()->insert (SERVICE_AUTO_START,
                                           SERVICE_ERROR_IGNORE,
                                           pathname);
    }
  else if (ACE_OS::strcmp (command, ACE_TEXT ("remove")) == 0) 
    {
      return SERVICE::instance ()->remove ();
    }

  ACE_ERROR ((LM_ERROR, "Error: Unrecognized command: %s\n", command));
  return -1;

#else /* ACE_WIN32 */
  ACE_UNUSED_ARG (command);
  ACE_ERROR ((LM_ERROR, "Service not supported on this platform"));
  
  return -1;
#endif /* ACE_WIN32 */
}


/**
 * Standalone Mode
 *
 * @retval 0 Run as standalone service
 * @retval 1 Run as a service (only on NT right now)
 */
int
Options::service (void) const
{
  return this->service_;
}


/**
 * Debug level for the IR.  
 *
 * @retval 0 Quiet
 * @retval 1 Trace messages
 * @retval 2 Detailed messages
 */
unsigned int
Options::debug (void) const
{
  return this->debug_;
}


/**
 * @return The file where the IOR will be stored.
 */
FILE *
Options::output_file (void) const
{
  return this->ior_output_file_;
}


/**
 * @return The amount of time to wait for a server to start.
 */
const ACE_Time_Value &
Options::startup_timeout (void) const
{
  return this->startup_timeout_;
}


/**
 * @return The file where the IOR will be stored.
 */
const ACE_Time_Value &
Options::ping_interval (void) const
{
  return this->startup_timeout_;
}

/**
 * @return The ACE_Configuration object that is used to store data.
 */
ACE_Configuration *
Options::config (void) const
{
  return this->config_;
}


/**
 * @return A pointer to the ORB.
 */
CORBA::ORB_ptr
Options::orb (void) const
{
  return CORBA::ORB::_duplicate (this->orb_.in ());
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton <Options, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton <Options, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

