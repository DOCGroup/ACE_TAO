/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Service_Config.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_SERVICE_CONFIG_H
#define ACE_SERVICE_CONFIG_H

#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Signal.h"
#include "ace/Containers.h"
#include "ace/SString.h"

// Forward decl.
class ACE_Service_Repository;
class ACE_Service_Type;
class ACE_Allocator;
class ACE_Reactor;
class ACE_Thread_Manager;

extern "C"
{
  typedef ACE_Service_Object *(*ACE_SERVICE_ALLOCATOR) (ACE_Service_Object_Exterminator *);
}

class ACE_Static_Svc_Descriptor
{
  // = TITLE
  //   Holds the information necessary to describe a statically linked
  //   Svc.
public:
  ASYS_TCHAR *name_;
  // Name of the service.

  int type_;
  // Type of service.

  ACE_SERVICE_ALLOCATOR alloc_;
  // Factory function that allocates the service.

  u_int flags_;
  // Bitmask flags indicating how the framework should delete memory.

  int active_;
  // Flag indicating whether the service starts out active.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

public:
  int operator== (ACE_Static_Svc_Descriptor &) const;
  // Compare two service descriptors for equality.
};

// Maintain a set of the statically linked service descriptors.
typedef ACE_Unbounded_Set<ACE_Static_Svc_Descriptor *>
        ACE_STATIC_SVCS;
typedef ACE_Unbounded_Set_Iterator<ACE_Static_Svc_Descriptor *>
        ACE_STATIC_SVCS_ITERATOR;

// Maintain a queue of services to be configured from the
// command-line.
typedef ACE_Unbounded_Queue<ACE_CString>
        ACE_SVC_QUEUE;
typedef ACE_Unbounded_Queue_Iterator<ACE_CString>
        ACE_SVC_QUEUE_ITERATOR;

class ACE_Export ACE_Service_Config
{
  // = TITLE
  //     Supplies common server operations for dynamic and static
  //     configuration of services.
  //
  // = DESCRIPTION
  //     NOTE: the signal_handler_ static member is allocated by the
  //     ACE_Object_Manager.  The ACE_Service_Config constructor uses
  //     signal_handler_.  Therefore, if the program has any static
  //     ACE_Service_Config objects, there might be initialization
  //     order problems.  They can be minimized, but not eliminated,
  //     by _not_ #defining ACE_HAS_NONSTATIC_OBJECT_MANAGER.
public:
  enum {MAX_SERVICES = ACE_DEFAULT_SELECT_REACTOR_SIZE};

  // = Initialization and termination methods.

  ACE_Service_Config (int ignore_static_svcs = 1,
                      size_t size = ACE_Service_Config::MAX_SERVICES,
                      int signum = SIGHUP);
  // Initialize the Service Repository.

  ACE_Service_Config (const ASYS_TCHAR program_name[],
                      LPCTSTR logger_key = ACE_DEFAULT_LOGGER_KEY);
  // Performs an open without parsing command-line arguments.

  static int open_i (const ASYS_TCHAR program_name[],
                     LPCTSTR logger_key = ACE_DEFAULT_LOGGER_KEY,
                     int ignore_default_svc_conf = 0);
  // Performs an open without parsing command-line arguments.  Returns
  // number of errors that occurred on failure and 0 otherwise.

  static int open (const ASYS_TCHAR program_name[],
                   LPCTSTR logger_key = ACE_DEFAULT_LOGGER_KEY,
                   int ignore_static_svcs = 1,
                   int ignore_default_svc_conf = 0);
  // Performs an open without parsing command-line arguments.  Returns
  // number of errors that occurred on failure and 0 otherwise.

  static int open (int argc,
                   ASYS_TCHAR *argv[],
                   LPCTSTR logger_key = ACE_DEFAULT_LOGGER_KEY,
                   int ignore_static_svcs = 1,
                   int ignore_default_svc_conf = 0);
  // This is the primary entry point into the ACE_Service_Config (the
  // constructor just handles simple initializations).  It parses
  // arguments passed in from the command-line.  Returns number of
  // errors that occurred on failure and 0 otherwise.

  virtual ~ACE_Service_Config (void);
  // Perform user-specified close activities and remove dynamic
  // memory.

  static int close (void);
  // Tidy up and perform last rites when ACE_Service_Config is shut
  // down.  This method calls <close_svcs> and <close_singletons>.
  // Returns 0.

  static int fini_svcs (void);
  // Perform user-specified close hooks and possibly delete all of the
  // configured services in the <Service_Repository>.

  static int close_svcs (void);
  // Perform user-specified close hooks on all of the configured
  // services in the <Service_Repository>, then delete the
  // <Service_Repository> itself.  Returns 0.

  static int close_singletons (void);
  // Delete the dynamically allocated Singletons (i.e., the <Reactor>,
  // <Proactor>, <ReactorEx>, <Thread_Manager>, and <Allocator>).
  // Returns 0.

  // = Reactor event loop management methods.
  static int run_reactor_event_loop (void);
  // Run the event loop until the <ACE_Reactor::handle_events> method
  // returns -1 or the <end_reactor_event_loop> method is invoked.
  // DO NOT USE THIS METHOD. It may be unsupported in future releases.
  // Use ACE_Reactor::run_event_loop() instead.

  static int run_reactor_event_loop (ACE_Time_Value &tv);
  // Run the event loop until the <ACE_Reactor::handle_events> method
  // returns -1, the <end_reactor_event_loop> method is invoked, or the
  // <ACE_Time_Value> expires.
  // DO NOT USE THIS METHOD. It may be unsupported in future releases.
  // Use ACE_Reactor::run_event_loop() instead.

  static int end_reactor_event_loop (void);
  // Instruct the <ACE_Service_Config> to terminate its event loop and
  // notifies the <ACE_Reactor::instance> so that it can wake up
  // and close down gracefully.
  // DO NOT USE THIS METHOD. It may be unsupported in future releases.
  // Use ACE_Reactor::end_event_loop() instead.

  static int reactor_event_loop_done (void);
  // Report if the Reactor's event loop is finished.
  // DO NOT USE THIS METHOD. It may be unsupported in future releases.
  // Use ACE_Reactor::event_loop_done() instead.

  static int reconfig_occurred (void);
  // True if reconfiguration occurred.

  static void reconfig_occurred (int);
  // Indicate that reconfiguration occurred.

  static void reconfigure (void);
  // Perform the reconfiguration process.

  // = The following methods are static in order to enforce Singleton
  // semantics for the Reactor, Service_Repository, Thread_Manager,
  // and Acceptor/Connector Strategy factory.  Other portions of the
  // system may need to access them at some point or another...

  // = Accessors and mutators for process-wide Singletons.

  static ACE_STATIC_SVCS *static_svcs (void);
  // Returns a pointer to the list of statically linked services.

  static ACE_Reactor *reactor (void);
  // Get pointer to a process-wide <ACE_Reactor>.
  // DO NOT USE THIS METHOD. It may be unsupported in future releases.
  // Use ACE_Reactor::instance() instead.

  static ACE_Reactor *reactor (ACE_Reactor *);
  // Set pointer to a process-wide <ACE_Reactor> and return existing
  // pointer.
  // DO NOT USE THIS METHOD. It may be unsupported in future releases.
  // Use ACE_Reactor::instance() instead.

  static ACE_Service_Repository *svc_rep (void);
  // Get pointer to a process-wide <ACE_Service_Repository>.
  // DO NOT USE THIS METHOD. It may be unsupported in future releases.
  // Use ACE_Service_Repository::instance() instead.

  static ACE_Service_Repository *svc_rep (ACE_Service_Repository *);
  // Set pointer to a process-wide <ACE_Service_Repository> and return
  // existing pointer.
  // DO NOT USE THIS METHOD. It may be unsupported in future releases.
  // Use ACE_Service_Repository::instance() instead.

  static ACE_Thread_Manager *thr_mgr (void);
  // Get pointer to a process-wide <ACE_Thread_Manager>.
  // DO NOT USE THIS METHOD. It may be unsupported in future releases.
  // Use ACE_Thread_Manager::instance() instead.

  static ACE_Thread_Manager *thr_mgr (ACE_Thread_Manager *);
  // Set pointer to a process-wide <ACE_Thread_Manager> and return
  // existing pointer.
  // DO NOT USE THIS METHOD. It may be unsupported in future releases.
  // Use ACE_Thread_Manager::instance() instead.

  static ACE_Allocator *alloc (void);
  // Get pointer to a default <ACE_Allocator>.
  // DO NOT USE THIS METHOD. It may be unsupported in future releases.
  // Use ACE_Allocator::instance() instead.

  static ACE_Allocator *alloc (ACE_Allocator *);
  // Set pointer to a process-wide <ACE_Allocator> and return existing
  // pointer.
  // DO NOT USE THIS METHOD. It may be unsupported in future releases.
  // Use ACE_Allocator::instance() instead.

  // = Utility methods.
  static int initialize (const ACE_Service_Type *,
                         ASYS_TCHAR parameters[]);
  // Dynamically link the shared object file and retrieve a pointer to
  // the designated shared object in this file.

  static int initialize (const ASYS_TCHAR svc_name[],
                         ASYS_TCHAR parameters[]);
  // Initialize and activate a statically <svc_name> service.

  static int resume (const ASYS_TCHAR svc_name[]);
  // Resume a <svc_name> that was previously suspended or has not yet
  // been resumed (e.g., a static service).

  static int suspend (const ASYS_TCHAR svc_name[]);
  // Suspend <svc_name>.  Note that this will not unlink the service
  // from the daemon if it was dynamically linked, it will mark it
  // as being suspended in the Service Repository and call the
  // suspend() member function on the appropriate ACE_Service_Object.
  // A service can be resumed later on by calling the RESUME()
  // member function...

  static int remove (const ASYS_TCHAR svc_name[]);
  // Totally remove <svc_name> from the daemon by removing it
  // from the ACE_Reactor, and unlinking it if necessary.

#if defined (ACE_HAS_WINCE)
  // We must provide these function to bridge the <Svc_Conf> parser
  // with ACE.
  static int initialize (const ACE_Service_Type *, char parameters[]);
  static int initialize (const char svc_name[], char parameters[]);
  static int resume (const char svc_name[]);
  static int suspend (const char svc_name[]);
  static int remove (const char svc_name[]);
#endif /* ACE_HAS_WINCE */

  void dump (void) const;
  // Dump the state of an object.

  static ACE_INLINE void signal_handler (ACE_Sig_Adapter *);
  // Set the signal_handler;for internal use by ACE_Object_Manager only.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  static int process_directive (const ASYS_TCHAR directive[]);
  // Process one service configuration <directive>, which is passed as
  // a string.  Returns the number of errors that occurred.

  static void handle_signal (int sig, siginfo_t *, ucontext_t *);
  // Handles signals to trigger reconfigurations.

protected:
  static int process_directives (void);
  // Process service configuration requests that are provided in the
  // svc.config file(s).  Returns the number of errors that occurred.

  static int process_commandline_directives (void);
  // Process service configuration requests that were provided on the
  // command-line.  Returns the number of errors that occurred.

  static int process_directives_i (void);
  // This is the implementation function that <process_directives> and
  // <process_directive> both call.  Returns the number of errors that
  // occurred.

  static int parse_args (int, ASYS_TCHAR *argv[]);
  // Handle the command-line options intended for the
  // <ACE_Service_Config>.  Note that <argv[0]> is assumed to be the
  // program name.

  static int start_daemon (void);
  // Become a daemon.

  static int load_static_svcs (void);
  // Add the default statically-linked services to the
  // <ACE_Service_Repository>.

private:
  static LPCTSTR logger_key_;
  // Where to write the logging output.

  static ACE_STATIC_SVCS *static_svcs_;
  // Singleton repository of statically linked services.

  static ACE_SVC_QUEUE *svc_queue_;
  // Queue of services specified on the command-line.

  static ACE_SVC_QUEUE *svc_conf_file_queue_;
  // Queue of svc.conf files specified on the command-line.
  // @@ This should probably be made to handle unicode filenames...

  static int init_svc_conf_file_queue (void);
  // Initialize the <svc_conf_file_queue_> if necessary.

  static sig_atomic_t reconfig_occurred_;
  // True if reconfiguration occurred.

  // = Set by command-line options.
  static char be_a_daemon_;
  // Shall we become a daemon process?

  static char no_static_svcs_;
  // Should we avoid loading the static services?

  static int signum_;
  // Number of the signal used to trigger reconfiguration.

  static ACE_Sig_Adapter *signal_handler_;
  // Handles the reconfiguration signals.
};

#if defined (__ACE_INLINE__)
#include "ace/Service_Config.i"
#endif /* __ACE_INLINE__ */

// These must go here to avoid circular includes...  (only left here
// for to not break applications which rely on this - no real need any
// longer)
#include "ace/Reactor.h"
#include "ace/Svc_Conf_Tokens.h"
#endif /* ACE_SERVICE_CONFIG_H */
