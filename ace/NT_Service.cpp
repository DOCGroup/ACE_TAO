// $Id$

// NT_Service.cpp

#include "ace/inc_user_config.h"
#if defined (ACE_HAS_WINNT4) && ACE_HAS_WINNT4 != 0

#define ACE_BUILD_DLL
#include "ace/NT_Service.h"
#include "ace/Service_Object.h"

#if !defined (__ACE_INLINE__)
#include "ace/NT_Service.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_NT_Service)

// This default implementation of ACE_NT_Service::open sets the service's
// status to START_PENDING with the estimated time until STARTED set to the
// value given when this object was constructed.  Then the svc function is
// called, which implements the guts of the service.  Note that this function
// is running in a thread created by the OS, not by ACE_Thread_Manager.
// The thread manager does not know anything about this thread.  The service
// can, however, use ACE_Thread_Manager to start more threads if desired.
// When the svc function returns, the service status is set to STOPPED, and
// exit codes set based on errno/GetLastError if the svc function returns -1.
//
// The svc function is expected to set the service status to SERVICE_RUNNING
// after it initializes.
//
// The handle_control function will be called for each time there is a request
// for the service.  It is up to that function and svc to cooperate to both
// respond appropriately to the request (by at least updating the service's
// status) and to fulfill the request.

int
ACE_NT_Service::open (void *args)
{

int svc_return;

  report_status(SERVICE_START_PENDING, 0);

  if ((svc_return = this->svc()) == 0) {
    this->svc_status_.dwWin32ExitCode = NO_ERROR;
    this->svc_status_.dwServiceSpecificExitCode = 0;
  }
  else {
    if (errno == 0) {
      this->svc_status_.dwWin32ExitCode = GetLastError();
    }
    else {
      this->svc_status_.dwWin32ExitCode = ERROR_SERVICE_SPECIFIC_ERROR;
      this->svc_status_.dwServiceSpecificExitCode = errno;
    }
  }

  report_status(SERVICE_STOPPED, 0);

  return svc_return;

}

void
ACE_NT_Service::handle_control (DWORD control_code)
{

  switch(control_code) {
  case SERVICE_CONTROL_SHUTDOWN:
  case SERVICE_CONTROL_STOP:
    report_status(SERVICE_STOP_PENDING);
    /* how to cancel? */
    break;

  case SERVICE_CONTROL_PAUSE:
    report_status(SERVICE_PAUSE_PENDING);
    this->suspend();
    report_status(SERVICE_PAUSED);
    break;

  case SERVICE_CONTROL_CONTINUE:
    report_status(SERVICE_CONTINUE_PENDING);
    this->resume();
    report_status(SERVICE_RUNNING);
    break;

  case SERVICE_CONTROL_INTERROGATE:
    report_status(0);
    break;
  }

  return;

}



// report_status
//
// Reports the current status.  If new_status is not 0, it sets the 
// status to the new value before reporting.  NOTE - this assumes that
// no actual service status values have the value 0.  This is true in
// WinNT 4.
// If the status is a 'pending' type, the supplied time hint is used
// unless it's 0, in which case the existing hint is used.  The dwWaitHint
// is not updated by this function.  The checkpoint is incremented
// by one after a pending report.
// 
int
ACE_NT_Service::report_status (DWORD new_status, DWORD time_hint)
{

int   bump_checkpoint = 0,
      retval = 0;
DWORD save_controls = 0;


  if (new_status != 0)
    this->svc_status_.dwCurrentState = new_status;
  switch(this->svc_status_.dwCurrentState) {
  case SERVICE_START_PENDING:
    save_controls = this->svc_status_.dwControlsAccepted;
    this->svc_status_.dwControlsAccepted = 0;
    /* Fall through */
  case SERVICE_STOP_PENDING:
  case SERVICE_CONTINUE_PENDING:
  case SERVICE_PAUSE_PENDING:
    this->svc_status_.dwWaitHint = time_hint ? time_hint : this->start_time_;
    bump_checkpoint = 1;
    break;

  default:
    this->svc_status_.dwCheckPoint = 0;
  }

  retval = SetServiceStatus(this->svc_handle_, &this->svc_status_) ? 0 : -1;

  if (save_controls != 0)
    this->svc_status_.dwControlsAccepted = save_controls;

  if (bump_checkpoint)
    ++this->svc_status_.dwCheckPoint;

  return retval;

}

#endif /* ACE_HAS_WINNT4 */
