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
                            0,
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
