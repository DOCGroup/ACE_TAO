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

#if !defined (ACE_NT_SERVICE_H)
#define ACE_NT_SERVICE_H

#include "ace/inc_user_config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
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

  virtual ~ACE_NT_Service (void);

  virtual int open (void *args = 0);
  // Hook called to open the service.  By default, will set the status to
  // START_PENDING, svc(), wait(), then set the status to STOPPED.

  virtual int svc (void) = 0;
  // The actual service implementation.  This function must be implemented
  // by subclasses.  It is expected that this function will set the status
  // to RUNNING.

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
  // Set the svc_handle_ member.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  int report_status (DWORD new_status, DWORD time_hint = 0);

protected:
  DWORD                  start_time_;   // Estimate of init time needed
  SERVICE_STATUS_HANDLE  svc_handle_;   // Service handle - doesn't need close.
  SERVICE_STATUS         svc_status_;
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

#if defined (__ACE_INLINE__)
#include "ace/NT_Service.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HAS_WINNT4 */
#endif /* ACE_SERVICE_OBJECT_H */
