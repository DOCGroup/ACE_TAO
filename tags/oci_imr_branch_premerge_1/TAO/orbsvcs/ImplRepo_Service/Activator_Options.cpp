//=============================================================================
/**
 *  @file    Activator_Options.cpp
 *
 *  $Id$
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================
#include "Activator_Options.h"
#include "Activator_NT_Service.h"
#include "tao/Strategies/advanced_resource.h"
#include "ace/Arg_Shifter.h"
#include "ace/ARGV.h"
#include "ace/OS_NS_strings.h"
#include "ace/Mutex.h"

ACE_RCSID (ImplRepo_Service,
           Options,
           "$Id$")


#if defined (ACE_WIN32)
static const HKEY SERVICE_REG_ROOT = HKEY_LOCAL_MACHINE;
// This string must agree with the one used in Activator_NT_Service.h
static const ACE_TCHAR *SERVICE_REG_PATH =
  ACE_TEXT ("SYSTEM\\CurrentControlSet\\Services\\TAOIMRActivator\\Parameters");
#endif /* ACE_WIN32 */

/**
 * Default Constructor.  Assigns default values to all the member variables.
 */
Options::Options ()
  : repo_mode_ (REPO_NONE)
  , debug_ (1)
  , ping_interval_ (0, 200 * 1000) // 200 milliseconds
  , service_ (false)
  , startup_timeout_ (5)
  , readonly_ (false)
  , service_command_(SC_NONE)
{
}

/**
 * parse_args uses an ACE_Arg_Shifter to grab all the options that are
 * specific to the ImR.
 * If running as an nt service, most of these options will come from the
 * registry instead.
 *
 * @retval  0 Success
 * @retval -1 Error parsing args
 * @retval  1 Success but we should exit.
 */
int
Options::parse_args (int &argc, char *argv[])
{
  ACE_Arg_Shifter shifter (argc, argv);
  
  while (shifter.is_anything_left ())
    {
      if (ACE_OS::strcasecmp (shifter.get_current (),
                              ACE_TEXT ("-c")) == 0)
        {
          shifter.consume_arg ();

          if (!shifter.is_anything_left () || shifter.get_current ()[0] == '-')
            {
              ACE_ERROR ((LM_ERROR, "Error: -c option needs a command\n"));
              this->print_usage ();
              return -1;
            }

          if (ACE_OS::strcasecmp (shifter.get_current (),
                                   ACE_TEXT ("install")) == 0) 
          {
            this->service_command_ = SC_INSTALL;
          }
          else if (ACE_OS::strcasecmp (shifter.get_current (),
                                   ACE_TEXT ("remove")) == 0) 
          {
            this->service_command_ = SC_REMOVE;
          }
          else
          {
            ACE_ERROR((LM_ERROR, "Error: Unknown service command : %s\n", shifter.get_current()));
            this->print_usage ();
            return -1;
          } 
        }
      else if (ACE_OS::strcasecmp (shifter.get_current (),
                                   ACE_TEXT ("-d")) == 0)
        {
          shifter.consume_arg ();

          if (!shifter.is_anything_left () || shifter.get_current ()[0] == '-')
            {
              ACE_ERROR ((LM_ERROR, "Error: -d option needs a debuglevel\n"));
              this->print_usage ();
              return -1;
            }

          this->debug_ = ACE_OS::atoi (shifter.get_current ());
        }
      else if (ACE_OS::strcasecmp (shifter.get_current (),
                                   ACE_TEXT ("-l")) == 0)
        {
          this->readonly_ = true;
        }
      else if (ACE_OS::strcasecmp (shifter.get_current (),
                                   ACE_TEXT ("-o")) == 0)
        {
          shifter.consume_arg ();

          if (!shifter.is_anything_left () || shifter.get_current ()[0] == '-')
            {
              ACE_ERROR ((LM_ERROR, "Error: -o option needs a filename\n"));
              this->print_usage ();
              return -1;
            }
          this->ior_output_file_ = shifter.get_current();
        }
      else if (ACE_OS::strcasecmp (shifter.get_current (),
                                   ACE_TEXT ("-p")) == 0)
        {
          shifter.consume_arg ();

          if (!shifter.is_anything_left () || shifter.get_current ()[0] == '-')
            {
              ACE_ERROR ((LM_ERROR, "Error: -p option needs a filename\n"));
              this->print_usage ();
              return -1;
            }

          if (repo_mode_ != REPO_NONE) 
          {
            ACE_ERROR ((LM_ERROR, "Error: Persistence already specified.\n"));
            this->print_usage ();
            return -1;
          }

          this->file_name_ = shifter.get_current ();
          this->repo_mode_ = REPO_HEAP_FILE;
        }
      else if (ACE_OS::strcasecmp (shifter.get_current (),
                                   ACE_TEXT ("-r")) == 0)
        {
          if (repo_mode_ != REPO_NONE) 
          {
            ACE_ERROR ((LM_ERROR, "Error: Persistence already specified.\n"));
            this->print_usage ();
            return -1;
          }
          this->repo_mode_ = REPO_REGISTRY;
        }
      else if (ACE_OS::strcasecmp (shifter.get_current (),
                                   ACE_TEXT ("-x")) == 0)
        {
          shifter.consume_arg ();

          if (!shifter.is_anything_left () || shifter.get_current ()[0] == '-')
            {
              ACE_ERROR ((LM_ERROR, "Error: -x option needs a filename\n"));
              this->print_usage ();
              return -1;
            }

          if (repo_mode_ != REPO_NONE) 
          {
            ACE_ERROR ((LM_ERROR, "Error: Persistence already specified.\n"));
            this->print_usage ();
            return -1;
          }

          this->file_name_ = shifter.get_current ();
          this->repo_mode_ = REPO_XML_FILE;
        }
      else if (ACE_OS::strcasecmp (shifter.get_current (),
                                   ACE_TEXT ("-s")) == 0)
        {
          this->service_ = true;
        }
      else if (ACE_OS::strcasecmp (shifter.get_current (),
                                   ACE_TEXT ("-t")) == 0)
        {
          shifter.consume_arg ();

          if (!shifter.is_anything_left () || shifter.get_current ()[0] == '-')
            {
              ACE_ERROR ((LM_ERROR, "Error: -t option needs a value\n"));
              this->print_usage ();
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
Options::init (int argc, char *argv[])
{
  // Make an initial pass through and grab the arguments that we recognize.
  // This may also run the commands to install or remove the nt service.
  int result = this->parse_args (argc, argv);
  if (result != 0) {
    return result;
  }

  ACE_ARGV orb_args;   // Save the leftovers to a ACE_ARGV class
  ACE_CString cmdline; // We'll save this in the registry when installing.
  for (int i = 1; i < argc; ++i)
    {
      cmdline += ACE_CString(argv[i]) + ACE_CString(" ");
      if (orb_args.add (argv[i]) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error: Could not save argument"));
          return -1;
        }
    }

  result = run_service_command(cmdline);
  
  if (result != 0) 
    return result;

  char* argv_tmp = 0;

  // Load from the registry. This may replace the args.
  if (this->load_registry_options(argv_tmp, orb_args) != 0)
    return -1;

  ACE_Auto_Array_Ptr<char> argv_deleter(argv_tmp);

  if (orb_args.add ("-ORBSvcConfDirective\"static Advanced_Resource_Factory '-ORBReactorType select_st'\"") == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t) TAO_ImR_Activator- Could not add"
                         " SvcConfDirective \n"),
                        -1);
    }

  int orb_argc = orb_args.argc ();

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
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception \n");
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

  if (this->initialize_persistence () != 0)
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
              "ImR_Activator [-c cmd] [-d 0|1|2] [-l] [-m] [-o file]"
              " [-r|-p file|-x file] [-s] [-t secs]\n"
              "\n"
              "  -c command  Runs service commands ('install' or 'remove')\n"
              "  -d level    Sets the debug level\n"
              "  -l          Lock the database\n"
              "  -o file     Outputs the ImR's IOR to a file\n"
              "  -p file     Use file for storing/loading settings\n"
              "  -x file     Use XML file for storing/loading setting\n"
              "  -r          Use the registry for storing/loading settings\n"
              "  -s          Runs as a service (NT Only)\n"
              "  -t secs     Timeout used for killing unresponsive servers\n")
             );
}

int
Options::initialize_persistence(void)
{
  switch (this->repo_mode_) 
  {
  case REPO_NONE:
    return this->initialize_non_persistence();
  case REPO_XML_FILE:
    return this->initialize_xml_persistence();
  case REPO_HEAP_FILE:
    return this->initialize_heap_persistence();
  case REPO_REGISTRY:
    return this->initialize_registry_persistence();
  }
  ACE_ERROR((LM_ERROR, "Error: Unknown persistence type.\n"));
  return -1;
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
Options::initialize_heap_persistence (void)
{
  ACE_ASSERT(this->repo_config_.get() == 0);

  auto_ptr<Repository_Configuration> rc(new Repository_Configuration ("h"));

  if (rc->open (this->file_name_.c_str()) == 0)
    {
      this->repo_config_ = rc;
      return 0;
    }
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("Error: Opening persistent heap file '%s'\n"),
              this->file_name_.c_str()));
  return -1;
}


/**
 * On Windows, we have the option of using the Registry to store the
 * server data.  Assigns a ACE_Configuration_Win32Registry to
 * this->repo_config_.  On non-Win32 systems, just returns an error.
 *
 * @todo Where in the registry should this be stored?
 *
 * @retval  0 Success
 * @retval -1 Failure
 */
int
Options::initialize_registry_persistence (void)
{
  ACE_ASSERT(this->repo_config_.get() == 0);
#if defined (ACE_WIN32)
  auto_ptr<Repository_Configuration> rc(new Repository_Configuration ("w"));
  this->repo_config_ = rc;
  return 0;
#else /* ACE_WIN32 */
  ACE_ERROR_RETURN ((LM_ERROR, "Registry not supported on this platform"), -1);
#endif /* ACE_WIN32 */
}


/**
 * In cases where persistence isn't needed, create an object of
 * the ACE_Configuration_Heap class to be used.  Initializes
 * this->repo_config_ to an opened ACE_Configuration_Heap.
 *
 * @retval 0  Success
 * @retval -1 Failure
 */
int
Options::initialize_non_persistence (void)
{
  ACE_ASSERT(this->repo_config_.get() == 0);

  auto_ptr<Repository_Configuration> rc(new Repository_Configuration ("h"));

  if (rc->open () == 0)
    {
      this->repo_config_ = rc;
      return 0;
    }
  ACE_ERROR ((LM_ERROR,
             ACE_TEXT ("Error: Opening Configuration heap\n")));
  return -1;
}

/// Initialize for the XML repository case.
int
Options::initialize_xml_persistence (void)
{
  ACE_ASSERT(this->repo_config_.get() == 0);

  auto_ptr<Repository_Configuration> rc(new Repository_Configuration ("x"));

  if (rc->open (this->file_name_.c_str()) == 0)
    {
      this->repo_config_ = rc;
      return 0;
    }
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("Error: Opening Configuration XML file %s\n"),
              this->file_name_.c_str()
              ));
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
Options::run_service_command (const ACE_CString& cmdline)
{
  if (this->service_command_ == SC_NONE) 
    return 0;
#if defined (ACE_WIN32)
  SERVICE::instance ()->name (IMR_ACTIVATOR_SERVICE_NAME, IMR_ACTIVATOR_DISPLAY_NAME);

  if (this->service_command_ == SC_INSTALL)
    {
      char pathname[_MAX_PATH * 2 + 3];  // +3 for the ' -s' at the end

      if (ACE_TEXT_GetModuleFileName(NULL, pathname, _MAX_PATH * 2) == 0)
        {
          ACE_ERROR ((LM_ERROR, "Error: Could not get module file name.\n"));
          return -1;
        }

      // Append the command used for running the implrepo as
      ACE_OS::strcat (pathname, ACE_TEXT (" -s"));

      const char* DEPENDS_ON = "TAOIMRLocator"; // Must match Locator_NT_Service.h

      int ret =  SERVICE::instance ()->insert (SERVICE_DEMAND_START,
                                           SERVICE_ERROR_NORMAL,
                                           pathname,
                                           0, // group
                                           0, // tag
                                           DEPENDS_ON
                                           );
      if (ret != -1) {
        if (debug() >= 1) {
          ACE_DEBUG ((LM_DEBUG, "ImR Activator: Service installed.\n"));
        }
        this->save_registry_options(cmdline);
      } else {
        ACE_ERROR((LM_ERROR, "Error: Failed to install service.\n"));
      }
      if (ret == 0) 
        return 1;
    }
  else if (this->service_command_ == SC_REMOVE)
    {
      int ret = SERVICE::instance ()->remove ();
      if (debug() >= 1) {
        ACE_DEBUG ((LM_DEBUG, "ImR Activator: Service removed.\n"));
      }
      if (ret == 0) 
        return 1; // If successfull, then we don't want to continue.
    }

  return -1;

#else /* ACE_WIN32 */
  ACE_UNUSED_ARG (cmdline);
  ACE_ERROR ((LM_ERROR, "Service not supported on this platform"));

  return -1;
#endif /* ACE_WIN32 */
}

int 
Options::save_registry_options(const ACE_CString& cmdline) 
{
#if defined (ACE_WIN32)
  HKEY key = 0;
  // Create or open the parameters key
  LONG err = ACE_TEXT_RegCreateKeyEx (SERVICE_REG_ROOT,
                             SERVICE_REG_PATH,
                             0,
                             "", // class
                             REG_OPTION_NON_VOLATILE,
                             KEY_ALL_ACCESS,
                             NULL,
                             &key,
                             NULL
                             );
  if (err != ERROR_SUCCESS) {
    return -1;
  }
  err = ACE_TEXT_RegSetValueEx(key, "ORBInitOptions", 0, REG_SZ, 
    (LPBYTE) cmdline.c_str(), cmdline.length() + 1);
  ACE_ASSERT(err == ERROR_SUCCESS);

  err = ACE_TEXT_RegSetValueEx(key, "PersistFile", 0, REG_SZ, 
   (LPBYTE) file_name_.c_str(), file_name_.length() + 1);
  ACE_ASSERT(err == ERROR_SUCCESS);

  err = ACE_TEXT_RegSetValueEx(key, "IORFile", 0, REG_SZ, 
    (LPBYTE) ior_output_file_.c_str(), ior_output_file_.length() + 1);
  ACE_ASSERT(err == ERROR_SUCCESS);

  err = ACE_TEXT_RegSetValueEx(key, "DebugLevel", 0, REG_DWORD, 
    (LPBYTE) &debug_ , sizeof(debug_));
  ACE_ASSERT(err == ERROR_SUCCESS);

  DWORD tmp = ping_interval_.msec();
  err = ACE_TEXT_RegSetValueEx(key, "PingInterval", 0, REG_DWORD, 
    (LPBYTE) &tmp, sizeof(DWORD));
  ACE_ASSERT(err == ERROR_SUCCESS);

  tmp = readonly_ ? 1 : 0;
  err = ACE_TEXT_RegSetValueEx(key, "Lock", 0, REG_DWORD, 
    (LPBYTE) &tmp, sizeof(DWORD));
  ACE_ASSERT(err == ERROR_SUCCESS);

  tmp = repo_mode_;
  err = ACE_TEXT_RegSetValueEx(key, "PersistType", 0, REG_DWORD, 
    (LPBYTE) &tmp, sizeof(DWORD));
  ACE_ASSERT(err == ERROR_SUCCESS);

  tmp = startup_timeout_.sec();
  err = ACE_TEXT_RegSetValueEx(key, "Timeout", 0, REG_DWORD, 
    (LPBYTE) &tmp, sizeof(DWORD));
  ACE_ASSERT(err == ERROR_SUCCESS);

  err = ::RegCloseKey(key);
  ACE_ASSERT(err == ERROR_SUCCESS);
#else 
  ACE_UNUSED_ARG (cmdline);
#endif
  return 0;
}

namespace {
  // This both parses the cmdline by replacing spaces with \0's, and
  // adds each command to the ACE_ARGV. 
  void parse_command_line(char* cmdline, ACE_ARGV& argv) {
    // This tokenizer will replace all spaces with end-of-string
    // characters and will preserve text between "" and '' pairs.
    ACE_Tokenizer parser (cmdline);
    parser.delimiter_replace (' ', '\0');
    parser.preserve_designators ('\"', '\"');
    parser.preserve_designators ('\'', '\'');

    for (char *p = parser.next (); p; p = parser.next ()) {
      argv.add(p);
    }
  }
}
/**
 *  We will only load from the registry if we are a service.
 *  We load each parameter from individual string keys, and then
 *  we have to parse the cmdline property into the orb_options
 *  so that they can be passed to ORB_init()
 */
int
Options::load_registry_options (char*& cmdline, ACE_ARGV& argv)
{
#if defined (ACE_WIN32)
  if (! this->service())
  {
    if (this->debug () >= 2)
      ACE_DEBUG ((LM_DEBUG,
      "ActivatorOptions::load_registry_options: Not running as "
      "a service, will not load data from registry\n"));
    return 0;
  }

  HKEY key = 0;
  // Create or open the parameters key
  LONG err = ACE_TEXT_RegOpenKeyEx (SERVICE_REG_ROOT,
                             SERVICE_REG_PATH,
                             0,
                             KEY_READ,
                             &key
                             );
  if (err != ERROR_SUCCESS) {
    // If there aren't any saved parameters, then that's ok.
    return 0;
  }
  ACE_TCHAR tmpstr[4096];
  DWORD sz = sizeof(tmpstr);
  DWORD type = 0;
  err = ACE_TEXT_RegQueryValueEx(key, "ORBInitOptions", 0, &type, 
    (LPBYTE) tmpstr, &sz);
  if (err == ERROR_SUCCESS) {
    ACE_ASSERT(type == REG_SZ);
    tmpstr[sz] = ACE_LIB_TEXT('\0');
    cmdline = new char[sz+1];
    ACE_OS::strcpy(cmdline, tmpstr);
    parse_command_line(cmdline, argv);
  }

  sz = sizeof(tmpstr);
  err = ACE_TEXT_RegQueryValueEx(key, "PersistFile", 0, &type, 
    (LPBYTE) tmpstr, &sz);
  if (err == ERROR_SUCCESS) {
    ACE_ASSERT(type == REG_SZ);
    tmpstr[sz] = ACE_LIB_TEXT('\0');
    this->file_name_ = ACE_CString(tmpstr);
  }

  sz = sizeof(tmpstr);
  err = ACE_TEXT_RegQueryValueEx(key, "IORFile", 0, &type, 
    (LPBYTE) tmpstr, &sz);
  if (err == ERROR_SUCCESS) {
    ACE_ASSERT(type == REG_SZ);
    tmpstr[sz] = ACE_LIB_TEXT('\0');
    this->ior_output_file_ = ACE_CString(tmpstr);
  }

  sz = sizeof(debug_);
  err = ACE_TEXT_RegQueryValueEx(key, "DebugLevel", 0, &type, 
    (LPBYTE) &debug_ , &sz);
  if (err == ERROR_SUCCESS) {
    ACE_ASSERT(type == REG_DWORD);
  }

  DWORD tmp = 0;
  err = ACE_TEXT_RegQueryValueEx(key, "PingInterval", 0, &type, 
    (LPBYTE) &tmp, &sz);
  if (err == ERROR_SUCCESS) {
    ACE_ASSERT(type == REG_DWORD);
    ping_interval_.msec(tmp);
  }

  tmp = 0;
  err = ACE_TEXT_RegQueryValueEx(key, "Lock", 0, &type, 
    (LPBYTE) &tmp, &sz);
  if (err == ERROR_SUCCESS) {
    ACE_ASSERT(type == REG_DWORD);
    readonly_ = tmp != 0;
  }

  err = ACE_TEXT_RegQueryValueEx(key, "PersistType", 0, &type, 
    (LPBYTE) &repo_mode_, &sz);
  if (err == ERROR_SUCCESS) {
    ACE_ASSERT(type == REG_DWORD);
  }

  tmp = 0;
  err = ACE_TEXT_RegQueryValueEx(key, "Timeout", 0, &type, 
    (LPBYTE) &tmp, &sz);
  if (err == ERROR_SUCCESS) {
    ACE_ASSERT(type == REG_DWORD);
    startup_timeout_.sec(tmp);
  }

  err = ::RegCloseKey(key);
  ACE_ASSERT(err == ERROR_SUCCESS);

  return 0;
#else /* ACE_WIN32 */
  ACE_UNUSED_ARG (cmdline);
  ACE_UNUSED_ARG (argv);
  return 0;
#endif /* ACE_WIN32 */
}

/**
 * Standalone Mode
 *
 * @retval 0 Run as standalone service
 * @retval 1 Run as a service (only on NT right now)
 */
bool
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
ACE_CString
Options::output_filename (void) const
{
  return this->ior_output_file_;
}


/**
 * @return The amount of time to wait for a server to start.
 */
ACE_Time_Value
Options::startup_timeout (void) const
{
  return this->startup_timeout_;
}


/**
 * @return The amount of time to wait between pings
 */
ACE_Time_Value
Options::ping_interval (void) const
{
  return this->ping_interval_;
}

/**
 * @return The ACE_Configuration object that is used to store data.
 */
Repository_Configuration *
Options::config (void) const
{
  return this->repo_config_.get();
}

int
Options::repository_mode (void)
{
  return this->repo_mode_;
}

ACE_CString
Options::file_name (void) const
{
  return this->file_name_;
}

const char *
Options::convert_str (ImplementationRepository::ActivationMode mode)
{
  switch (mode)
    {
    case ImplementationRepository::NORMAL:
      return "NORMAL";
    case ImplementationRepository::MANUAL:
      return "MANUAL";
    case ImplementationRepository::PER_CLIENT:
      return "PER_CLIENT";
    case ImplementationRepository::AUTO_START:
      return "AUTO_START";
    default:
      return "UNKNOWN";
    };
}

ImplementationRepository::ActivationMode
Options::convert_mode (const char * mode)
{
  if (ACE_OS::strcmp (mode, "NORMAL") == 0)
    return ImplementationRepository::NORMAL;
  else if (ACE_OS::strcmp (mode, "MANUAL") == 0)
    return ImplementationRepository::MANUAL;
  else if (ACE_OS::strcmp (mode, "PER_CLIENT") == 0)
    return ImplementationRepository::PER_CLIENT;
  else //if (ACE_OS::strcmp (mode, "AUTO_START") == 0)
    return ImplementationRepository::AUTO_START;
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
 * @retval 0 Normal operation.
 * @retval 1 Do not let server info be modified.
 */
bool
Options::readonly (void) const
{
  return this->readonly_;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton <Options, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton <Options, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
