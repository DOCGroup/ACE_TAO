// -*- C++ -*-

// $Id$

#include /**/ "NT_Naming_Service.h"

#if defined (ACE_WIN32)

#include /**/ "Naming_Service.h"
#include "tao/ORB_Core.h"
#include "ace/ARGV.h"

#define REGISTRY_KEY_ROOT HKEY_LOCAL_MACHINE
#define TAO_REGISTRY_SUBKEY "SOFTWARE\\ACE\\TAO"
#define TAO_NAMING_SERVICE_OPTS_NAME "TaoNamingServiceOptions"
#define TAO_SERVICE_PARAM_COUNT "TaoServiceParameterCount"

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
                             ACE_TCHAR *argv[])
{
  HKEY hkey = 0;
  BYTE buf[ACE_DEFAULT_ARGV_BUFSIZ];

  *buf = '\0';

  // This solution is very kludgy.  It looks in the NT Registry under
  // \\HKEY_LOCAL_MACHINE\SOFTWARE\ACE\TAO for the value of
  // "TaoNamingServiceOptions" for any Naming Service options such as
  // "-ORBEndpoint".

  // Get Naming Service options from the NT Registry.

  ACE_TEXT_RegOpenKeyEx (REGISTRY_KEY_ROOT,
                         TAO_REGISTRY_SUBKEY,
                         0,
                         KEY_READ,
                         &hkey);

  DWORD type;
  DWORD bufSize = sizeof (buf);

  ACE_TEXT_RegQueryValueEx (hkey,
                            TAO_NAMING_SERVICE_OPTS_NAME,
                            NULL,
                            &type,
                            buf,
                            &bufSize);

  RegCloseKey (hkey);

  // Add options to the args list (if any).

  if (ACE_OS::strlen ((char *) buf) > 0)
    {
      ACE_ARGV args ((const char*) buf);
      // Allocate the internal args list to be one bigger than the
      // args list passed into the function. We use a 'save' list in
      // case we use a 'destructive' args list processor - this way we
      // maintain the correct argv and argc for memory freeing
      // operations in the destructor.
      argv_save_ = (char **) ACE_OS::malloc (sizeof (char *) * (argc + args.argc ()));

      // Copy the values into the internal args buffer.
      int i;
      for (i = 0; i < argc; i++)
        argv_save_[i] = ACE_OS::strdup (argv[i]);

      int j = 0;
      for (i = argc; i < ACE_static_cast (int, (args.argc () + argc)); i++)
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

  if (naming_service.init (argc_,
                           argv_) == -1)
    return -1;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      report_status (SERVICE_RUNNING);
      naming_service.run (ACE_ENV_SINGLE_ARG_PARAMETER);
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

#endif /* ACE_WIN32 */
