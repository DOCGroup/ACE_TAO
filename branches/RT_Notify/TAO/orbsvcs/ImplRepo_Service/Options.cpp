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
#include "tao/Environment.h"
#include "ace/Arg_Shifter.h"
#include "ace/ARGV.h"
#include "ace/Configuration.h"

#if defined (ACE_WIN32)
const HKEY SERVICE_REG_ROOT = HKEY_LOCAL_MACHINE;
const ACE_TCHAR *SERVICE_REG_PATH =
  ACE_TEXT ("SYSTEM\\CurrentControlSet\\Services\\TAOImplRepo\\Parameters");
const ACE_TCHAR *SERVICE_REG_VALUE_NAME = ACE_TEXT ("ORBOptions");
#endif /* ACE_WIN32 */


/**
 * Default Constructor.  Assigns default values to all the member variables.
 */
Options::Options ()
  : config_ (0)
  , debug_ (1)
  , ior_output_file_ (0)
  , multicast_ (0)
  , ping_interval_ (0, 200)
  , service_ (0)
  , startup_timeout_ (5)
  , readonly_ (0)
{
}


/**
 * Destructor.  Just deletes this->config_.
 */
Options::~Options ()
{
  delete this->config_;
}


/**
 * parse_args uses an ACE_Arg_Shifter to grab all the options that are
 * specific to the ImR.
 *
 * @retval  0 Success
 * @retval -1 Error parsing args
 * @retval  1 Success but we should exit.
 */
int
Options::parse_args (int &argc, ACE_TCHAR *argv[])
{
  ACE_Arg_Shifter shifter (argc, argv);

  while (shifter.is_anything_left ())
    {
      if (ACE_OS::strcasecmp (shifter.get_current (),
                              ACE_TEXT ("-c")) == 0)
        {
          // Run the service command
          shifter.consume_arg ();

          if (!shifter.is_anything_left () || shifter.get_current ()[0] == '-')
            {
              ACE_ERROR ((LM_ERROR, "Error: -c option needs a command\n"));
              return -1;
            }

          if (this->run_service_command (shifter.get_current ()) != 0)
            return -1;

          // Since we just ran a command, we will exit right away.
          return 1;
        }
      else if (ACE_OS::strcasecmp (shifter.get_current (),
                                   ACE_TEXT ("-d")) == 0)
        {
          // Set the debug level
          shifter.consume_arg ();

          if (!shifter.is_anything_left () || shifter.get_current ()[0] == '-')
            {
              ACE_ERROR ((LM_ERROR, "Error: -d option needs a debuglevel\n"));
              return -1;
            }

          this->debug_ = ACE_OS::atoi (shifter.get_current ());
        }
      else if (ACE_OS::strcasecmp (shifter.get_current (),
                                   ACE_TEXT ("-l")) == 0)
        {
          // Lock the database
          this->readonly_ = 1;
        }
      else if (ACE_OS::strcasecmp (shifter.get_current (),
                                   ACE_TEXT ("-m")) == 0)
        {
          // multicast?
          shifter.consume_arg ();

          if (!shifter.is_anything_left () || shifter.get_current ()[0] == '-')
            {
              ACE_ERROR ((LM_ERROR, "Error: -m option requires 1/0\n"));
              return -1;
            }

          this->multicast_ = ACE_OS::atoi (shifter.get_current ());
        }
      else if (ACE_OS::strcasecmp (shifter.get_current (),
                                   ACE_TEXT ("-o")) == 0)
        {
          // Output the IOR to a file.
          shifter.consume_arg ();

          if (!shifter.is_anything_left () || shifter.get_current ()[0] == '-')
            {
              ACE_ERROR ((LM_ERROR, "Error: -o option needs a filename\n"));
              return -1;
            }

          this->ior_output_file_ = ACE_OS::fopen (shifter.get_current (), "w");
          if (this->ior_output_file_ == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Error: Unable to open %s for writing: %p\n",
                               shifter.get_current ()), -1);
        }
      else if (ACE_OS::strcasecmp (shifter.get_current (),
                                   ACE_TEXT ("-p")) == 0)
        {
          // Initialize file persistence
          shifter.consume_arg ();

          if (!shifter.is_anything_left () || shifter.get_current ()[0] == '-')
            {
              ACE_ERROR ((LM_ERROR, "Error: -p option needs a filename\n"));
              return -1;
            }

          if (this->initialize_file_persistence (shifter.get_current ()) != 0)
            return -1;
        }
      else if (ACE_OS::strcasecmp (shifter.get_current (),
                                   ACE_TEXT ("-r")) == 0)
        {
          // win32 registry implementation
          if (this->initialize_registry_persistence () != 0)
            return -1;
        }
      else if (ACE_OS::strcasecmp (shifter.get_current (),
                                   ACE_TEXT ("-s")) == 0)
        {
          // Run as a service
          this->service_ = 1;
        }
      else if (ACE_OS::strcasecmp (shifter.get_current (),
                                   ACE_TEXT ("-t")) == 0)
        {
          // Set timeout value
          shifter.consume_arg ();

          if (!shifter.is_anything_left () || shifter.get_current ()[0] == '-')
            {
              ACE_ERROR ((LM_ERROR, "Error: -t option needs a value\n"));
              return -1;
            }

          this->startup_timeout_ =
            ACE_Time_Value (ACE_OS::atoi (shifter.get_current ()));
        }
      else if ((ACE_OS::strcasecmp (shifter.get_current (),
                                    ACE_TEXT ("-?")) == 0)
               || (ACE_OS::strcasecmp (shifter.get_current (),
                                       ACE_TEXT ("-h")) == 0))
        {
          this->print_usage ();
          return 1;
        }
      else
        {
          shifter.ignore_arg ();
          continue;
        }

      shifter.consume_arg ();
    }

  return 0;
}

/**
 * @retval  0 Success
 * @retval -1 Error parsing args
 * @retval  1 Success but we should exit.
 */
int
Options::init (int argc, ACE_TCHAR *argv[])
{
  ACE_ARGV orb_args;
  int i = 0;
  int result = 0;

  // Make an initial pass through and grab the arguments that we recognize.

  result = this->parse_args (argc, argv);

  if (result != 0)
    return result;

  // Save the leftovers to a ACE_ARGV class

  for (i = 1; i < argc; ++i)
    {
      if (orb_args.add (argv[i]) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error: Could not save argument"));
          return -1;
        }
    }

#if defined (ACE_WIN32)
  // Load any arguments from the registry

  if (this->load_registry_options (orb_args) != 0)
    return -1;
#endif /* ACE_WIN32 */

  // Make another pass to catch any ImR arguments that may be in the
  // registry

  int orb_argc = orb_args.argc ();

  result = this->parse_args (orb_argc, orb_args.argv ());

  if (result != 0)
    return result;

  // Now initialize the orb and pass it the leftover arguments

  ACE_TRY_NEW_ENV
    {

      this->orb_ = CORBA::ORB_init (orb_argc,
                                    orb_args.argv (),
                                    0
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "Error: Cannot initialize ORB\n"));
      return -1;
    }
  ACE_ENDTRY;

  // If there are any arguments left (besides the executable filename)
  // then they were not picked up by us or the orb and must be
  // unrecognized.

  if (orb_argc > 1)
    {
      // Just print out the first option as an error
      ACE_ERROR ((LM_ERROR,
                  "Unrecognized option: %s\n",
                  orb_args.argv ()[1]));
      return -1;
    }

  // If no persistent implementation specified, use a simple heap.
  if (this->config_ == 0)
    if (this->initialize_non_persistence () != 0)
      return -1;

  // Indicates successful parsing of command line.
  return 0;
}


/**
 * Just print out the usage message to STDERR
 */
void
Options::print_usage (void) const
{
  ACE_ERROR ((LM_ERROR,
              "Usage:\n"
              "\n"
              "ImplRepo_Service [-c cmd] [-d lvl] [-l] [-m 0/1] [-o file]"
              " [-r|-p file] [-r] [-s] [-t secs]\n"
              "\n"
              "  -c command  Runs service commands ('install' or 'remove')\n"
              "  -d level    Sets the debug level\n"
              "  -l          Lock the database\n"
              "  -m [0/1]    Turn on(1)/off(0) multicast (default: 1)\n"
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
Options::initialize_file_persistence (const ACE_TCHAR *filename)
{
  if (this->config_ != 0)
  {
    ACE_ERROR ((LM_ERROR,
                "Error: initialize_file_persistence (): "
                "Configuration already defined. \n"
                "Did you pass multiple persistence options?\n"));
    return -1;
  }

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
 * @todo Where in the registry should this be stored?
 *
 * @retval  0 Success
 * @retval -1 Failure
 */
int
Options::initialize_registry_persistence (void)
{
#if defined (ACE_WIN32)
  if (this->config_ != 0)
  {
    ACE_ERROR ((LM_ERROR,
                "Error: initialize_registry_persistence (): "
                "Configuration already defined. \n"
                "Did you pass multiple persistence options?\n"));
    return -1;
  }

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
 * Executes the various commands that are useful for a NT service.  Right
 * now these include 'install' and 'remove'.  Others, such as 'start' and
 * 'stop' can be added, but the 'net' program in Windows already handles
 * these commands.
 *
 * @todo Finish implementing Options::run_service_command
 * @todo Update to unicode
 */
int
Options::run_service_command (const ACE_TCHAR *command)
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

      // Append the command used for running the implrepo as
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
 *  We will only load from the registry if we are a service.
 *  The location we store options in is HKEY_LOCAL_MACHINE:
 *  SYSTEM\CurrentControlSet\Services\TAOImplRepo\Parameters
 *
 *  The only key currently supported is ORBOptions
 *
 *  @todo Is there a better way to handle the hKey? (as in a auto_ptr)
 */
int
Options::load_registry_options (ACE_ARGV &orb_options)
{
#if defined (ACE_WIN32)
  // Only if we are running as a service
  if (!this->service ())
  {
    if (this->debug () > 1)
      ACE_DEBUG ((LM_DEBUG,
                  "Not running as a service, will not load data from "
                  "registry\n"));
    return 0;
  }

  HKEY hKey = 0;
  BYTE buffer[ACE_DEFAULT_ARGV_BUFSIZ] = {0};

  if (ACE_TEXT_RegOpenKeyEx (SERVICE_REG_ROOT,
                             SERVICE_REG_PATH,
                             0,
                             KEY_READ,
                             &hKey) != ERROR_SUCCESS)
    {
      if (this->debug () > 1)
        {
          ACE_DEBUG ((LM_DEBUG, "Could not open Registry Key, skipping\n"));
        }

      return 0;
    }

  DWORD dwType = 0;
  DWORD dwSize = sizeof (buffer);

  if (ACE_TEXT_RegQueryValueEx (hKey,
                                SERVICE_REG_VALUE_NAME,
                                NULL,
                                &dwType,
                                buffer,
                                &dwSize) != ERROR_SUCCESS)
    {
      if (this->debug () > 1)
        {
          ACE_DEBUG ((LM_DEBUG, "Could not read Registry Key, skipping\n"));
          ::RegCloseKey (hKey);
          return 0;
        }
    }

  if (dwType != REG_MULTI_SZ)
    {
      ACE_ERROR ((LM_ERROR, "Error: ORB Options registry key not MULTI_SZ\n"));
     ::RegCloseKey (hKey);
      return -1;
    }

  // Skip the case where there are no arguments
  if (dwSize > 1)
    {
      // Create an argv array
      ACE_TCHAR **orb_argv = 0;

      ACE_NEW_RETURN (orb_argv, ACE_TCHAR *[dwSize], -1);

      ACE_TCHAR *tchar_buffer = ACE_reinterpret_cast (ACE_TCHAR *, buffer);
      orb_argv[0] = tchar_buffer;

      if (this->debug () > 1)
        ACE_DEBUG ((LM_DEBUG,
                    "Registry Argument Added: %s\n",
                    orb_argv[0]));

      int orb_argv_pos = 1;

      for (unsigned int buffer_pos = 0;
           buffer_pos < dwSize / sizeof (ACE_TCHAR) - 2;
           ++buffer_pos)
        {
          if (tchar_buffer[buffer_pos] == 0)
            {
              if (this->debug () > 1)
                ACE_DEBUG ((LM_DEBUG,
                            "Registry Argument Added: %s\n",
                            &tchar_buffer[buffer_pos] + 1));

              orb_argv[orb_argv_pos++] = &tchar_buffer[buffer_pos] + 1;
            }
        }

      orb_argv[orb_argv_pos] = 0;

      orb_options.add (orb_argv);

      delete [] orb_argv;
    }

  if (::RegCloseKey (hKey) != ERROR_SUCCESS)
    {
      ACE_ERROR ((LM_ERROR, "Error: Could close Registry Key\n"));
      return -1;
    }

  return 0;
#else /* ACE_WIN32 */
  ACE_UNUSED_ARG (orb_options);
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
 * @return The amount of time to wait between pings
 */
const ACE_Time_Value &
Options::ping_interval (void) const
{
  return this->ping_interval_;
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


/**
 * @retval 0 Do not listen for multicast location requests.
 * @retval 1 Do Listen.
 */
int
Options::multicast (void) const
{
  return this->multicast_;
}

/**
 * @retval 0 Normal operation.
 * @retval 1 Do not let server info be modified.
 */
int
Options::readonly (void) const
{
  return this->readonly_;
}
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton <Options, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton <Options, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
