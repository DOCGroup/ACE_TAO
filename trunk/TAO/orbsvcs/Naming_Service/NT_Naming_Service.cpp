// -*- C++ -*-

// $Id$

#include /**/ "NT_Naming_Service.h"

#if defined (ACE_WIN32) && !defined (ACE_LACKS_WIN32_SERVICES)

#include /**/ "Naming_Service.h"
#include "tao/ORB_Core.h"
#include "ace/ARGV.h"
#include "orbsvcs/Log_Macros.h"

#define REGISTRY_KEY_ROOT HKEY_LOCAL_MACHINE
#define TAO_REGISTRY_SUBKEY ACE_TEXT ("SOFTWARE\\ACE\\TAO")
#define TAO_NAMING_SERVICE_OPTS_NAME ACE_TEXT ("TaoNamingServiceOptions")
#define TAO_SERVICE_PARAM_COUNT "TaoServiceParameterCount"

AutoFinalizer::AutoFinalizer (TAO_NT_Naming_Service &service)
  : service_ (service)
{
}

AutoFinalizer::~AutoFinalizer ()
{
  service_.report_status (SERVICE_STOPPED);
  ORBSVCS_DEBUG ((LM_DEBUG, "Reported service stoped\n"));
}


TAO_NT_Naming_Service::TAO_NT_Naming_Service (void)
  : argc_ (0),
    argc_save_ (0),
    argv_ (0),
    argv_save_ (0)
{
}

TAO_NT_Naming_Service::~TAO_NT_Naming_Service (void)
{
  if (argv_save_)
    {
      for (int i = 0; i < argc_save_; i++)
        ACE_OS::free (argv_save_[i]);

      ACE_OS::free (argv_save_);
    }
}

void
TAO_NT_Naming_Service::handle_control (DWORD control_code)
{
  if (control_code == SERVICE_CONTROL_SHUTDOWN
      || control_code == SERVICE_CONTROL_STOP)
    {
      // Just in case any of the following method calls
      // throws in a way we do not expect.
      // This instance's destructor will notify the OS.
      AutoFinalizer afinalizer (*this);

      report_status (SERVICE_STOP_PENDING);

      // This must be all that needs to be done since this method is executing
      // in a separate thread from the one running the reactor.
      // When the reactor is stopped it calls ORB::destroy(), which in turn
      // calls ORB::shutdown(1) *and* unbinds the ORB from the ORB table.

      try
        {
          TAO_ORB_Core_instance ()->orb ()->shutdown (1);
        }
      catch (const CORBA::Exception&)
        {
          // What should we do here? Even the log messages are not
          // showing up, since the thread that runs this is not an ACE
          // thread. It is allways spawned/controlled by Windows ...
        }
    }
  else
  {
    ACE_NT_Service::handle_control (control_code);
  }
}

int
TAO_NT_Naming_Service::handle_exception (ACE_HANDLE)
{
  return 0;
}

void
TAO_NT_Naming_Service::report_error (const ACE_TCHAR *format,
                                     const ACE_TCHAR *val,
                                     LONG result)
{
  ACE_TCHAR msg[100];
  ACE_TEXT_FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM,
                          0,
                          result,
                          LANG_SYSTEM_DEFAULT,
                          msg,
                          100,0);
  ORBSVCS_DEBUG ((LM_DEBUG, format, val, msg));
}

void
TAO_NT_Naming_Service::arg_manip (char *args, DWORD arglen, bool query)
{
  HKEY hkey = 0;

  // It looks in the NT Registry under
  // \\HKEY_LOCAL_MACHINE\SOFTWARE\ACE\TAO for the value of
  // "TaoNamingServiceOptions" for any Naming Service options such as
  // "-ORBListenEndpoints".

  // Get/Set Naming Service options from the NT Registry.

  LONG result = ACE_TEXT_RegOpenKeyEx (REGISTRY_KEY_ROOT,
                                       TAO_REGISTRY_SUBKEY,
                                       0,
                                       query ? KEY_READ : KEY_WRITE,
                                       &hkey);

  DWORD type = REG_EXPAND_SZ;

  if (query)
    {
      *args = '\0';
      if (result == ERROR_SUCCESS)
        {
          result = ACE_TEXT_RegQueryValueEx (hkey,
                                             TAO_NAMING_SERVICE_OPTS_NAME,
                                             0,
                                             &type,
                                             (BYTE *)args,
                                             &arglen);
          if (result != ERROR_SUCCESS)
            {
              this->report_error (ACE_TEXT ("Could not query %s, %s\n"),
                                  TAO_NAMING_SERVICE_OPTS_NAME, result);
            }
        }
      else
        {
          this->report_error (ACE_TEXT ("No key for %s, %s\n"),
                              TAO_REGISTRY_SUBKEY, result);
        }
    }
  else
    {
      if (result != ERROR_SUCCESS)
        {
          result = ACE_TEXT_RegCreateKeyEx (REGISTRY_KEY_ROOT,
                                            TAO_REGISTRY_SUBKEY,
                                            0,
                                            0,
                                            0,
                                            KEY_WRITE,
                                            0,
                                            &hkey,
                                            0);
        }
      DWORD bufSize = static_cast<DWORD>(ACE_OS::strlen (args) + 1);
      if (result == ERROR_SUCCESS)
        {
          result = ACE_TEXT_RegSetValueEx (hkey,
                                           TAO_NAMING_SERVICE_OPTS_NAME,
                                           0,
                                           type,
                                           (BYTE *)args,
                                           bufSize);
          if (result != ERROR_SUCCESS)
            {
               this->report_error (ACE_TEXT ("Could not set %s, %s\n"),
                                   TAO_NAMING_SERVICE_OPTS_NAME, result);
            }
        }
      else
        {
          this->report_error (ACE_TEXT ("Could not create key %s, %s\n"),
                              TAO_REGISTRY_SUBKEY, result);
        }
    }

  RegCloseKey (hkey);

}

int
TAO_NT_Naming_Service::set_args (const ACE_TCHAR *args)
{
  char argbuf[ACE_DEFAULT_ARGV_BUFSIZ];
  if (args == 0)
    {
      this->arg_manip (argbuf, ACE_DEFAULT_ARGV_BUFSIZ, true);
      ACE_OS::printf ("%s\n", argbuf);
    }
  else
    {
      ACE_OS::strcpy (argbuf, ACE_TEXT_ALWAYS_CHAR (args));
      this->arg_manip (argbuf, 0, false);
    }
  return 0;
}

int
TAO_NT_Naming_Service::init (int argc,
                             ACE_TCHAR *argv[])
{
  // Add options to the args list (if any).
  char argbuf[ACE_DEFAULT_ARGV_BUFSIZ];
  this->arg_manip (argbuf, ACE_DEFAULT_ARGV_BUFSIZ, true);

  if (ACE_OS::strlen (argbuf) > 0)
    {
      ACE_ARGV args (argbuf);
      // Allocate the internal args list to be one bigger than the
      // args list passed into the function. We use a 'save' list in
      // case we use a 'destructive' args list processor - this way we
      // maintain the correct argv and argc for memory freeing
      // operations in the destructor.
      argv_save_ = (ACE_TCHAR **) ACE_OS::malloc (sizeof (ACE_TCHAR *) * (argc + args.argc ()));

      // Copy the values into the internal args buffer.
      int i;
      for (i = 0; i < argc; i++)
        argv_save_[i] = ACE_OS::strdup (argv[i]);

      int j = 0;
      for (i = argc; i < static_cast<int> ((args.argc () + argc)); i++)
        argv_save_[i] = ACE_OS::strdup (args.argv ()[j++]);

      // Set the arg counter.
      argc_save_ = argc + args.argc ();
      argc_ = argc_save_;
      argv_ = argv_save_;
    }
  else
    {
      argc_ = argc;
      argv_ = argv;
    }

  return 0;
}

int
TAO_NT_Naming_Service::svc (void)
{
  TAO_Naming_Service naming_service;

  if (naming_service.init (argc_, argv_) == -1)
    return -1;

  try
    {
      // Just in case handle_control does not get the chance
      // to execute, or is never called by Windows. This instance's
      // destructor will inform the OS of our demise.
      AutoFinalizer afinalizer (*this);

      ORBSVCS_DEBUG ((LM_INFO, "Notifying Windows of service startup\n"));
      report_status (SERVICE_RUNNING);

      naming_service.run ();
    }
  catch (const CORBA::Exception& ex)
    {
      ORBSVCS_DEBUG ((LM_INFO, "Exception in service - exitting\n"));
      ex._tao_print_exception ("TAO NT Naming Service");
      return -1;
    }

  ORBSVCS_DEBUG ((LM_INFO, "Exiting gracefully\n"));
  return 0;
}

#endif /* ACE_WIN32 && !ACE_LACKS_WIN32_SERVICES */
