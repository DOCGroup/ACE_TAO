/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    NT_Service.h
//
// = AUTHOR
//    Steve Huston
//
// ============================================================================

#ifndef ACE_NT_SERVICE_H
#define ACE_NT_SERVICE_H

#include "ace/inc_user_config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_WINNT4) && ACE_HAS_WINNT4 != 0

#include "ace/Service_Object.h"
#include "ace/Synch.h"
#include "ace/Task.h"

// NT Services can be implemented using the framework defined by the
// ACE_NT_Service class, and the macros defined in this file.  Some quick
// refresher notes on NT Services:
//   - The main program defines an array of entries describing the services
//     offered.  The ACE_NT_SERVICE_ENTRY macro can help with this.
//   - For each service, a separate ServiceMain and Handler function need to
//     be defined.  These are taken care of by the ACE_NT_SERVICE_DEFINE
//     macro.
//   - When the main program/thread calls StartServiceCtrlDispatcher, NT
//     creates a thread for each service, and runs the ServiceMain function
//     for the service in that new thread.  When that thread exits, the service
//     is gone.
//
// To use this facility, you could derive a class from ACE_Service_Object (if
// you want to start via ACE's service configurator), or use any other class
// to run when the image starts (assuming that NT runs the image).  You must
// set up an NT SERVICE_TABLE_ENTRY array to define your service(s).  You
// can use the ACE_NT_SERVICE_... macros defined below for this.
// A SERVICE_TABLE might look like this:
//       ACE_NT_SERVICE_REFERENCE(Svc1);  // If service is in another file
//       SERVICE_TABLE_ENTRY myServices[] = {
//                    ACE_NT_SERVICE_ENTRY ("MyNeatService", Svc1),
//                    { 0, 0 } };
//
// In the file where your service(s) are implemented, use the
// ACE_NT_SERVICE_DEFINE macro to set up the following:
//    1. A pointer to the service's implementation object (must be derived
//       from ACE_NT_Service).
//    2. The service's Handler function (forwards all requests to the
//       ACE_NT_Service-derived object's handle_control function).
//    3. The service's ServiceMain function.  Creates a new instance of the
//       ACE_NT_Service-derived class SVCCLASS, unless one has been created
//       already.
//
// If you are using all the default constructor values, you
// can let the generated ServiceMain function create the object, else
// you need to create it by hand before calling StartServiceCtrlDispatcher.
// Set the pointer so ServiceMain won't create another one.
// Another reason you may want to do the object creation yourself is if you
// want to also implement suspend and resume functions (the ones inherited
// from ACE_Service_Object) to do something intelligent to the services which
// are running, like call their handle_control functions to request suspend
// and resume actions, similar to what NT would do if a Services control panel
// applet would do if the user clicks on Suspend.


// ACE_NT_SERVICE_START_TIMEOUT is an estimate of the number of milliseconds
// your service will take to start.  Default is 5 seconds; you can pass
// a different value (or set one) when you create the ACE_NT_Service object
// for your service.
#if !defined ACE_NT_SERVICE_START_TIMEOUT
#define ACE_NT_SERVICE_START_TIMEOUT  5000
#endif /* ACE_NT_SERVICE_TIMEOUT */

class ACE_Export ACE_NT_Service : public ACE_Task<ACE_MT_SYNCH>
{
  // = TITLE
  //     Provide the base class which defines the interface for controlling
  //     an NT service.
  //
  // = DESCRIPTION
public:
  // = Initialization and termination methods.
  ACE_NT_Service (DWORD start_timeout = ACE_NT_SERVICE_START_TIMEOUT,
                  DWORD service_type = SERVICE_WIN32_OWN_PROCESS,
                  DWORD controls_mask = SERVICE_ACCEPT_STOP);
  // Constructor primarily for use when running the service.

  ACE_NT_Service (LPCTSTR name,
                  LPCTSTR desc = 0,
                  DWORD start_timeout = ACE_NT_SERVICE_START_TIMEOUT,
                  DWORD service_type = SERVICE_WIN32_OWN_PROCESS,
                  DWORD controls_mask = SERVICE_ACCEPT_STOP);
  // Constructor primarily for use when inserting/removing/controlling
  // the service.

  virtual ~ACE_NT_Service (void);

  // = Functions to operate the service

  virtual int open (void *args = 0);
  // Hook called to open the service.  By default, will set the status to
  // START_PENDING, svc(), wait(), then set the status to STOPPED.

  virtual int svc (void);
  // The actual service implementation.  This function need not be overridden
  // by applications that are just using SCM capabilities, but must be
  // by subclasses when actually running the service.  It is expected that
  // this function will set the status to RUNNING.

  virtual void  handle_control (DWORD control_code);
  // This function is called in response to a request from the Service
  // Dispatcher.  It must interact with the svc() function to effect the
  // requested control operation.  The default implementation handles
  // all requests as follows:
  //    SERVICE_CONTROL_STOP: set stop pending, set cancel flag
  //    SERVICE_CONTROL_PAUSE: set pause pending, suspend(), set paused
  //    SERVICE_CONTROL_CONTINUE: set continue pending, resume(), set running
  //    SERVICE_CONTROL_INTERROGATE: reports current status
  //    SERVICE_CONTROL_SHUTDOWN: same as SERVICE_CONTROL_STOP.

  void svc_handle (const SERVICE_STATUS_HANDLE new_svc_handle);
  // Set the svc_handle_ member.  This is only a public function because
  // the macro-generated service function calls it.


  // = Methods which can be used to do SCP-like functions. The first group
  // are used to register/insert and remove the service's definition in the
  // SCM registry.

  void name (LPCTSTR name, LPCTSTR desc = 0);
  // Sets the name and description for the service.
  // If desc is 0, it takes the same value as name.

  LPCTSTR name (void) const;
  // Get the service name.

  LPCTSTR desc (void) const;
  // Get the service description.

  int insert (DWORD start_type = SERVICE_DEMAND_START,
              DWORD error_control = SERVICE_ERROR_IGNORE,
              LPCTSTR exe_path = 0,
              LPCTSTR group_name = 0,
              LPDWORD tag_id = 0,
              LPCTSTR dependencies = 0,
              LPCTSTR account_name = 0,
              LPCTSTR password = 0);
  // Insert (create) the service in the NT Service Control Manager,
  // with the given creation values.  exe_path defaults to the path name
  // of the program that calls the function.  All other 0-defaulted arguments
  // pass 0 into the service creation, taking NT_specified defaults.
  // Returns -1 on error, 0 on success.

  int remove (void);
  // Remove the service from the NT Service Control Manager.  Returns -1 on
  // error, 0 on success.  This just affects the SCM and registry - the
  // can and will keep running fine if it is already running.

  int startup (DWORD startup);
  // Sets the startup type for the service.  Returns -1 on error, 0 on success.

  DWORD startup (void);
  // Returns the current startup type.


  // = Methods which control the service's execution.
  // These methods to start/pause/resume/stop/check the service all have the
  // following common behavior with respect to <wait_time> and return value.
  //   <wait_time> is a pointer to an ACE_Time_Value object.  If not supplied
  //   (a zero pointer) the function will wait indefinitely for the action
  //   to be finalized (service reach running state, completely shut down,
  //   etc.) or get "stuck" before returning.  If the time is supplied, it
  //   specifies how long to wait for the service to reach a steady state,
  //   and on return, it is updated to the service's last reported wait hint.
  //   So, if you want to control the waiting yourself (for example, you want
  //   to react to UI events during the wait) specify a <wait_time> of (0, 0)
  //   and use the updated time to know when to check the service's state
  //   again.
  //   NOTE!!!! The wait_time things don't work yet.  The calls always
  //   check status once, and do not wait for it to change.
  //
  //   The return value from start_svc, stop_svc, pause_svc, continue_svc is
  //   0 if the request to NT to effect the change was made successfully.  The
  //   service may refuse to change, or not do what you wanted; so if you
  //   need to know, supply a <svc_state> pointer to receive the service's
  //   reported last state on return and check it to see if it's what you
  //   want.  The functions only return -1 when the actual request to the
  //   service is refused - this would include privilege restrictions and
  //   if the service is not configured to receive the request (this is most
  //   likely to happen in the case of pause and continue).

  int start_svc (ACE_Time_Value *wait_time = 0,
                 DWORD *svc_state = 0,
                 DWORD argc = 0, LPCTSTR *argv = 0);
  // Start the service (must have been inserted before).  wait_time is the
  // time to wait for the service to reach a steady state before returning.
  // If it is 0, the function waits as long as it takes for the service to
  // reach the 'running' state, or gets stuck in some other state, or exits.
  // If <wait_time> is supplied, it is updated on return to hold the service's
  // last reported wait hint.
  // svc_state can be used to receive the state which the service settled in.
  // If the value is 0, the service never ran.  argc/argv are passed
  // to the service's ServiceMain function when it starts.  Returns 0 for
  // success, -1 for error.

  int stop_svc (ACE_Time_Value *wait_time = 0, DWORD *svc_state = 0);
  // Requests the service to stop.  Will wait up to <wait_time> for the service
  // to actually stop.  If not specified, the function waits until the service
  // either stops or gets stuck in some other state before it stops.
  // If <svc_state> is specified, it receives the last reported state of the
  // service.  Returns 0 if the request was made successfully, -1 if not.

  int pause_svc (ACE_Time_Value *wait_time = 0, DWORD *svc_state = 0);
  // Pause the service.

  int continue_svc (ACE_Time_Value *wait_time = 0, DWORD *svc_state = 0);
  // Continue the service.

  DWORD state (ACE_Time_Value *wait_hint = 0);
  // Get the current state for the service.  If <wait_hint> is not 0, it
  // receives the service's reported wait hint.
  // Note that this function returns 0 on failure (not -1 as is usual in ACE).
  // A zero return would (probably) only be returned if there is either no
  // service with the given name in the SCM database, or the caller does not
  // have sufficient rights to access the service state.  The set of valid
  // service state values are all greater than 0.

  int state (DWORD *pstate, ACE_Time_Value *wait_hint = 0);
  // A version of state() which returns -1 for failure, 0 for success.
  // The DWORD pointed to by pstate receives the state value.

  int test_access (DWORD desired_access = SERVICE_ALL_ACCESS);
  // Test access to the object's service in the SCM.  The service must already
  // have been inserted in the SCM database.  This function has no affect on
  // the service itself.  Returns 0 if the specified access is allowed, -1
  // otherwise (either the access is denied, or there is a problem with the
  // service's definition - check ACE_OS::last_error to get the specific
  // error indication.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  int report_status (DWORD new_status, DWORD time_hint = 0);

  SC_HANDLE svc_sc_handle (void);
  // Return the svc_sc_handle_ member. If the member is null,
  // it retrieves the handle from the Service Control Manager and caches it.

  void wait_for_service_state (DWORD desired_state, ACE_Time_Value *wait_time);
  // Waits for the service to reach <desired_state> or get (apparently) stuck
  // before it reaches that state.  Will wait at most <wait_time> to get to
  // the desired state.  If <wait_time> is 0, then the function keeps waiting
  // until the desired state is reached or the service doesn't update its
  // state any further.
  // The svc_status_ class member is updated upon return.
  // NOTE - the timeout doesn't currently work - it always acts like
  // ACE_Time_Value::zero is passed - it checks the state once but doesn't
  // wait after that.

protected:
  DWORD                  start_time_;   // Estimate of init time needed
  SERVICE_STATUS_HANDLE  svc_handle_;   // Service handle - doesn't need close.
  SERVICE_STATUS         svc_status_;

  SC_HANDLE              svc_sc_handle_;// Service's SCM handle
  LPTSTR                 name_;
  LPTSTR                 desc_;

};

// These macros help to get things set up correctly at compile time
// and to take most of the grudge work out of creating the proper
// functions and doing the registrations.
//
// ACE_NT_SERVICE_DEFINE - defines the 'ServiceMain' function which NT will
//                         call in its own thread when the service control
//                         dispatcher starts.

#define ACE_NT_SERVICE_DEFINE(SVCNAME, SVCCLASS, SVCDESC)                   \
  ACE_NT_Service * _ace_nt_svc_obj_##SVCNAME = 0;                           \
  VOID WINAPI ace_nt_svc_handler_##SVCNAME (DWORD fdwControl) {             \
    _ace_nt_svc_obj_##SVCNAME->handle_control(fdwControl);                  \
  }                                                                         \
  VOID WINAPI ace_nt_svc_main_##SVCNAME (DWORD dwArgc, LPTSTR *lpszArgv) {  \
    int delete_svc_obj = 0;                                                 \
    if (_ace_nt_svc_obj_##SVCNAME == 0) {                                   \
      ACE_NEW (_ace_nt_svc_obj_##SVCNAME, SVCCLASS);                        \
      if (_ace_nt_svc_obj_##SVCNAME == 0)                                   \
        return;                                                             \
      delete_svc_obj = 1;                                                   \
    }                                                                       \
    _ace_nt_svc_obj_##SVCNAME->init(dwArgc, lpszArgv);                      \
    _ace_nt_svc_obj_##SVCNAME->svc_handle(                                  \
                  RegisterServiceCtrlHandler(SVCDESC,                       \
                                          &ace_nt_svc_handler_##SVCNAME));  \
    _ace_nt_svc_obj_##SVCNAME->open();                                      \
    _ace_nt_svc_obj_##SVCNAME->wait();                                      \
    _ace_nt_svc_obj_##SVCNAME->fini();                                      \
    if (delete_svc_obj) {                                                   \
      delete _ace_nt_svc_obj_##SVCNAME;                                     \
      _ace_nt_svc_obj_##SVCNAME = 0;                                        \
    }                                                                       \
    return;                                                                 \
  }

#define ACE_NT_SERVICE_REFERENCE(SVCNAME)                                  \
extern VOID WINAPI ace_nt_svc_main_##SVCNAME (DWORD dwArgc, LPTSTR *lpszArgv);

#define ACE_NT_SERVICE_ENTRY(SVCDESC, SVCNAME)                             \
                      { SVCDESC, &ace_nt_svc_main_##SVCNAME }

#define ACE_NT_SERVICE_RUN(SVCNAME, SVCINSTANCE, RET)                      \
  SERVICE_TABLE_ENTRY _ace_nt_svc_table[2] =                               \
  {                                                                        \
    ACE_NT_SERVICE_ENTRY(#SVCNAME, SVCNAME),                               \
    { 0, 0 }                                                               \
  };                                                                       \
  _ace_nt_svc_obj_##SVCNAME = SVCINSTANCE;                                 \
  ACE_OS::last_error (0);                                                  \
  int RET = StartServiceCtrlDispatcher(_ace_nt_svc_table);

#if defined (__ACE_INLINE__)
#include "ace/NT_Service.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HAS_WINNT4 */
#endif /* ACE_SERVICE_OBJECT_H */
