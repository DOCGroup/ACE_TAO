/* -*- C++ -*- */
// $Id$

#include "ImplRepo_i.h"
#include "NT_ImplRepo_Service.h"

ACE_RCSID(ImplRepo_Service, NT_ImplRepo_Service, "$Id$")

#define REGISTRY_KEY_ROOT HKEY_LOCAL_MACHINE
#define TAO_REGISTRY_SUBKEY "SOFTWARE\\ACE\\TAO"
#define TAO_IMPLREPO_SERVICE_OPTS_NAME "TaoImplRepoServiceOptions"

TAO_NT_ImplRepo_Service::TAO_NT_ImplRepo_Service (void)
  : argc_ (0),
    argc_save_ (0),
    argv_ (0),
    argv_save_ (0)
{
}

TAO_NT_ImplRepo_Service::~TAO_NT_ImplRepo_Service (void)
{
  if (argv_save_)
    {
      for (int i = 0; i < this->argc_save_; i++)
        {
          ACE_OS::free (this->argv_save_[i]);
        }

      ACE_OS::free (argv_save_);
    }
}

void
TAO_NT_ImplRepo_Service::handle_control (DWORD control_code)
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
    {
      ACE_NT_Service::handle_control (control_code);
    }
}

int
TAO_NT_ImplRepo_Service::handle_exception (ACE_HANDLE)
{
  return 0;
}

int
TAO_NT_ImplRepo_Service::init (int argc,
                               ACE_TCHAR *argv[])
{
  HKEY hkey = 0;
  BYTE buf[ACE_DEFAULT_ARGV_BUFSIZ];

  *buf = '\0';

  // This solution is very kludgy.  It looks in the NT Registry under
  // \\HKEY_LOCAL_MACHINE\SOFTWARE\ACE\TAO for the value of
  // "TaoImplRepoServiceOptions" for any ImplRepo Service options such as
  // "-ORBEndpoint".

  // Get ImplRepo Service options from the NT Registry.

  RegOpenKeyEx (REGISTRY_KEY_ROOT,
                TAO_REGISTRY_SUBKEY,
                0,
                KEY_READ|KEY_WRITE,
                &hkey);

  DWORD type;
  DWORD bufSize = sizeof (buf);

  RegQueryValueEx (hkey,
                   TAO_IMPLREPO_SERVICE_OPTS_NAME,
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
        {
          this->argv_save_[i] = ACE_OS::strdup (argv[i]);
        }

      // Add the configured option to the argument list.
      this->argv_save_[argc] = ACE_OS::strdup ((char *) buf);

      // Set the arg counter.
      this->argc_save_ = argc + 1;
      this->argc_ = this->argc_save_;
      this->argv_ = this->argv_save_;
    }
  else
    {
      this->argc_ = argc;
      this->argv_ = argv;
    }

  return 0;
}

int
TAO_NT_ImplRepo_Service::svc (void)
{
  ImplRepo_i server;

  int status = 0;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      status = server.init (argc_, 
                            argv_, 
                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (status == -1)
        {
          return -1;
        }
      else
        {
          report_status (SERVICE_RUNNING);
          server.run (ACE_TRY_ENV);
          ACE_TRY_CHECK;

          status = server.fini (ACE_TRY_ENV);
          ACE_TRY_CHECK;

          if (status == -1)
            {
              return -1;
            }
        }
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex, 
                           "TAO NT ImplRepo Service");
      return -1;
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_PRINT_EXCEPTION (userex, 
                           "TAO NT ImplRepo Service");
      return -1;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, 
                           "TAO NT ImplRepo Service");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
