/* -*- C++ -*- */
// $Id$

#include /**/ "Naming_Service.h"
#include /**/ "NT_Naming_Service.h"

#define REGISTRY_KEY_ROOT HKEY_LOCAL_MACHINE
#define TAO_REGISTRY_SUBKEY "SOFTWARE\\ACE\\TAO"
#define TAO_NAMING_SERVICE_OPTS_NAME "TaoNamingServiceOptions"

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
      report_status (SERVICE_STOP_PENDING);
      TAO_ORB_Core_instance ()->reactor ()->end_reactor_event_loop ();
      TAO_ORB_Core_instance ()->orb ()->shutdown (1);
      report_status (SERVICE_STOPPED);
    }
  else
    ACE_NT_Service::handle_control (control_code);
}

int
TAO_NT_Naming_Service::handle_exception (ACE_HANDLE)
{
  return 0;
}

int
TAO_NT_Naming_Service::init (int argc,
                             ASYS_TCHAR *argv[])
{
  HKEY hkey = 0;
  BYTE buf[ACE_DEFAULT_ARGV_BUFSIZ];

  *buf = '\0';

  // This solution is very kludgy.  It looks in the NT Registry under
  // \\HKEY_LOCAL_MACHINE\SOFTWARE\ACE\TAO for the value of
  // "TaoNamingServiceOptions" for any Naming Service options such as
  // "-ORBEndpoint".

  // Get Naming Service options from the NT Registry.

  RegOpenKeyEx (REGISTRY_KEY_ROOT,
                TAO_REGISTRY_SUBKEY,
                0,
                KEY_READ|KEY_WRITE,
                &hkey);

  DWORD type;
  DWORD bufSize = sizeof (buf);

  RegQueryValueEx (hkey,
                   TAO_NAMING_SERVICE_OPTS_NAME,
                   NULL,
                   &type,
                   buf,
                   &bufSize);

  RegCloseKey (hkey);

  // Add options to the args list (if any).

  if (ACE_OS::strlen ((char *) buf) > 0)
    {
      // Allocate the internal args list to be one bigger than the
      // args list passed into the function. We use a 'save' list in
      // case we use a 'destructive' args list processor - this way we
      // maintain the correct argv and argc for memory freeing
      // operations in the destructor.
      argv_save_ = (char **) malloc (sizeof (char *) *(argc + 1));

      // Copy the values into the internal args buffer.
      for (int i = 0; i < argc; i++)
        argv_save_[i] = ACE_OS::strdup (argv[i]);

      // Add the configured option to the argument list.
      argv_save_[argc] = ACE_OS::strdup ((char *) buf);

      // Set the arg counter.
      argc_save_ = argc + 1;
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

  if (naming_service.init (argc_,
                           argv_) == -1)
    return -1;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      report_status (SERVICE_RUNNING);
      naming_service.run (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "TAO NT Naming Service");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (1);

  return 0;
}
