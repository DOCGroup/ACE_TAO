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

#include "ace/Arg_Shifter.h"

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
          else if (ACE_OS::strcasecmp (shifter.get_current (),
                                   ACE_TEXT ("install_no_locator")) == 0)
          {
            this->service_command_ = SC_INSTALL_NO_LOCATOR;
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

          this->persist_file_name_ = shifter.get_current ();
          this->repo_mode_ = REPO_HEAP_FILE;
        }
      else if (ACE_OS::strcasecmp (shifter.get_current (),
                                   ACE_TEXT ("-r")) == 0)
        {
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

          this->persist_file_name_ = shifter.get_current ();
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
 * Should always return 0
 */
int
Options::init (int argc, char *argv[])
{
  // Make an initial pass through and grab the arguments that we recognize.
  // This may also run the commands to install or remove the nt service.
  int result = this->parse_args (argc, argv);
  if (result != 0) 
  {
    return result;
  }

  for (int i = 1; i < argc; ++i)
  {
    this->cmdline_ += ACE_CString(argv[i]) + ACE_CString(" ");
  }

  return 0;
}

int
Options::init_from_registry (void)
{
  this->load_registry_options();
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
              "  -c command  Runs service commands \n"
              "              ('install' or 'remove' or 'install_no_locator')\n"
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
Options::save_registry_options() 
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
    (LPBYTE) this->cmdline_.c_str(), this->cmdline_.length() + 1);
  ACE_ASSERT(err == ERROR_SUCCESS);

  err = ACE_TEXT_RegSetValueEx(key, "PersistFile", 0, REG_SZ, 
   (LPBYTE) this->persist_file_name_.c_str(), this->persist_file_name_.length() + 1);
  ACE_ASSERT(err == ERROR_SUCCESS);

  err = ACE_TEXT_RegSetValueEx(key, "IORFile", 0, REG_SZ, 
    (LPBYTE) this->ior_output_file_.c_str(), this->ior_output_file_.length() + 1);
  ACE_ASSERT(err == ERROR_SUCCESS);

  err = ACE_TEXT_RegSetValueEx(key, "DebugLevel", 0, REG_DWORD, 
    (LPBYTE) &this->debug_ , sizeof(this->debug_));
  ACE_ASSERT(err == ERROR_SUCCESS);

  DWORD tmp = this->ping_interval_.msec();
  err = ACE_TEXT_RegSetValueEx(key, "PingInterval", 0, REG_DWORD, 
    (LPBYTE) &tmp, sizeof(DWORD));
  ACE_ASSERT(err == ERROR_SUCCESS);

  tmp = this->readonly_ ? 1 : 0;
  err = ACE_TEXT_RegSetValueEx(key, "Lock", 0, REG_DWORD, 
    (LPBYTE) &tmp, sizeof(DWORD));
  ACE_ASSERT(err == ERROR_SUCCESS);

  tmp = this->repo_mode_;
  err = ACE_TEXT_RegSetValueEx(key, "PersistType", 0, REG_DWORD, 
    (LPBYTE) &tmp, sizeof(DWORD));
  ACE_ASSERT(err == ERROR_SUCCESS);

  tmp = this->startup_timeout_.sec();
  err = ACE_TEXT_RegSetValueEx(key, "Timeout", 0, REG_DWORD, 
    (LPBYTE) &tmp, sizeof(DWORD));
  ACE_ASSERT(err == ERROR_SUCCESS);

  err = ::RegCloseKey(key);
  ACE_ASSERT(err == ERROR_SUCCESS);
#endif
  return 0;
}

int
Options::load_registry_options ()
{
#if defined (ACE_WIN32)
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
  char tmpstr[4096];
  DWORD sz = sizeof(tmpstr);
  DWORD type = 0;
  err = ACE_TEXT_RegQueryValueEx(key, "ORBInitOptions", 0, &type, 
    (LPBYTE) tmpstr, &sz);
  if (err == ERROR_SUCCESS) {
    ACE_ASSERT(type == REG_SZ);
    tmpstr[sz - 1] = '\0';
    this->cmdline_ = tmpstr;
  }

  sz = sizeof(tmpstr);
  err = ACE_TEXT_RegQueryValueEx(key, "PersistFile", 0, &type, 
    (LPBYTE) tmpstr, &sz);
  if (err == ERROR_SUCCESS) {
    ACE_ASSERT(type == REG_SZ);
    tmpstr[sz - 1] = '\0';
    this->persist_file_name_ = ACE_CString(tmpstr);
  }

  sz = sizeof(tmpstr);
  err = ACE_TEXT_RegQueryValueEx(key, "IORFile", 0, &type, 
    (LPBYTE) tmpstr, &sz);
  if (err == ERROR_SUCCESS) {
    ACE_ASSERT(type == REG_SZ);
    tmpstr[sz - 1] = '\0';
    this->ior_output_file_ = tmpstr;
  }

  sz = sizeof(debug_);
  err = ACE_TEXT_RegQueryValueEx(key, "DebugLevel", 0, &type, 
    (LPBYTE) &this->debug_ , &sz);
  if (err == ERROR_SUCCESS) {
    ACE_ASSERT(type == REG_DWORD);
  }

  DWORD tmp = 0;
  sz = sizeof(tmp);
  err = ACE_TEXT_RegQueryValueEx(key, "PingInterval", 0, &type, 
    (LPBYTE) &tmp, &sz);
  if (err == ERROR_SUCCESS) {
    ACE_ASSERT(type == REG_DWORD);
    ping_interval_.msec(tmp);
  }

  tmp = 0;
  sz = sizeof(tmp);
  err = ACE_TEXT_RegQueryValueEx(key, "Lock", 0, &type, 
    (LPBYTE) &tmp, &sz);
  if (err == ERROR_SUCCESS) {
    ACE_ASSERT(type == REG_DWORD);
    readonly_ = tmp != 0;
  }

  sz = sizeof(this->repo_mode_);
  err = ACE_TEXT_RegQueryValueEx(key, "PersistType", 0, &type, 
    (LPBYTE) &this->repo_mode_, &sz);
  if (err == ERROR_SUCCESS) {
    ACE_ASSERT(type == REG_DWORD);
  }

  tmp = 0;
  sz = sizeof(tmp);
  err = ACE_TEXT_RegQueryValueEx(key, "Timeout", 0, &type, 
    (LPBYTE) &tmp, &sz);
  if (err == ERROR_SUCCESS) {
    ACE_ASSERT(type == REG_DWORD);
    this->startup_timeout_.sec(tmp);
  }

  err = ::RegCloseKey(key);
  ACE_ASSERT(err == ERROR_SUCCESS);
#endif /* ACE_WIN32 */
  return 0;
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
const ACE_CString&
Options::ior_filename (void) const
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

Options::RepoMode
Options::repository_mode (void) const
{
  return this->repo_mode_;
}

const ACE_CString&
Options::file_name (void) const
{
  return this->persist_file_name_;
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

Options::SERVICE_COMMAND
Options::service_command(void) const
{
  return this->service_command_;
}

const ACE_CString&
Options::cmdline(void) const {
  return this->cmdline_;
}

