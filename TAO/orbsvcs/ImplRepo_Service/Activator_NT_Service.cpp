//=============================================================================
/**
 *  @file   Activator_NT_Service.cpp
 *
 *  $Id$
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 *  @author John Tucker <jtucker@infoglide.com>
 *  @author Mike Vitalo <mvitalo@infoglide.com>
 */
//=============================================================================

#include "Activator_NT_Service.h"

#if defined (ACE_WIN32)

#include "ImR_Activator_i.h"
#include "Activator_Options.h"

#include "tao/ORB_Core.h"

#include "ace/Reactor.h"
/**
 * Handles the SERVICE_CONTROL_SHUTDOWN and SERVICE_CONTROL_STOP commands
 * by shutting down the ORB.  Otherwise ACE_NT_Service::handle_control
 * handles the command.
 */
void
Activator_NT_Service::handle_control (DWORD control_code)
{
  if (control_code == SERVICE_CONTROL_SHUTDOWN
      || control_code == SERVICE_CONTROL_STOP)
    {
      report_status (SERVICE_STOP_PENDING);
      TAO_ORB_Core_instance ()->reactor ()->end_reactor_event_loop ();
      TAO_ORB_Core_instance ()->orb ()->shutdown (1);
    }
  else
    {
      ACE_NT_Service::handle_control (control_code);
    }
}


/**
 */
int
Activator_NT_Service::handle_exception (ACE_HANDLE)
{
  return 0;
}


/**
 * We do almost the same thing as we do in run_standalone () except that
 * we update the report_status after init.
 */
int
Activator_NT_Service::svc (void)
{
  ImR_Activator_i server;
  Activator_Options opts;

  if (opts.init_from_registry() != 0)
    {
      report_status (SERVICE_STOPPED);
      return -1;
    }

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      int status = server.init (opts ACE_ENV_ARG_PARAMETER);

      if (status == -1)
        {
          report_status (SERVICE_STOPPED);
          return -1;
        }
      else
        {
          report_status (SERVICE_RUNNING);
          server.run ();

          status = server.fini ();

          report_status (SERVICE_STOPPED);

        }
        if (status != -1)
            return 0;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex, IMR_ACTIVATOR_DISPLAY_NAME);
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_PRINT_EXCEPTION (userex, IMR_ACTIVATOR_DISPLAY_NAME);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, IMR_ACTIVATOR_DISPLAY_NAME);
    }
  ACE_ENDTRY;

  report_status (SERVICE_STOPPED);
  return -1;
}

#endif /* ACE_WIN32 */
