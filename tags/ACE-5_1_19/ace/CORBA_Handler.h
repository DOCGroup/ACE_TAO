/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    CORBA_Handler.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author Irfan Pyarali (irfan@cs.wustl.edu)
 */
//=============================================================================


#ifndef ACE_CORBA_HANDLER_H
#define ACE_CORBA_HANDLER_H
#include "ace/pre.h"

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

/**
 * @class ACE_CORBA_Handler
 *
 * @brief Handle Orbix requests in conjunction with ACE.
 *
 * Note, do *NOT* inherit from this class!  Instead, use the
 * <ACE_MT_CORBA_HAndler> and <ACE_ST_CORBA_Handler> as
 * Singletons.
 */
class ACE_Export ACE_CORBA_Handler : public ACE_Service_Object
{
public:
  // = Activation and deactivation methods.

  /**
   * Activate and register <service_name> with the Orbix daemon.  If
   * <marker_name> and <service_location> are != 0 then do a "putit"
   * to register this service with orbixd.  This method also
   * increments the reference count of active services using the
   * ACE_ST_CORBA_Handler.
   */
  virtual int activate_service (const char *service_name,
                                const char *marker_name = 0,
                                const char *service_location = 0);

  /**
   * Decrement the reference count and free up all the
   * resources if this is the last service to be using
   * the ACE_ST_CORBA_Handler...
   */
  virtual int deactivate_service (const char *service_name = 0,
                                  const char *marker_name = 0);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  /// Make this into an "abstract" class...
  ACE_CORBA_Handler (void);

  /// Note virtual destructor...
  virtual ~ACE_CORBA_Handler (void);

  /**
   * Register <service_name> by doing a "putit" to register the
   * <service_name> using the <marker_name> at <service_location> with
   * orbixd.
   */
  virtual int register_service (const char *service_name,
                                const char *marker_name,
                                const char *service_location);

  /// Register <service_name> by doing a "putit" to register
  /// <service_name> using the <marker_name> with orbixd.
  virtual int remove_service (const char *service_name,
                              const char *marker_name = 0);

  /// Keep track of the number of active CORBA_Handlers.
  ssize_t reference_count_;

private:
  // = Disallow assignment and initialization.
  ACE_CORBA_Handler (const ACE_CORBA_Handler &rhs);
  const ACE_CORBA_Handler &operator= (const ACE_CORBA_Handler &rhs);
};

/**
 * @class ACE_ST_CORBA_Handler
 *
 * @brief Handle single-threaded Orbix requests in conjunction with the
 * <ACE_Reactor>.
 *
 * You should NOT use this class unless you've got a VERY old
 * version of Orbix that only supports single-threading.  If
 * you're using a more recent version of Orbix use the
 * <ACE_MT_CORBA_Handler>.
 */
class ACE_Export ACE_ST_CORBA_Handler : public ACE_CORBA_Handler
{
public:
  // = Singleton access point.
  /// Returns a Singleton.
  static ACE_CORBA_Handler *instance (void);

  // = Demuxing hook.
  /// Process the next Orbix event.
  virtual int handle_input (ACE_HANDLE);

  // = Dynamic linking hooks.
  /// Atomically suspend all the threads associated with the <thr_mgr>.
  virtual int suspend (void);

  /// Atomically resume all the threads associated with the <thr_mgr>.
  virtual int resume (void);

  // = Iterations dictate # of <processNextEvent> calls per-callback.
  /// Get the current iteration.
  size_t iterations (void);

  /// Set the current iteration.
  void iterations (size_t);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  /// Preinitialize any descriptors that Orbix is using.  This is
  /// called in <instance>.
  void get_orbix_descriptors (void);

  /// Constructors (ensure Singleton...).
  ACE_ST_CORBA_Handler (void);

  /// Destructor cleans up resources.
  virtual ~ACE_ST_CORBA_Handler (void);

  /// Insert a descriptor into the ACE_Reactor that Orbix has just added.
  static void insert_handle (ACE_HANDLE);

  /// Remove a descriptor from the ACE_Reactor that Orbix has just deleted.
  static void remove_handle (ACE_HANDLE);

  /// Clean up the singleton at program rundown.
  static void instance_cleaner (void *object, void *param);

  /// ACE_ST_CORBA_Handler is a singleton object.
  static ACE_ST_CORBA_Handler *instance_;

  /// Number of iterations to process per <processNextEvent> call.
  size_t iterations_;

  // If the user has complete control of all Orbix callback processing and
  // really, really knows how to handle all of the involved interworkings,
  // they can set up to daisy-chain Orbix callbacks from this class to
  // other handlers established outside the control of this class. This is
  // an intrinsically dangerous thing to do, and is most often the wrong
  // thing to do. But if you must, set ACE_TAKEOVER_ORBIX_CALLBACKS in the
  // config.h file before including the platform's config file.
# if defined (ACE_TAKEOVER_ORBIX_CALLBACKS)
  static int set_callbacks_;
  static OrbixIOCallback previous_orbix_open_callback_;
  static OrbixIOCallback previous_orbix_close_callback_;
# endif /* ACE_TAKEOVER_ORBIX_CALLBACKS */
};

#if defined (ACE_HAS_MT_ORBIX) && (ACE_HAS_MT_ORBIX != 0)

/**
 * @class ACE_MT_CORBA_Handler
 *
 * @brief Handle multi-threaded Orbix requests in conjunction with the
 * <ACE_Reactor>.
 *
 * If you are using MT-Orbix (which has been the default Orbix
 * for years) you should use this class rather than
 * <ACE_ST_CORBA_Handler>.  See
 * www.cs.wustl.edu/~schmidt/COOTS-96.ps.gz
 * for an explanation of what this class does for Orbix.
 */
class ACE_Export ACE_MT_CORBA_Handler : public ACE_CORBA_Handler, public ACE_CORBA_1 (ThreadFilter)
{
public:
  // = Singleton access point.
  /// Returns a Singleton.
  static ACE_CORBA_Handler *instance (void);

  // = Demuxing hook.
  /// Process the next Orbix event.
  virtual int handle_input (ACE_HANDLE);

  // = Threading hook.
  /// Set the Thread_Manager used by ACE_MT_CORBA_Handler
  void thr_mgr (ACE_Thread_Manager *tm);

  /// Get the Thread_Manager used by ACE_MT_CORBA_Handler
  ACE_Thread_Manager *thr_mgr (void) const;

  // = Dynamic linking hooks.
  /// Atomically suspend all the threads associated with the <thr_mgr>.
  virtual int suspend (void);

  /// Atomically resume all the threads associated with the <thr_mgr>.
  virtual int resume (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  /// function executed by new thread
  static void *process_events (void *);

  /// Constructors (ensure Singleton...).
  ACE_MT_CORBA_Handler (void);

  /// Destructor cleans up resources.
  virtual ~ACE_MT_CORBA_Handler (void);

  /// Take the incoming request and pass it to <handle_input> through
  /// the Reactor.
  virtual int inRequestPreMarshal (ACE_CORBA_1 (Request) &r,
                                   ACE_CORBA_1 (Environment) &IT_env = ACE_CORBA_1 (default_environment));

  /// ACE_MT_CORBA_Handler is a singleton object.
  static ACE_MT_CORBA_Handler *instance_;

  /// Event demultiplexor used by ACE_ST_CORBA_Handler.
  ACE_Thread_Manager *thr_mgr_;

  /// Used to send CORBA::Requests through the server
  ACE_Pipe pipe_;
};
#endif /* ACE_HAS_MT_ORBIX */

#if defined (__ACE_INLINE__)
#include "ace/CORBA_Handler.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HAS_ORBIX */
#include "ace/post.h"
#endif /* ACE_CORBA_HANDLER_H */
