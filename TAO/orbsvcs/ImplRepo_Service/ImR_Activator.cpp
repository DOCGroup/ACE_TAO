/* -*- C++ -*- */
// $Id$

#include "Activator_Options.h"
#include "ImR_Activator_i.h"
#include "Activator_NT_Service.h"

int
run_standalone (void)
{
  ImR_Activator_i server;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Initialize the ImR_Activator_i server.
      int status = server.init (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (status == -1)
        {
          return 1;
        }
      else
        {
          // Run the server if it is initialized correctly.
          server.run (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // We should only get here if the shutdown_repo operation has
          // been called. That operation saves the Activator's state.  

          if (status == -1)
            return 1;
        }
      return 0;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex, "System Exception");
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_PRINT_EXCEPTION (userex, "User Exception");
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Unknown Exception");
    }
  ACE_ENDTRY;

  return 1;
}

#if defined (ACE_WIN32)
ACE_NT_SERVICE_DEFINE (service, Activator_NT_Service, IMR_ACTIVATOR_SERVICE_NAME);
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

  if (OPTIONS::instance()->service())
    return run_service ();

  return run_standalone ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#elif defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)

template ACE_Singleton<Options, ACE_Null_Mutex> *ACE_Singleton<Options, ACE_Null_Mutex>::singleton_;

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
