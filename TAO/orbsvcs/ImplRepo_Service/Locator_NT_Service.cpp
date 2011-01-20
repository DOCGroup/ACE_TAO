//=============================================================================
/**
 *  @file   Locator_NT_Service.cpp
 *
 *  $Id$
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 *  @author John Tucker <jtucker@infoglide.com>
 *  @author Mike Vitalo <mvitalo@infoglide.com>
 */
//=============================================================================

#include "Locator_NT_Service.h"

#if defined (ACE_WIN32) && !defined (ACE_LACKS_WIN32_SERVICES)

#include "ImR_Locator_i.h"
#include "Locator_Options.h"

#include "tao/ORB_Core.h"
#include "tao/corba.h"
#include "ace/Reactor.h"

const ACE_TCHAR* IMR_LOCATOR_SERVICE_NAME = ACE_TEXT("TAOImR");
const ACE_TCHAR* IMR_LOCATOR_DISPLAY_NAME = ACE_TEXT("TAO Implementation Repository");
const ACE_TCHAR* IMR_LOCATOR_DESCRIPTION = ACE_TEXT("Implementation Repository Locator service for TAO");

/**
 * Handles the SERVICE_CONTROL_SHUTDOWN and SERVICE_CONTROL_STOP commands
 * by shutting down the ORB.  Otherwise ACE_NT_Service::handle_control
 * handles the command.
 */
void
Locator_NT_Service::handle_control (DWORD control_code)
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
Locator_NT_Service::handle_exception (ACE_HANDLE)
{
  return 0;
}


/**
 * We do almost the same thing as we do in run_standalone ()
 */
int
Locator_NT_Service::svc (void)
{
  ImR_Locator_i server;
  Options opts;

  if (opts.init_from_registry () != 0)
    {
      report_status (SERVICE_STOPPED);
      return -1;
    }

  try
    {
      int status = server.init (opts);

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
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception (IMR_LOCATOR_DISPLAY_NAME);
    }
  catch (const CORBA::UserException& userex)
    {
      userex._tao_print_exception (IMR_LOCATOR_DISPLAY_NAME);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (IMR_LOCATOR_DISPLAY_NAME);
    }

  report_status (SERVICE_STOPPED);

  return -1;
}

#endif /* ACE_WIN32 && !ACE_LACKS_WIN32_SERVICES */
