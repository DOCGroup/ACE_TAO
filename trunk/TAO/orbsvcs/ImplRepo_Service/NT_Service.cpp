//=============================================================================
/**
 *  @file   NT_Service.cpp
 *
 *  $Id$
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 *  @author John Tucker <jtucker@infoglide.com>
 *  @author Mike Vitalo <mvitalo@infoglide.com>
 */
//=============================================================================

#include "NT_Service.h"

// Only on Windows for now
#if defined (ACE_WIN32)

#include "ImplRepo_i.h"
#include "tao/ORB_Core.h"

/**
 * Default constructor, does nothing special.
 */
ImR_NT_Service::ImR_NT_Service (void)
{
}

/**
 * Destructor, does nothing special.
 */
ImR_NT_Service::~ImR_NT_Service (void)
{
}

/**
 * Handles the SERVICE_CONTROL_SHUTDOWN and SERVICE_CONTROL_STOP commands
 * by shutting down the ORB.  Otherwise ACE_NT_Service::handle_control
 * handles the command.
 */
void
ImR_NT_Service::handle_control (DWORD control_code)
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


/**
 */
int
ImR_NT_Service::handle_exception (ACE_HANDLE)
{
  return 0;
}


/**
 * We do almost the same thing as we do in run_standalone () except that
 * we update the report_status after init.
 */
int
ImR_NT_Service::svc (void)
{
  ImplRepo_i server;

  int status = 0;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      status = server.init (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (status == -1)
        {
          return -1;
        }
      else
        {
          report_status (SERVICE_RUNNING);
          server.run (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          status = server.fini (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (status == -1)
              return -1;
        }
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex, "Implementation Repository");
      return -1;
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_PRINT_EXCEPTION (userex, "Implementation Repository");
      return -1;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Implementation Repository");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

#endif /* ACE_WIN32 */
