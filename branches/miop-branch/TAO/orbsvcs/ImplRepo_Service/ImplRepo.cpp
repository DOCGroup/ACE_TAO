/* -*- C++ -*- */
// $Id$

#include "Options.h"
#include "ImplRepo_i.h"
#include "NT_Service.h"

int
run_standalone (void)
{
  ImplRepo_i server;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      int status = server.init (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (status == -1)
        {
          return 1;
        }
      else
        {
          server.run (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          status = server.fini (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (status == -1)
            return 1;
        }
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex, "System Exception");
      return 1;
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_PRINT_EXCEPTION (userex, "User Exception");
      return 1;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Unknown Exception");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

#if defined (ACE_WIN32)
ACE_NT_SERVICE_DEFINE (service, ImR_NT_Service, IMR_SERVICE_NAME);
#endif /* ACE_WIN32 */

int
run_service (void)
{
#if defined (ACE_WIN32)
  // @todo: Update me

  // If we get here, we either run the app in debug mode (-d) or are
  // being called from the service manager to start the service.

  ACE_NT_SERVICE_RUN (service, SERVICE::instance (), ret);

  if (ret == 0)
    ACE_ERROR ((LM_ERROR, "%p\n", "Couldn't start service"));

  return ret;
#else /* ACE_WIN32 */
  return 1;
#endif /* ACE_WIN32 */
}

int
main (int argc, char *argv[])
{
  int result = OPTIONS::instance ()->init (argc, argv);

  if (result < 0)
    return 1;  // Error parsing args
  else if (result > 0)
    return 0;  // No error, but we should exit anyway.

  if (OPTIONS::instance ()->service () == 1)
    return run_service ();

  return run_standalone ();
}
