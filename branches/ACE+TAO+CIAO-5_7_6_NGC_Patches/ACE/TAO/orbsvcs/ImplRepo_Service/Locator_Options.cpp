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
#include "ace/Log_Msg.h"
#include "ace/OS_NS_strings.h"

ACE_RCSID (ImplRepo_Service,
           Options,
           "$Id$")


#if defined (ACE_WIN32)
static const HKEY SERVICE_REG_ROOT = HKEY_LOCAL_MACHINE;
// This string must agree with the one used in Locator_NT_Service.h
static const ACE_TCHAR *SERVICE_REG_PATH =
  ACE_TEXT ("SYSTEM\\CurrentControlSet\\Services\\TAOImR\\Parameters");
#endif /* ACE_WIN32 */

static const int DEFAULT_PING_INTERVAL = 10; // seconds
static const int DEFAULT_START_TIMEOUT = 60; // seconds

Options::Options ()
: repo_mode_ (REPO_NONE)
, erase_repo_ (false)
, debug_ (1)
, multicast_ (false)
, service_ (false)
, ping_interval_(DEFAULT_PING_INTERVAL)
, startup_timeout_(DEFAULT_START_TIMEOUT)
, readonly_ (false)
, service_command_(SC_NONE)
, unregister_if_address_reused_ (false)
{
}

int
Options::parse_args (int &argc, ACE_TCHAR *argv[])
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
      else if (ACE_OS::strcasecmp (shifter.get_current (),
                                   ACE_TEXT ("-l")) == 0)
        {
          this->readonly_ = true;
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
                                   ACE_TEXT ("-UnregisterIfAddressReused")) == 0)
        {
          this->unregister_if_address_reused_ = true;
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
                                   ACE_TEXT ("-e")) == 0)
        {
          this->erase_repo_ = true;
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
      else if (ACE_OS::strcasecmp (shifter.get_current (),
                                   ACE_TEXT ("-v")) == 0)
        {
          shifter.consume_arg ();

          if (!shifter.is_anything_left () || shifter.get_current ()[0] == '-')
            {
              ACE_ERROR ((LM_ERROR, "Error: -v option needs a value\n"));
              this->print_usage ();
              return -1;
            }
          this->ping_interval_ =
            ACE_Time_Value (0, 1000 * ACE_OS::atoi (shifter.get_current ()));
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

int
Options::init (int argc, ACE_TCHAR *argv[])
{
  // Make an initial pass through and grab the arguments that we recognize.
  // This may also run the commands to install or remove the nt service.
  int result = this->parse_args (argc, argv);
  if (result != 0)
    {
      return result;
    }

  for (int i = 0; i < argc; ++i)
    {
      this->cmdline_ += ACE_CString (ACE_TEXT_ALWAYS_CHAR(argv[i])) + ACE_CString (" ");
    }
  return 0;
}

int
Options::init_from_registry (void)
{
  this->load_registry_options ();
  return 0;
}


void
Options::print_usage (void) const
{
  ACE_ERROR ((LM_ERROR,
              "Usage:\n"
              "\n"
              "ImplRepo_Service [-c cmd] [-d 0|1|2] [-m] [-o file]\n"
              " [-r|-p file|-x file] [-s] [-t secs] [-v secs]\n"
              "  -c command  Runs nt service commands ('install' or 'remove')\n"
              "  -d level    Sets the debug level (default 1)\n"
              "  -l          Lock the database\n"
              "  -m          Turn on multicast\n"
              "  -o file     Outputs the ImR's IOR to a file\n"
              "  -p file     Use file for storing/loading settings\n"
              "  -x file     Use XML file for storing/loading setting\n"
              "  -r          Use the registry for storing/loading settings\n"
              "  -t secs     Server startup timeout.(Default=60s)\n"
              "  -v msecs     Server verification interval.(Default=10s)\n"
              ));
}

int
Options::save_registry_options ()
{
#if defined (ACE_WIN32)
  HKEY key = 0;
  // Create or open the parameters key
  LONG err = ACE_TEXT_RegCreateKeyEx (SERVICE_REG_ROOT,
                             SERVICE_REG_PATH,
                             0,
                             ACE_TEXT(""), // class
                             REG_OPTION_NON_VOLATILE,
                             KEY_ALL_ACCESS,
                             0,
                             &key,
                             0
                             );
  if (err != ERROR_SUCCESS)
    {
      return -1;
    }
  err = ACE_TEXT_RegSetValueEx (key, ACE_TEXT("ORBInitOptions"), 0, REG_SZ,
    (LPBYTE) this->cmdline_.c_str (), this->cmdline_.length () + 1);
  ACE_ASSERT (err == ERROR_SUCCESS);

  err = ACE_TEXT_RegSetValueEx (key, ACE_TEXT("IORFile"), 0, REG_SZ,
    (LPBYTE) ior_output_file_.c_str (), ior_output_file_.length () + 1);
  ACE_ASSERT (err == ERROR_SUCCESS);

  err = ACE_TEXT_RegSetValueEx (key, ACE_TEXT("DebugLevel"), 0, REG_DWORD,
    (LPBYTE) &debug_ , sizeof (debug_));
  ACE_ASSERT(err == ERROR_SUCCESS);

  err = ACE_TEXT_RegSetValueEx (key, ACE_TEXT("PersistFile"), 0, REG_SZ,
    (LPBYTE) this->persist_file_name_.c_str (), this->persist_file_name_.length () + 1);
  ACE_ASSERT (err == ERROR_SUCCESS);

  DWORD tmp = this->ping_interval_.msec ();
  err = ACE_TEXT_RegSetValueEx (key, ACE_TEXT("PingInterval"), 0, REG_DWORD,
    (LPBYTE) &tmp, sizeof (DWORD));
  ACE_ASSERT (err == ERROR_SUCCESS);

  tmp = this->readonly_ ? 1 : 0;
  err = ACE_TEXT_RegSetValueEx (key, ACE_TEXT("Lock"), 0, REG_DWORD,
    (LPBYTE) &tmp, sizeof (DWORD));
  ACE_ASSERT (err == ERROR_SUCCESS);

  tmp = this->repo_mode_;
  err = ACE_TEXT_RegSetValueEx (key, ACE_TEXT("PersistType"), 0, REG_DWORD,
    (LPBYTE) &tmp, sizeof (DWORD));
  ACE_ASSERT (err == ERROR_SUCCESS);

  tmp = static_cast<DWORD> (this->startup_timeout_.sec());
  err = ACE_TEXT_RegSetValueEx (key, ACE_TEXT("Timeout"), 0, REG_DWORD,
    (LPBYTE) &tmp, sizeof (DWORD));
  ACE_ASSERT (err == ERROR_SUCCESS);

  tmp = multicast_ ? 1 : 0;
  err = ACE_TEXT_RegSetValueEx (key, ACE_TEXT("Multicast"), 0, REG_DWORD,
    (LPBYTE) &tmp, sizeof (DWORD));
  ACE_ASSERT (err == ERROR_SUCCESS);

  err = ::RegCloseKey (key);
  ACE_ASSERT (err == ERROR_SUCCESS);
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
  if (err != ERROR_SUCCESS)
    {
      // If there aren't any saved parameters, then that's ok.
      return 0;
    }
  ACE_TCHAR tmpstr[4096];
  DWORD sz = sizeof (tmpstr);
  DWORD type = 0;
  err = ACE_TEXT_RegQueryValueEx (key, ACE_TEXT("ORBInitOptions"), 0, &type,
    (LPBYTE) tmpstr, &sz);
  if (err == ERROR_SUCCESS)
    {
      ACE_ASSERT (type == REG_SZ);
      tmpstr[sz - 1] = '\0';
      this->cmdline_ = ACE_TEXT_ALWAYS_CHAR(tmpstr);
    }

  sz = sizeof(tmpstr);
  err = ACE_TEXT_RegQueryValueEx (key, ACE_TEXT("IORFile"), 0, &type,
    (LPBYTE) tmpstr, &sz);
  if (err == ERROR_SUCCESS)
    {
      ACE_ASSERT (type == REG_SZ);
      tmpstr[sz - 1] = '\0';
      this->ior_output_file_ = tmpstr;
    }

  sz = sizeof(debug_);
  err = ACE_TEXT_RegQueryValueEx (key, ACE_TEXT("DebugLevel"), 0, &type,
    (LPBYTE) &this->debug_ , &sz);
  if (err == ERROR_SUCCESS)
    {
      ACE_ASSERT (type == REG_DWORD);
    }

  DWORD tmp = 0;
  sz = sizeof(tmp);
  err = ACE_TEXT_RegQueryValueEx (key, ACE_TEXT("PingInterval"), 0, &type,
    (LPBYTE) &tmp, &sz);
  if (err == ERROR_SUCCESS)
    {
      ACE_ASSERT (type == REG_DWORD);
      ping_interval_.msec (static_cast<long> (tmp));
    }

  tmp = 0;
  sz = sizeof(tmp);
  err = ACE_TEXT_RegQueryValueEx (key, ACE_TEXT("Lock"), 0, &type,
    (LPBYTE) &tmp, &sz);
  if (err == ERROR_SUCCESS)
    {
      ACE_ASSERT (type == REG_DWORD);
      readonly_ = tmp != 0;
    }

  sz = sizeof(this->repo_mode_);
  err = ACE_TEXT_RegQueryValueEx (key, ACE_TEXT("PersistType"), 0, &type,
    (LPBYTE) &this->repo_mode_, &sz);
  if (err == ERROR_SUCCESS)
    {
      ACE_ASSERT (type == REG_DWORD);
    }

  tmp = 0;
  sz = sizeof(tmp);
  err = ACE_TEXT_RegQueryValueEx (key, ACE_TEXT("Timeout"), 0, &type,
    (LPBYTE) &tmp, &sz);
  if (err == ERROR_SUCCESS)
    {
      ACE_ASSERT (type == REG_DWORD);
      this->startup_timeout_.sec (tmp);
    }

  tmp = 0;
  sz = sizeof(tmp);
  err = ACE_TEXT_RegQueryValueEx (key, ACE_TEXT("Multicast"), 0, &type,
    (LPBYTE) &tmp, &sz);
  if (err == ERROR_SUCCESS)
    {
      ACE_ASSERT (type == REG_DWORD);
      this->multicast_ = tmp != 0;
    }

  sz = sizeof(tmpstr);
  err = ACE_TEXT_RegQueryValueEx (key, ACE_TEXT("PersistFile"), 0, &type,
    (LPBYTE) tmpstr, &sz);
  if (err == ERROR_SUCCESS)
    {
      ACE_ASSERT (type == REG_SZ);
      tmpstr[sz - 1] = '\0';
      this->persist_file_name_ = tmpstr;
    }

  err = ::RegCloseKey (key);
  ACE_ASSERT (err == ERROR_SUCCESS);
#endif
  return 0;
}

bool
Options::service (void) const
{
  return this->service_;
}

unsigned int
Options::debug (void) const
{
  return this->debug_;
}

const ACE_TString&
Options::ior_filename (void) const
{
  return this->ior_output_file_;
}

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

const char*
Options::cmdline(void) const {
  return this->cmdline_.c_str ();
}

const ACE_TString&
Options::persist_file_name(void) const {
  return this->persist_file_name_;
}

ACE_Time_Value
Options::startup_timeout (void) const
{
  return this->startup_timeout_;
}

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

bool
Options::repository_erase (void) const
{
  return this->erase_repo_;
}

bool
Options::readonly (void) const
{
  return this->readonly_;
}

bool
Options::unregister_if_address_reused (void) const
{
  return this->unregister_if_address_reused_;
}
