// $Id$

#include "ImR_Locator_i.h"
#include "Locator_NT_Service.h"
#include "Locator_Options.h"
#include "orbsvcs/Shutdown_Utilities.h"

class ImR_Locator_Shutdown : public Shutdown_Functor
{
public:
  ImR_Locator_Shutdown (ImR_Locator_i& imr);

  void operator() (int which_signal);
private:
  ImR_Locator_i& imr_;
};

ImR_Locator_Shutdown::ImR_Locator_Shutdown (ImR_Locator_i &imr)
  : imr_(imr)
{
}

void
ImR_Locator_Shutdown::operator () (int /*which_signal*/)
{
  try
    {
      this->imr_.shutdown (true);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("ImR: ");
    }
}

int
run_standalone (Options& opts)
{
  ImR_Locator_i server;

  ImR_Locator_Shutdown killer (server);
  Service_Shutdown kill_contractor(killer);

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
ACE_NT_SERVICE_DEFINE (service, Locator_NT_Service, IMR_LOCATOR_SERVICE_NAME);
#endif /* ACE_WIN32 && !ACE_LACKS_WIN32_SERVICES */

int
run_service (void)
{
#if defined (ACE_WIN32) && !defined (ACE_LACKS_WIN32_SERVICES)

  SERVICE::instance ()->name (IMR_LOCATOR_SERVICE_NAME, IMR_LOCATOR_DISPLAY_NAME);

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
run_service_command (Options& opts)
{
  if (opts.service_command () == Options::SC_NONE)
    return 0;

#if defined (ACE_WIN32) && !defined (ACE_LACKS_WIN32_SERVICES)
  SERVICE::instance()->name (IMR_LOCATOR_SERVICE_NAME, IMR_LOCATOR_DISPLAY_NAME);

  if (opts.service_command () == Options::SC_INSTALL)
    {
      const DWORD MAX_PATH_LENGTH = 4096;
      ACE_TCHAR pathname[MAX_PATH_LENGTH];

      DWORD length = ACE_TEXT_GetModuleFileName (0, pathname, MAX_PATH_LENGTH);
      if (length == 0 || length >= MAX_PATH_LENGTH - sizeof(" -s"))
        {
          ACE_ERROR ((LM_ERROR, "Error: Could not get module file name\n"));
          return -1;
        }

      // Append the command used for running the implrepo as a service
      ACE_OS::strcat (pathname, ACE_TEXT (" -s"));

      int ret =  SERVICE::instance ()->insert (SERVICE_DEMAND_START,
                                           SERVICE_ERROR_NORMAL,
                                           pathname);
      if (ret != -1)
        {
          ACE_DEBUG ((LM_DEBUG, "ImR: Service installed.\n"));
          opts.save_registry_options ();
        }
      else
        {
          ACE_ERROR ((LM_ERROR, "Error: Failed to install service. error:%d\n", errno));
        }
      if (ret == 0)
        return 1;
    }
  else if (opts.service_command () == Options::SC_REMOVE)
    {
      int ret = SERVICE::instance ()->remove ();
      ACE_DEBUG ((LM_DEBUG, "ImR: Service removed.\n"));
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
  Options opts;

  int result = opts.init (argc, argv);
  if (result < 0)
    return 1;  // Error
  else if (result > 0)
    return 0;  // No error, but we should exit anyway.

  result = run_service_command(opts);
  if (result < 0)
    return 1;  // Error
  else if (result > 0)
    return 0;  // No error, but we should exit anyway.

  if (opts.service ())
  {
    return run_service ();
  }

  return run_standalone (opts);
}

