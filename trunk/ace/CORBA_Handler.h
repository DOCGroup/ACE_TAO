/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    CORBA_Handler.h
//
// = AUTHOR
//    Douglas C. Schmidt (schmidt@cs.wustl.edu) and
//    Irfan Pyarali (irfan@wuerl.wustl.edu).
//
// ============================================================================

#ifndef ACE_CORBA_HANDLER_H
#define ACE_CORBA_HANDLER_H

#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Pipe.h"

#if (defined (ACE_HAS_ORBIX) && (ACE_HAS_ORBIX != 0)) || (defined (ACE_HAS_MT_ORBIX) && (ACE_HAS_MT_ORBIX != 0))
#define EXCEPTIONS
#define WANT_ORBIX_FDS
#include /**/ <CORBA.h>
// #include /**/ <daemon.hh>
#undef EXCEPTIONS
#undef WANT_ORBIX_FDS

class ACE_Export ACE_CORBA_Handler : public ACE_Service_Object
{
  // = TITLE
  //     Handle CORBA requests in conjunction with ACE.
  //
  // = DESCRIPTION
  //     Note, do *NOT* inherit from this class!  Instead, use the
  //     <ACE_MT_CORBA_HAndler> and <ACE_ST_CORBA_Handler> as
  //     Singletons.
public:
  // = Activation and deactivation methods.

  virtual int activate_service (const char *service_name,
                                const char *marker_name = 0,
                                const char *service_location = 0);
  // Activate and register <service_name> with the Orbix daemon.  If
  // <marker_name> and <service_location> are != 0 then do a "putit"
  // to register this service with orbixd.  This method also
  // increments the reference count of active services using the
  // ACE_ST_CORBA_Handler.

  virtual int deactivate_service (const char *service_name = 0,
                                  const char *marker_name = 0);
  // Decrement the reference count and free up all the
  // resources if this is the last service to be using
  // the ACE_ST_CORBA_Handler...

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  ACE_CORBA_Handler (void);
  // Make this into an "abstract" class...

  virtual ~ACE_CORBA_Handler (void);
  // Note virtual destructor...

  virtual int register_service (const char *service_name,
                                const char *marker_name,
                                const char *service_location);
  // Register <service_name> by doing a "putit" to register the
  // <service_name> using the <marker_name> at <service_location> with
  // orbixd.

  virtual int remove_service (const char *service_name,
                              const char *marker_name = 0);
  // Register <service_name> by doing a "putit" to register
  // <service_name> using the <marker_name> with orbixd.

  ssize_t reference_count_;
  // Keep track of the number of active CORBA_Handlers.

private:
  // = Disallow assignment and initialization.
  ACE_CORBA_Handler (const ACE_CORBA_Handler &rhs);
  const ACE_CORBA_Handler &operator= (const ACE_CORBA_Handler &rhs);
};

class ACE_Export ACE_ST_CORBA_Handler : public ACE_CORBA_Handler
  // = TITLE
  //     Handle single-threaded CORBA requests in conjunction with the
  //     <ACE_Reactor>.
{
public:
  // = Singleton access point.
  static ACE_CORBA_Handler *instance (void);
  // Returns a Singleton.

  // = Demuxing hook.
  virtual int handle_input (ACE_HANDLE);
  // Process the next Orbix event.

  // = Dynamic linking hooks.
  virtual int suspend (void);
  // Atomically suspend all the threads associated with the <thr_mgr ()>.

  virtual int resume (void);
  // Atomically resume all the threads associated with the <thr_mgr ()_>.

  // = Iterations dictate # of <processNextEvent> calls per-callback.
  size_t iterations (void);
  // Get the current iteration.

  void iterations (size_t);
  // Set the current iteration.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  void get_orbix_descriptors (void);
  // Preinitialize any descriptors that Orbix is using.  This is
  // called in instance ().

  ACE_ST_CORBA_Handler (void);
  // Constructors (ensure Singleton...).

  virtual ~ACE_ST_CORBA_Handler (void);
  // Destructor cleans up resources.

  static void insert_handle (ACE_HANDLE);
  // Insert a descriptor into the ACE_Reactor that Orbix has just added.

  static void remove_handle (ACE_HANDLE);
  // Remove a descriptor from the ACE_Reactor that Orbix has just deleted.

  static ACE_ST_CORBA_Handler *instance_;
  // ACE_ST_CORBA_Handler is a singleton object.

  size_t iterations_;
  // Number of iterations to process per processNextEvent() call.
};

#if defined (ACE_HAS_MT_ORBIX) && (ACE_HAS_MT_ORBIX != 0)

class ACE_Export ACE_MT_CORBA_Handler : public ACE_CORBA_Handler, public ACE_CORBA_1 (ThreadFilter)
{
  // = TITLE
  //     Handle multi-threaded CORBA requests in conjunction with the
  //     <ACE_Reactor>.
public:
  // = Singleton access point.
  static ACE_CORBA_Handler *instance (void);
  // Returns a Singleton.

  // = Demuxing hook.
  virtual int handle_input (ACE_HANDLE);
  // Process the next Orbix event.

  // = Threading hook.
  void thr_mgr (ACE_Thread_Manager *tm);
  // Set the Thread_Manager used by ACE_MT_CORBA_Handler

  ACE_Thread_Manager *thr_mgr (void) const;
  // Get the Thread_Manager used by ACE_MT_CORBA_Handler

  // = Dynamic linking hooks.
  virtual int suspend (void);
  // Atomically suspend all the threads associated with the <thr_mgr ()>.

  virtual int resume (void);
  // Atomically resume all the threads associated with the <thr_mgr ()>.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  static void *process_events (void *);
  // function executed by new thread

  ACE_MT_CORBA_Handler (void);
  // Constructors (ensure Singleton...).

  virtual ~ACE_MT_CORBA_Handler (void);
  // Destructor cleans up resources.

  virtual int inRequestPreMarshal (ACE_CORBA_1 (Request) &r,
                                   ACE_CORBA_1 (Environment) &IT_env = ACE_CORBA_1 (default_environment));
  // Take the incoming request and pass it to this->handle_input() but
  // through the Reactor.

  static ACE_MT_CORBA_Handler *instance_;
  // ACE_MT_CORBA_Handler is a singleton object.

  ACE_Thread_Manager *thr_mgr_;
  // Event demultiplexor used by ACE_ST_CORBA_Handler.

  ACE_Pipe pipe_;
  // Used to send CORBA::Requests through the server
};
#endif /* ACE_HAS_MT_ORBIX */

#if defined (__ACE_INLINE__)
#include "ace/CORBA_Handler.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HAS_ORBIX */
#endif /* ACE_CORBA_HANDLER_H */
