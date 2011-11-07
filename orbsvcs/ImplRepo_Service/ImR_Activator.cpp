/* -*- C++ -*- */
// $Id$

#include "Activator_Options.h"
#include "ImR_Activator_i.h"
#include "Activator_NT_Service.h"

#include "orbsvcs/Shutdown_Utilities.h"

class ImR_Activator_Shutdown : public Shutdown_Functor
{
public:
  ImR_Activator_Shutdown (ImR_Activator_i& act);

  void operator() (int which_signal);
private:
  ImR_Activator_i& act_;
};

ImR_Activator_Shutdown::ImR_Activator_Shutdown (ImR_Activator_i &act)
  : act_(act)
{
}

void
ImR_Activator_Shutdown::operator() (int /*which_signal*/)
{
  try
  {
    this->act_.shutdown (true);
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("ImR Activator: ");
  }
}

int
run_standalone (Activator_Options& opts)
{
  ImR_Activator_i server;

  ImR_Activator_Shutdown killer (server);
  Service_Shutdown kill_contractor (killer);

  try
    {
      int status = server.init (opts);

      if (status == -1)
        {
          return 1;
        }
      else
        {
          // Run the server if it is initialized correctly.
          server.run ();

          // End the server after its work is done.
          status = server.fini ();

          if (status == -1)
            return 1;
        }
      return 0;
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception ("System Exception");
    }
  catch (const CORBA::UserException& userex)
    {
      userex._tao_print_exception ("User Exception");
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Unknown Exception");
    }

  return 1;
}

#if defined (ACE_WIN32) && !defined (ACE_LACKS_WIN32_SERVICES)
ACE_NT_SERVICE_DEFINE (service, Activator_NT_Service, IMR_ACTIVATOR_SERVICE_NAME);
#endif /* ACE_WIN32 && !ACE_LACKS_WIN32_SERVICES */

int
run_service (void)
{
#if defined (ACE_WIN32) && !defined (ACE_LACKS_WIN32_SERVICES)
  SERVICE::instance()->name (IMR_ACTIVATOR_SERVICE_NAME, IMR_ACTIVATOR_DISPLAY_NAME);
  ACE_NT_SERVICE_RUN (service, SERVICE::instance (), ret);

  if (ret == 0)
    ACE_ERROR ((LM_ERROR, "%p\n", "Couldn't start service"));

  return ret;
#else /* ACE_WIN32 */
  return 1;
#endif /* ACE_WIN32 && !ACE_LACKS_WIN32_SERVICES */
}

/**
 * Executes the various commands that are useful for a NT service.  Right
 * now these include 'install' and 'remove'.  Others, such as 'start' and
 * 'stop' can be added, but the 'net' program in Windows already handles
 * these commands.
 */
static int
run_service_command (Activator_Options& opts)
{
  if (opts.service_command () == Activator_Options::SC_NONE)
    return 0;

#if defined (ACE_WIN32) && !defined (ACE_LACKS_WIN32_SERVICES)
  SERVICE::instance()->name (IMR_ACTIVATOR_SERVICE_NAME, IMR_ACTIVATOR_DISPLAY_NAME);

  if (opts.service_command () == Activator_Options::SC_INSTALL ||
    opts.service_command () == Activator_Options::SC_INSTALL_NO_LOCATOR)
      {
        const DWORD MAX_PATH_LENGTH = 4096;
        ACE_TCHAR pathname[MAX_PATH_LENGTH];

        DWORD length = ACE_TEXT_GetModuleFileName(0, pathname, MAX_PATH_LENGTH);
        if (length == 0 || length >= MAX_PATH_LENGTH - sizeof(" -s"))
          {
            ACE_ERROR ((LM_ERROR, "Error: Could not get module file name\n"));
            return -1;
          }

        // Append the command used for running the implrepo as a service
        ACE_OS::strcat (pathname, ACE_TEXT (" -s"));
        int ret = -1;
        if (opts.service_command () == Activator_Options::SC_INSTALL)
          {
            // Must match Locator_NT_Service.h
            const ACE_TCHAR* DEPENDS_ON = ACE_TEXT("TAOImR");

            ret =  SERVICE::instance ()->insert (SERVICE_DEMAND_START,
                                                 SERVICE_ERROR_NORMAL,
                                                 pathname,
                                                 0, // group
                                                 0, // tag
                                                 DEPENDS_ON
                                                 );
          }
        else
          {
            ret =  SERVICE::instance ()->insert (SERVICE_DEMAND_START,
                                                SERVICE_ERROR_NORMAL,
                                                pathname);
          }
      if (ret != -1)
        {
          ACE_DEBUG ((LM_DEBUG, "ImR Activator: Service installed.\n"));
          opts.save_registry_options ();
        }
      else
        {
          ACE_ERROR ((LM_ERROR, "Error: Failed to install service.\n"));
        }
      if (ret == 0)
        return 1;
    }
  else if (opts.service_command () == Activator_Options::SC_REMOVE)
    {
      int ret = SERVICE::instance ()->remove ();
      ACE_DEBUG ((LM_DEBUG, "ImR Activator: Service removed.\n"));
      if (ret == 0)
        return 1; // If successful, then we don't want to continue.
    }
  else
    {
      ACE_ERROR ((LM_ERROR, "Error: Unknown service command :%d\n",
        opts.service_command ()));
      return -1;
    }

  return -1;

#else /* ACE_WIN32 */
  ACE_ERROR ((LM_ERROR, "NT Service not supported on this platform"));
  return -1;
#endif /* ACE_WIN32 && !ACE_LACKS_WIN32_SERVICES */
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Activator_Options opts;

  int result = opts.init (argc, argv);
  if (result < 0)
    return 1;  // Error
  else if (result > 0)
    return 0;  // No error, but we should exit anyway.

  result = run_service_command (opts);
  if (result < 0)
    return 1;  // Error
  else if (result > 0)
    return 0;  // No error, but we should exit anyway.

  if (opts.service ())
    return run_service ();

  return run_standalone (opts);
}

