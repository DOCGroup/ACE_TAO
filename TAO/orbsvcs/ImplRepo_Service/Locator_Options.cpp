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
#include "Locator_NT_Service.h"
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
 * @retval  1 Success but we should exit.
 */
int
Options::init (int argc, char *argv[])
{
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
              "ImR_Locator [-c cmd] [-d 0|1|2] [-m] [-o file] [-s]\n"
              "\n"
              "  -c command  Runs nt service commands ('install' or 'remove')\n"
              "  -d level    Sets the debug level (default 1)\n"
              "  -m          Turn on multicast\n"
              "  -o file     Outputs the ImR's IOR to a file\n"
              "  -s          Runs as a service (NT Only)\n")
             );
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
  SERVICE::instance ()->name (IMR_LOCATOR_SERVICE_NAME, IMR_LOCATOR_DISPLAY_NAME);

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

      int ret =  SERVICE::instance ()->insert (SERVICE_DEMAND_START,
                                           SERVICE_ERROR_NORMAL,
                                           pathname
                                           );
      if (ret != -1) {
        if (debug() > 0) {
          ACE_DEBUG ((LM_DEBUG, "ImR Locator: Service installed.\n"));
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
      if (debug() > 0) {
        ACE_DEBUG ((LM_DEBUG, "ImR Locator: Service removed.\n"));
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
    if (this->debug () > 1)
      ACE_DEBUG ((LM_DEBUG,
      "Locator_Options::load_registry_options: Not running "
      "as a service, will not load data from registry\n"));
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
    cmdline = new char[sz+1];
    ACE_OS::strncpy(cmdline, tmpstr, sz);
    parse_command_line(cmdline, argv);
  }

  sz = sizeof(tmpstr);
  err = ACE_TEXT_RegQueryValueEx(key, "IORFile", 0, &type, 
    (LPBYTE) tmpstr, &sz);
  if (err == ERROR_SUCCESS) {
    ACE_ASSERT(type == REG_SZ);
    tmpstr[sz] = ACE_TCHAR('\0');
    this->ior_output_file_ = ACE_CString(tmpstr);
  }

  sz = sizeof(debug_);
  err = ACE_TEXT_RegQueryValueEx(key, "DebugLevel", 0, &type, 
    (LPBYTE) &debug_ , &sz);
  if (err == ERROR_SUCCESS) {
    ACE_ASSERT(type == REG_DWORD);
  }

  DWORD tmp = 0;
  err = ACE_TEXT_RegQueryValueEx(key, "Multicast", 0, &type, 
    (LPBYTE) &tmp, &sz);
  if (err == ERROR_SUCCESS) {
    ACE_ASSERT(type == REG_DWORD);
    this->multicast_ = tmp != 0;
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
ACE_CString
Options::output_filename (void) const
{
  return this->ior_output_file_;
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
 * @retval false Do not listen for multicast location requests.
 * @retval true Do Listen.
 */
bool
Options::multicast (void) const
{
  return this->multicast_;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton <Options, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton <Options, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
