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
#include "ace/Log_Msg.h"

#if defined (ACE_WIN32)
static const HKEY SERVICE_REG_ROOT = HKEY_LOCAL_MACHINE;
// This string must agree with the one used in Activator_NT_Service.h
static const ACE_TCHAR *SERVICE_REG_PATH =
  ACE_TEXT ("SYSTEM\\CurrentControlSet\\Services\\TAOImRActivator\\Parameters");
#endif /* ACE_WIN32 */

Activator_Options::Activator_Options ()
: debug_ (1)
, service_ (false)
, notify_imr_ (false)
, service_command_(SC_NONE)
, env_buf_len_ (Activator_Options::ENVIRONMENT_BUFFER)
, max_env_vars_ (Activator_Options::ENVIRONMENT_MAX_VARS)
{
}

int
Activator_Options::parse_args (int &argc, ACE_TCHAR *argv[])
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
                                   ACE_TEXT ("install_no_imr")) == 0)
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
                                   ACE_TEXT ("-e")) == 0)
        {
          shifter.consume_arg ();

          if (!shifter.is_anything_left () || shifter.get_current ()[0] == '-')
            {
              ACE_ERROR ((LM_ERROR, "Error: -e option needs "
                                    "an environment buffer length\n"));
              this->print_usage ();
              return -1;
            }

          this->env_buf_len_ = ACE_OS::atoi (shifter.get_current ());
        }
      else if (ACE_OS::strcasecmp (shifter.get_current (),
                                   ACE_TEXT ("-m")) == 0)
        {
          shifter.consume_arg ();

          if (!shifter.is_anything_left () || shifter.get_current ()[0] == '-')
            {
              ACE_ERROR ((LM_ERROR, "Error: -m option needs "
                                    "a maximum number of environment vars\n"));
              this->print_usage ();
              return -1;
            }

          this->max_env_vars_ = ACE_OS::atoi (shifter.get_current ());
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
          this->ior_output_file_ = shifter.get_current ();
        }
      else if (ACE_OS::strcasecmp (shifter.get_current (),
                                   ACE_TEXT ("-s")) == 0)
        {
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
                                   ACE_TEXT ("-n")) == 0)
        {
          shifter.consume_arg ();

          if (!shifter.is_anything_left () || shifter.get_current ()[0] == '-')
            {
              ACE_ERROR ((LM_ERROR, "Error: -n option needs a name\n"));
              this->print_usage ();
              return -1;
            }
          this->name_ = ACE_TEXT_ALWAYS_CHAR(shifter.get_current ());
        }
      else if (ACE_OS::strcasecmp (shifter.get_current (),
                                   ACE_TEXT ("-l")) == 0)
        {
          this->notify_imr_ = true;
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
Activator_Options::init (int argc, ACE_TCHAR *argv[])
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
Activator_Options::init_from_registry (void)
{
  this->load_registry_options();
  return 0;
}

void
Activator_Options::print_usage (void) const
{
  ACE_ERROR ((LM_ERROR,
              "Usage:\n"
              "\n"
              "ImR_Activator [-c cmd] [-d 0|1|2] [-e buflen] [-o file] [-l] [-n name] [-m maxenv]\n"
              "\n"
              "  -c command  Runs service commands\n"
              "              ('install' or 'remove' or 'install_no_imr')\n"
              "  -d level    Sets the debug level\n"
              "  -e buflen   Set the environment buffer length in bytes for activated servants\n"
              "  -o file     Outputs the ImR's IOR to a file\n"
              "  -l          Notify the ImR when a process exits\n"
              "  -n name     Specify a name for the Activator\n")
             );
}

int
Activator_Options::save_registry_options()
{
#if defined (ACE_WIN32)
  HKEY key = 0;
  // Create or open the parameters key
  LONG err = ACE_TEXT_RegCreateKeyEx (SERVICE_REG_ROOT,
                             SERVICE_REG_PATH,
                             0,
                             const_cast<ACE_TCHAR *> (ACE_TEXT("")), // class
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
    (LPBYTE) this->ior_output_file_.c_str (), this->ior_output_file_.length () + 1);
  ACE_ASSERT (err == ERROR_SUCCESS);

  err = ACE_TEXT_RegSetValueEx (key, ACE_TEXT("DebugLevel"), 0, REG_DWORD,
    (LPBYTE) &this->debug_ , sizeof (this->debug_));
  ACE_ASSERT (err == ERROR_SUCCESS);

  err = ACE_TEXT_RegSetValueEx( key, ACE_TEXT("Name"), 0, REG_SZ,
    (LPBYTE) this->name_.c_str (), this->name_.length () + 1);
  ACE_ASSERT (err == ERROR_SUCCESS);

  DWORD tmpint = this->notify_imr_;
  err = ACE_TEXT_RegSetValueEx (key, ACE_TEXT("NotifyImR"), 0, REG_DWORD,
    (LPBYTE) &tmpint , sizeof (tmpint));
  ACE_ASSERT (err == ERROR_SUCCESS);

  tmpint = this->env_buf_len_;
  err = ACE_TEXT_RegSetValueEx(key, ACE_TEXT("EnvBufLen"), 0, REG_DWORD,
    (LPBYTE) &tmpint , sizeof(tmpint));
  ACE_ASSERT(err == ERROR_SUCCESS);

  tmpint = this->max_env_vars_;
  err = ACE_TEXT_RegSetValueEx(key, ACE_TEXT("MaxEnvVars"), 0, REG_DWORD,
    (LPBYTE) &tmpint , sizeof(tmpint));
   ACE_ASSERT(err == ERROR_SUCCESS);

  err = ::RegCloseKey (key);
  ACE_ASSERT (err == ERROR_SUCCESS);
#endif
  return 0;
}

int
Activator_Options::load_registry_options ()
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
  char tmpstr[4096];
  DWORD sz = sizeof (tmpstr);
  DWORD type = 0;
  err = ACE_TEXT_RegQueryValueEx (key, ACE_TEXT("ORBInitOptions"), 0, &type,
    (LPBYTE) tmpstr, &sz);
  if (err == ERROR_SUCCESS)
    {
      ACE_ASSERT (type == REG_SZ);
      tmpstr[sz - 1] = '\0';
      this->cmdline_ = tmpstr;
    }

  sz = sizeof(tmpstr);
  err = ACE_TEXT_RegQueryValueEx (key, ACE_TEXT("IORFile"), 0, &type,
    (LPBYTE) tmpstr, &sz);
  if (err == ERROR_SUCCESS)
    {
      ACE_ASSERT (type == REG_SZ);
      tmpstr[sz - 1] = '\0';
      this->ior_output_file_ = ACE_TEXT_CHAR_TO_TCHAR(tmpstr);
    }

  sz = sizeof(debug_);
  err = ACE_TEXT_RegQueryValueEx (key, ACE_TEXT("DebugLevel"), 0, &type,
    (LPBYTE) &this->debug_ , &sz);
  if (err == ERROR_SUCCESS)
    {
      ACE_ASSERT (type == REG_DWORD);
    }

  sz = sizeof(tmpstr);
  err = ACE_TEXT_RegQueryValueEx (key, ACE_TEXT("Name"), 0, &type,
    (LPBYTE) tmpstr, &sz);
  if (err == ERROR_SUCCESS)
    {
      ACE_ASSERT (type == REG_SZ);
      tmpstr[sz - 1] = '\0';
      this->name_ = tmpstr;
    }

  DWORD tmpint = 0;
  sz = sizeof(tmpint);
  err = ACE_TEXT_RegQueryValueEx (key, ACE_TEXT("NotifyImR"), 0, &type,
    (LPBYTE) &tmpint , &sz);
  if (err == ERROR_SUCCESS)
    {
      ACE_ASSERT (type == REG_DWORD);
    }
  this->notify_imr_ = tmpint != 0;

  err = ACE_TEXT_RegQueryValueEx(key, ACE_TEXT("EnvBufLen"), 0, &type,
    (LPBYTE) &tmpint , &sz);
  if (err == ERROR_SUCCESS) {
    ACE_ASSERT(type == REG_DWORD);
  }
  this->env_buf_len_ = tmpint;

  err = ACE_TEXT_RegQueryValueEx(key, ACE_TEXT("MaxEnvArgs"), 0, &type,
    (LPBYTE) &tmpint , &sz);
  if (err == ERROR_SUCCESS) {
    ACE_ASSERT(type == REG_DWORD);
  }
  this->max_env_vars_ = tmpint;

  err = ::RegCloseKey (key);
  ACE_ASSERT(err == ERROR_SUCCESS);
#endif /* ACE_WIN32 */
  return 0;
}

bool
Activator_Options::service (void) const
{
  return this->service_;
}

bool
Activator_Options::notify_imr (void) const
{
  return this->notify_imr_;
}

unsigned int
Activator_Options::debug (void) const
{
  return this->debug_;
}

const ACE_TString&
Activator_Options::ior_filename (void) const
{
  return this->ior_output_file_;
}

Activator_Options::SERVICE_COMMAND
Activator_Options::service_command (void) const
{
  return this->service_command_;
}

const char*
Activator_Options::cmdline (void) const
{
  return this->cmdline_.c_str ();
}

const ACE_CString&
Activator_Options::name (void) const
{
  return this->name_;
}

int
Activator_Options::env_buf_len (void) const
{
  return this->env_buf_len_;
}

int
Activator_Options::max_env_vars (void) const
{
  return this->max_env_vars_;
}
