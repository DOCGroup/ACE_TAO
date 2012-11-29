
//=============================================================================
/**
 *  @file    ntsvc.cpp
 *
 *  $Id$
 *
 *    This is the implementation of the NT service.  It beeps every 2
 *    seconds until the service is stopped.
 *
 *
 *  @author Gonzalo Diethelm <gonzo@cs.wustl.edu> and Steve Huston <shuston@riverace.com>
 */
//=============================================================================


#include "ace/Reactor.h"
#include "ntsvc.h"

#if defined (ACE_WIN32) && !defined (ACE_LACKS_WIN32_SERVICES)

Service::Service (void)
{
  // Remember the Reactor instance.
  reactor (ACE_Reactor::instance ());
}

Service::~Service (void)
{
  if (ACE_Reactor::instance ()->cancel_timer(this) == -1)
    ACE_ERROR ((LM_ERROR,
                "Service::~Service failed to cancel_timer.\n"));
}

// This method is called when the service gets a control request.  It
// handles requests for stop and shutdown by calling terminate ().
// All others get handled by calling up to inherited::handle_control.

void
Service::handle_control (DWORD control_code)
{
  if (control_code == SERVICE_CONTROL_SHUTDOWN
      || control_code == SERVICE_CONTROL_STOP)
    {
      report_status (SERVICE_STOP_PENDING);

      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("Service control stop requested\n")));
      stop_ = 1;
      reactor ()->notify (this,
                          ACE_Event_Handler::EXCEPT_MASK);
    }
  else
    inherited::handle_control (control_code);
}

// This is just here to be the target of the notify from above... it
// doesn't do anything except aid on popping the reactor off its wait
// and causing a drop out of handle_events.

int
Service::handle_exception (ACE_HANDLE)
{
  return 0;
}

// Beep every two seconds.  This is what this NT service does...

int
Service::handle_timeout (const ACE_Time_Value &tv,
                         const void *)
{
  ACE_UNUSED_ARG (tv);
  MessageBeep (MB_OK);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%T (%t): Beep...\n")));
  return 0;
}

// This is the main processing function for the Service.  It sets up
// the initial configuration and runs the event loop until a shutdown
// request is received.

int
Service::svc (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Service::svc\n")));

  // As an NT service, we come in here in a different thread than the
  // one which created the reactor.  So in order to do anything, we
  // need to own the reactor. If we are not a service, report_status
  // will return -1.
  if (report_status (SERVICE_RUNNING) == 0)
    reactor ()->owner (ACE_Thread::self ());

  this->stop_ = 0;

  // Schedule a timer every two seconds.
  ACE_Time_Value tv (2, 0);
  ACE_Reactor::instance ()->schedule_timer (this, 0, tv, tv);

  while (!this->stop_)
    reactor ()->handle_events ();

  // Cleanly terminate connections, terminate threads.
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Shutting down\n")));
  reactor ()->cancel_timer (this);
  return 0;
}

#endif /* ACE_WIN32 && !ACE_LACKS_WIN32_SERVICES */
