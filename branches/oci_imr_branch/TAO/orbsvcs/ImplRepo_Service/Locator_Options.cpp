//=============================================================================
/**
 *  @file    Locator_Options.cpp
 *
 *  $Id$
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#include "Locator_Options.h"
#include "ace/Arg_Shifter.h"
#include "ace/OS_NS_strings.h"
#include "ace/Mutex.h"

ACE_RCSID (ImplRepo_Service,
           Options,
           "$Id$")


#if defined (ACE_WIN32)
static const HKEY SERVICE_REG_ROOT = HKEY_LOCAL_MACHINE;
// This string must agree with the one used in Locator_NT_Service.h
static const ACE_TCHAR *SERVICE_REG_PATH =
  ACE_TEXT ("SYSTEM\\CurrentControlSet\\Services\\TAOIMRLocator\\Parameters");
#endif /* ACE_WIN32 */

/**
 * Default Constructor.  Assigns default values to all the member variables.
 */
Options::Options ()
  : debug_ (1)
  , multicast_ (false)
  , service_ (false)
  , service_command_(SC_NONE)
{
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
                                   ACE_TEXT ("-m")) == 0)
        {
          this->multicast_ = true;
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
                                   ACE_TEXT ("-s")) == 0)
        {
          // Run as a service
          this->service_ = true;
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
              "ImR_Locator [-c cmd] [-d 0|1|2] [-m] [-o file] [-s]\n"
              "\n"
              "  -c command  Runs nt service commands ('install' or 'remove')\n"
              "  -d level    Sets the debug level (default 1)\n"
              "  -m          Turn on multicast\n"
              "  -o file     Outputs the ImR's IOR to a file\n"
              "  -s          Runs as a service (NT Only)\n")
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

  err = ACE_TEXT_RegSetValueEx(key, "IORFile", 0, REG_SZ, 
    (LPBYTE) ior_output_file_.c_str(), ior_output_file_.length() + 1);
  ACE_ASSERT(err == ERROR_SUCCESS);

  err = ACE_TEXT_RegSetValueEx(key, "DebugLevel", 0, REG_DWORD, 
    (LPBYTE) &debug_ , sizeof(debug_));
  ACE_ASSERT(err == ERROR_SUCCESS);

  DWORD tmp = multicast_ ? 1 : 0;
  err = ACE_TEXT_RegSetValueEx(key, "Multicast", 0, REG_DWORD, 
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
  ACE_TCHAR tmpstr[4096];
  DWORD sz = sizeof(tmpstr);
  DWORD type = 0;
  err = ACE_TEXT_RegQueryValueEx(key, "ORBInitOptions", 0, &type, 
    (LPBYTE) tmpstr, &sz);
  if (err == ERROR_SUCCESS) {
    ACE_ASSERT(type == REG_SZ);
    tmpstr[sz - 1] = ACE_TCHAR('\0');
    this->cmdline_ = tmpstr;
  }

  sz = sizeof(tmpstr);
  err = ACE_TEXT_RegQueryValueEx(key, "IORFile", 0, &type, 
    (LPBYTE) tmpstr, &sz);
  if (err == ERROR_SUCCESS) {
    ACE_ASSERT(type == REG_SZ);
    tmpstr[sz - 1] = ACE_TCHAR('\0');
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
  err = ACE_TEXT_RegQueryValueEx(key, "Multicast", 0, &type, 
    (LPBYTE) &tmp, &sz);
  if (err == ERROR_SUCCESS) {
    ACE_ASSERT(type == REG_DWORD);
    this->multicast_ = tmp != 0;
  }

  err = ::RegCloseKey(key);
  ACE_ASSERT(err == ERROR_SUCCESS);
#endif
  return 0;
}

/**
 * Standalone Mode
 *
 * @retval false Run as standalone service
 * @retval true Run as a service (only on NT right now)
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
 * @retval false Do not listen for multicast location requests.
 * @retval true Do Listen.
 */
bool
Options::multicast (void) const
{
  return this->multicast_;
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
