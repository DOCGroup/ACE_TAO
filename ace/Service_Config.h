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

#if !defined (ACE_SERVICE_CONFIG_H)
#define ACE_SERVICE_CONFIG_H

#include "ace/Service_Object.h"
#include "ace/Signal.h"
#include "ace/Containers.h"

// Forward decl.
class ACE_Service_Repository;
class ACE_Service_Type;
class ACE_Allocator;
class ACE_Reactor;
class ACE_Thread_Manager;

extern "C"
{
typedef ACE_Service_Object *(*ACE_SERVICE_ALLOCATOR)(void);
}

struct ACE_Static_Svc_Descriptor
{
  char *name_;
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

// = Maintain a set of the statically linked service descriptor.

typedef ACE_Unbounded_Set<ACE_Static_Svc_Descriptor *> ACE_STATIC_SVCS;
typedef ACE_Unbounded_Set_Iterator<ACE_Static_Svc_Descriptor *> ACE_STATIC_SVCS_ITERATOR;

class ACE_Export ACE_Service_Config
  // = TITLE
  //     Supplies common server operations for dynamic and static
  //     configuration of services.
{
public:
  enum {MAX_SERVICES = ACE_DEFAULT_SELECT_REACTOR_SIZE};

  // = Initialization and termination methods.

  ACE_Service_Config (int ignore_static_svcs = 0, 
		      size_t size = ACE_Service_Config::MAX_SERVICES, 
		      int signum = SIGHUP);
  // Initialize the Service Repository.

  ACE_Service_Config (const char program_name[]);
  // Performs an open without parsing command-line arguments.

  static int open (const char program_name[]);
  // Performs an open without parsing command-line arguments.
  // @@ What is its return value?

  static int open (int argc, char *argv[]);
  // This is the primary entry point into the ACE_Service_Config (the
  // constructor just handles simple initializations).  It parses
  // arguments passed in from the command-line.
  // @@ What is its return value?

  virtual ~ACE_Service_Config (void);
  // Perform user-specified close activities and remove dynamic
  // memory.

  static int close (void);
  // Tidy up and perform last rites when ACE_Service_Config is shut
  // down.  This method calls <close_svcs> and <close_singletons>.
  // @@ What is its return value?

  static int close_svcs (void);
  // Perform user-specified close hooks on all of the configured
  // services in the <Service_Repository>, then delete the
  // <Service_Repository> itself.
  // @@ What is its return value?

  static int close_singletons (void); 
  // Delete the dynamically allocated Singletons (i.e., the <Reactor>,
  // <Proactor>, <ReactorEx>, <Thread_Manager>, and <Allocator>).
  // @@ What is its return value?

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

  static sig_atomic_t reactor_event_loop_done (void);
  // Report if the Reactor's event loop is finished.
  // DO NOT USE THIS METHOD. It may be unsupported in future releases.
  // Use ACE_Reactor::event_loop_done() instead.

  static sig_atomic_t reconfig_occurred (void);
  // True if reconfiguration occurred.

  static void reconfig_occurred (sig_atomic_t);
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

  // = Member functions used by various other parts
  //   of the Service Configurator class category.
  static int initialize (const ACE_Service_Type *, char parameters[]);
  // Dynamically link the shared object file and retrieve
  // a pointer to the designated shared object in this file. 

  static int initialize (const char svc_name[], char parameters[]);
  // Initialize and activate a statically <svc_name> service. 

  static int resume (const char svc_name[]);
  // Resume a <svc_name> that was previously suspended or has not yet
  // been resumed (e.g., a static service). 

  static int suspend (const char svc_name[]);
  // Suspend <svc_name>.  Note that this will not unlink the service 
  // from the daemon if it was dynamically linked, it will mark it 
  // as being suspended in the Service Repository and call the
  // suspend() member function on the appropriate ACE_Service_Object.  
  // A service can be resumed later on by calling the RESUME() 
  // member function... 

  static int remove (const char svc_name[]);
  // Totally remove <svc_name> from the daemon by removing it 
  // from the ACE_Reactor, and unlinking it if necessary.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  static int process_directives (void);
  // Process service configuration requests as indicated in the
  // <service_config_file>.  Returns -1 if errors occur, else 0.

  static void parse_args (int, char *argv[]);
  // Handle the command-line options intended for the
  // <ACE_Service_Config>.  Note that <argv[0]> is assumed to be the
  // program name.

  static int start_daemon (void);
  // Become a daemon. 

  static int load_static_svcs (void);
  // Add the default statically-linked services to the <ACE_Service_Repository>.

  static void handle_signal (int sig, siginfo_t *, ucontext_t *);
  // Handles signals to trigger reconfigurations.

private:
  static const char *service_config_file_;
  // Name of service configuration file.

  static LPCTSTR logger_key_;
  // Where to write the logging output.

  //static ACE_Static_Svc_Descriptor service_list_[];
  // List of statically linked services.

  static ACE_STATIC_SVCS *static_svcs_;
  // Singleton repository of statically linked services.

  static sig_atomic_t reconfig_occurred_;
  // True if reconfiguration occurred.

  // = Set by command-line options. 
  static char debug_;
  static char be_a_daemon_;
  static char no_static_svcs_;

  static int signum_;
  // Number of the signal used to trigger reconfiguration.

  static ACE_Sig_Adapter signal_handler_;
  // Handles the reconfiguration signals.
};

#if defined (__ACE_INLINE__)
#include "ace/Service_Config.i"
#endif /* __ACE_INLINE__ */

// These must go here to avoid circular includes...
// (only left here for to not break applications 
//  which rely on this - no real need any longer)
#include "ace/Reactor.h"
#include "ace/Svc_Conf_Tokens.h"
#endif /* ACE_SERVICE_CONFIG_H */
