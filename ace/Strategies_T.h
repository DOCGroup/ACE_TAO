/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//   ACE_Strategies_T.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_STRATEGIES_T_H
#define ACE_STRATEGIES_T_H

#include "ace/Strategies.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "ace/Reactor.h"
#include "ace/Synch_Options.h"
#include "ace/Thread_Manager.h"
#include "ace/Hash_Map_Manager.h"

template<class SVC_HANDLER>
class ACE_Recycling_Strategy
{
  // = TITLE
  //     Defines the interface (and default implementation) for
  //     specifying a recycling strategy for a SVC_HANDLER.
  //
  // = DESCRIPTION
  //      Acts as a consular to the Svc_Handler, preparing it for the
  //      tough times ahead when the Svc_Handler will be recycled.
public:
  virtual ~ACE_Recycling_Strategy (void);
  // Virtual Destructor

  virtual int assign_recycler (SVC_HANDLER *svc_handler,
                               ACE_Connection_Recycling_Strategy *recycler,
                               const void *recycling_act);
  // Tell the Svc_Handler something about the recycler, so that it can
  // reach the recycler when necessary.

  virtual int prepare_for_recycling (SVC_HANDLER *svc_handler);
  // This allows us to prepare the svc_handler for recycling.
};

template <class SVC_HANDLER>
class ACE_Creation_Strategy
{
  // = TITLE
  //     Defines the interface for specifying a creation strategy for
  //     a SVC_HANDLER.
  //
  // = DESCRIPTION
  //     The default behavior is to make a new SVC_HANDLER.  However,
  //     subclasses can override this strategy to perform SVC_HANDLER
  //     creation in any way that they like (such as creating subclass
  //     instances of SVC_HANDLER, using a singleton, dynamically
  //     linking the handler, etc.).
public:
  // = Initialization and termination methods.

  ACE_Creation_Strategy (ACE_Thread_Manager * = 0);
  // Default constructor.

  int open (ACE_Thread_Manager * = 0);
  // A <Thread_Manager> is useful when creating active objects.

  virtual ~ACE_Creation_Strategy (void);

  // = Factory method.
  virtual int make_svc_handler (SVC_HANDLER *&sh);
  // Create a SVC_HANDLER with the appropriate creation strategy.  The
  // default behavior of this method is to make a new <SVC_HANDLER> if
  // <sh> == 0 (passing in the <Thread_Manager>), else <sh> is
  // unchanged.  Returns -1 on failure, else 0.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  ACE_Thread_Manager *thr_mgr_;
  // Pointer to a thread manager.
};

template <class SVC_HANDLER>
class ACE_Singleton_Strategy : public ACE_Creation_Strategy<SVC_HANDLER>
{
  // = TITLE
  //     Defines the interface for specifying a creation strategy for
  //     a <SVC_HANDLER> that always returns the same <SVC_HANDLER> (i.e.,
  //     it's a Singleton).
  //
  // = DESCRIPTION
  //     Note that this class takes over the ownership of the
  //     SVC_HANDLER passed into it as a parameter and it becomes
  //     responsible for deleting this object.
public:
  // = Initialization and termination methods.
  ACE_Singleton_Strategy (SVC_HANDLER * = 0,
                          ACE_Thread_Manager * = 0);
  int open (SVC_HANDLER *,
            ACE_Thread_Manager * = 0);
  virtual ~ACE_Singleton_Strategy (void);

  // = Factory method.
  virtual int make_svc_handler (SVC_HANDLER *&);
  // Create a Singleton SVC_HANDLER by always returning the same
  // SVC_HANDLER.  Returns -1 on failure, else 0.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  SVC_HANDLER *svc_handler_;
  // Pointer to the Singleton svc_handler.
};

template <class SVC_HANDLER>
class ACE_DLL_Strategy : public ACE_Creation_Strategy<SVC_HANDLER>
{
  // = TITLE
  //     Defines the interface for specifying a creation strategy for
  //     a SVC_HANDLER based on dynamic linking of the SVC_HANDLER.
public:
  // = Intialization and termination methods.

  ACE_DLL_Strategy (void);
  // "Do-nothing" constructor.

  ACE_DLL_Strategy (const char dll_name[],
                    const char factory_function[],
                    const char svc_name[],
                    ACE_Service_Repository *,
                    ACE_Thread_Manager * = 0);
  // Initialize the DLL strategy based upon the service's DLL
  // information contained in the <svc_dll_info> string.

  int open (const char dll_name[],
            const char factory_function[],
            const char svc_name[],
            ACE_Service_Repository *,
            ACE_Thread_Manager * = 0);
  // Initialize the DLL strategy based upon the service's DLL
  // information contained in the <svc_dll_info> string.

  // = Factory method.
  virtual int make_svc_handler (SVC_HANDLER *&);
  // Create a SVC_HANDLER by dynamically linking it from a DLL.
  // Returns -1 on failure, else 0.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  typedef ACE_Creation_Strategy<SVC_HANDLER> inherited;

  char dll_name_[MAXPATHLEN + 1];
  // Name of the DLL to dynamically link.

  char factory_function_[MAXPATHLEN + 1];
  // Name of the factory function in the shared library to use to
  // obtain a pointer to the new SVC_HANDLER.

  char svc_name_[MAXNAMELEN + 1];
  // Name of the service.

  ACE_Service_Repository *svc_rep_;
  // Pointer to the <Service_Repository>.
};

template <class SVC_HANDLER>
class ACE_Concurrency_Strategy
{
  // = TITLE
  //     Defines the interface for specifying a concurrency strategy
  //     for a SVC_HANDLER.
  //
  // = DESCRIPTION
  //     Default behavior is to activate the SVC_HANDLER by calling
  //     its open() method (which allows the SVC_HANDLER to define its
  //     own concurrency strategy).  However, subclasses can override
  //     this default strategy to do more sophisticated concurrency
  //     activations (such as creating the SVC_HANDLER as an active
  //     object via multi-threading or multi-processing).
public:
  ACE_Concurrency_Strategy (int flags = 0);
  // Constructor

  // = Factory method.
  virtual int activate_svc_handler (SVC_HANDLER *svc_handler,
                                    void *arg = 0);
  // Activate the <svc_handler> with an appropriate concurrency
  // strategy.  The default behavior of this method is to activate the
  // SVC_HANDLER by calling its open() method (which allows the
  // SVC_HANDLER to define its own concurrency strategy).

  virtual ~ACE_Concurrency_Strategy (void);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:

  int flags_;
  // Flags that are parsed to set options for the connected
  // <SVC_HANDLER>.
};

template <class SVC_HANDLER>
class ACE_Reactive_Strategy : public ACE_Concurrency_Strategy <SVC_HANDLER>
{
  // = TITLE
  //     Defines the interface for specifying a Reactive concurrency
  //     strategy for a SVC_HANDLER.
  //
  // = DESCRIPTION
  //     This class provides a strategy that registers the
  //     <SVC_HANDLER> with a <Reactor>.
public:
  // = Intialization and termination methods.
  ACE_Reactive_Strategy (int flags = 0);
  // "Do-nothing constructor"

  ACE_Reactive_Strategy (ACE_Reactor *reactor,
                         ACE_Reactor_Mask = ACE_Event_Handler::READ_MASK,
                         int flags = 0);
  // Initialize the strategy.

  virtual int open (ACE_Reactor *reactor,
                    ACE_Reactor_Mask = ACE_Event_Handler::READ_MASK,
                    int flags = 0);
  // Initialize the strategy.

  virtual ~ACE_Reactive_Strategy (void);
  // Destructor.

  // = Factory method.
  virtual int activate_svc_handler (SVC_HANDLER *svc_handler,
                                    void *arg = 0);
  // Activate the <svc_handler> by registering it with the <Reactor>
  // and then calling it's <open> hook.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  typedef ACE_Concurrency_Strategy<SVC_HANDLER> inherited;

  ACE_Reactor *reactor_;
  // Pointer to the Reactor we'll use to register the <SVC_HANDLER>.

  ACE_Reactor_Mask mask_;
  // The mask that we pass to the <Reactor> when we register the
  // <SVC_HANDLER>.
};

template <class SVC_HANDLER>
class ACE_Thread_Strategy : public ACE_Concurrency_Strategy<SVC_HANDLER>
{
  // = TITLE
  //     Defines the interface for specifying a concurrency strategy
  //     for a <SVC_HANDLER> based on multithreading.
  //
  // = DESCRIPTION
  //     This class provides a strategy that manages the creation of
  //     threads to handle requests from clients concurrently.  It
  //     behaves as a "thread factory", spawning threads "on-demand"
  //     to run the service specified by a user-supplied
  //     <SVC_HANDLER>.
public:
  // = Intialization and termination methods.
  ACE_Thread_Strategy (int flags = 0);
  // "Do-nothing constructor"

  ACE_Thread_Strategy (ACE_Thread_Manager *tm,
                       long thr_flags,
                       size_t n_threads = 1,
                       int flags = 0);
  // Initialize the strategy.

  virtual int open (ACE_Thread_Manager *tm,
                    long thr_flags,
                    size_t n_threads = 1,
                    int flags = 0);
  // Initialize the strategy.

  virtual ~ACE_Thread_Strategy (void);

  // = Factory method.
  virtual int activate_svc_handler (SVC_HANDLER *svc_handler,
                                    void *arg = 0);
  // Activate the <svc_handler> with an appropriate concurrency
  // strategy.  This method activates the SVC_HANDLER by first calling
  // its open() method and then calling its activate() method to turn
  // it into an active object.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  typedef ACE_Concurrency_Strategy<SVC_HANDLER> inherited;

  ACE_Thread_Manager *thr_mgr_;
  // Thread manager for this class (must be provided).

  long thr_flags_;
  // Flags to pass into the SVC_HANDLER::activate() method.

  size_t n_threads_;
  // Number of threads to spawn.
};

template <class SVC_HANDLER>
class ACE_Process_Strategy : public ACE_Concurrency_Strategy<SVC_HANDLER>
{
  // = TITLE
  //     Defines the interface for specifying a concurrency strategy
  //     for a <SVC_HANDLER> based on multiprocessing.
  //
  // = DESCRIPTION
  //     This class provides a strategy that manages the creation of
  //     processes to handle requests from clients concurrently.  It
  //     behaves as a "process factory", using <ACE::fork> to fork
  //     threads "on-demand" to run the service specified by a
  //     user-supplied <SVC_HANDLER> in a separate process.
public:
  // = Intialization and termination methods.

  ACE_Process_Strategy (size_t n_processes = 1,
                        ACE_Event_Handler *acceptor = 0,
                        ACE_Reactor * = 0,
                        int avoid_zombies = 0);
  // Initialize the strategy.  If <avoid_zombies> is non-0 then set a
  // flag to <ACE::fork> to avoid zombies.

  virtual int open (size_t n_processes = 1,
                    ACE_Event_Handler *acceptor = 0,
                    ACE_Reactor * = 0,
                    int avoid_zombies = 0);
  // Initialize the strategy.  If <avoid_zombies> is non-0 then set a
  // flag to <ACE::fork> to avoid zombies.

  virtual ~ACE_Process_Strategy (void);

  // = Factory method.
  virtual int activate_svc_handler (SVC_HANDLER *svc_handler,
                                    void *arg = 0);
  // Activate the <svc_handler> with an appropriate concurrency
  // strategy.  This method activates the SVC_HANDLER by first forking
  // and then calling the open() method of the SVC_HANDLER in the
  // child.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  typedef ACE_Concurrency_Strategy<SVC_HANDLER> inherited;

  size_t n_processes_;
  // Number of processes to spawn.

  ACE_Event_Handler *acceptor_;
  // This is the <Acceptor> in the parent is listening on.  We need to
  // make sure that we remove it from the Reactor and close it down in
  // the child.

  ACE_Reactor *reactor_;
  // This is the <Reactor> the child is using in conjunction with the
  // <Acceptor>.  We need to remove the <Acceptor> from this <Reactor>
  // in the child.
};

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1>
class ACE_Accept_Strategy
{
  // = TITLE
  //     Defines the interface for specifying a passive connection
  //     acceptance strategy for a SVC_HANDLER.
  //
  // = DESCRIPTION
  //     This class provides a strategy that manages passive
  //     connection acceptance of a client.
public:
  // = Initialization and termination methods.
  ACE_Accept_Strategy (ACE_Reactor *reactor = ACE_Reactor::instance ());
  // Default constructor.

  ACE_Accept_Strategy (const ACE_PEER_ACCEPTOR_ADDR &local_addr,
                       int restart = 0,
                       ACE_Reactor *reactor = ACE_Reactor::instance ());
  // Initialize the <peer_acceptor_> with <local_addr>.

  virtual int open (const ACE_PEER_ACCEPTOR_ADDR &local_addr,
                    int restart = 0);
  // Initialize the <peer_acceptor_> with <local_addr>.

  virtual ACE_HANDLE get_handle (void) const;
  // Return the underlying ACE_HANDLE of the <peer_acceptor_>.

  virtual ACE_PEER_ACCEPTOR &acceptor (void) const;
  // Return a reference to the <peer_acceptor_>.

  virtual ~ACE_Accept_Strategy (void);

  // = Factory method.
  virtual int accept_svc_handler (SVC_HANDLER *);
  // The default behavior delegates to the <accept> method of the
  // PEER_ACCEPTOR.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  ACE_PEER_ACCEPTOR acceptor_;
  // Factory that establishes connections passively.

  ACE_Reactor *reactor_;
  // Pointer to the reactor used by the Acceptor.
};

template <class SVC_HANDLER, ACE_PEER_CONNECTOR_1>
class ACE_Connect_Strategy
{
  // = TITLE
  //     Defines the interface for specifying an active
  //     connection establishment strategy for a SVC_HANDLER.
  //
  // = DESCRIPTION
  //     This class provides a strategy that manages active
  //     connection establishment to a server.
public:
  // = Initialization and termination methods.
  ACE_Connect_Strategy (void);
  // Default constructor.

  virtual ACE_PEER_CONNECTOR &connector (void) const;
  // Return a reference to the <peer_connector_>.

  virtual ~ACE_Connect_Strategy (void);

  // = Factory method.
  virtual int connect_svc_handler (SVC_HANDLER *&sh,
                                   const ACE_PEER_CONNECTOR_ADDR &remote_addr,
                                   ACE_Time_Value *timeout,
                                   const ACE_PEER_CONNECTOR_ADDR &local_addr,
                                   int reuse_addr,
                                   int flags,
                                   int perms);
  // The default behavior delegates to the <connect> method of the
  // <PEER_CONNECTOR::connect>.

  virtual int connect_svc_handler (SVC_HANDLER *&sh,
                                   SVC_HANDLER *&sh_copy,
                                   const ACE_PEER_CONNECTOR_ADDR &remote_addr,
                                   ACE_Time_Value *timeout,
                                   const ACE_PEER_CONNECTOR_ADDR &local_addr,
                                   int reuse_addr,
                                   int flags,
                                   int perms);
  // The default behavior delegates to the <connect> method of the
  // <PEER_CONNECTOR::connect>.
  // Please check the documentation in Connector.h for more details.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  ACE_PEER_CONNECTOR connector_;
  // Factory that establishes connections actively.
};

template <class SVC_HANDLER>
class ACE_Scheduling_Strategy
{
  // = TITLE
  //     Defines the interface for specifying how to suspend and
  //     resume a service .
  //
  // = DESCRIPTION
  //     This class provides a strategy that allows arbitrarily
  //     sophisticated service suspension and resumption.  The default
  //     behavior is to do nothing...
public:
  // = Initialization and termination methods.

  ACE_Scheduling_Strategy (SVC_HANDLER * = 0);
  // Constructor

  virtual ~ACE_Scheduling_Strategy (void);
  // Destructor

  // = Scheduling methods

  virtual int suspend (void);
  // Suspend hook.

  virtual int resume (void);
  // Resume hook.

  virtual void dump (void) const;
  // Dump the state of the object.
};

template <class SVC_HANDLER>
class ACE_Schedule_All_Reactive_Strategy : public ACE_Scheduling_Strategy<SVC_HANDLER>
{
  // = TITLE
  //     Defines the interface for specifying how to suspend and
  //     resume a single-threaded reactive service .
  //
  // = DESCRIPTION
  //     This class provides a strategy that suspends and resumes all
  //     the Event_Handlers in a Reactor in one fell swoop.
public:
  // = Initialization and termination methods.
  ACE_Schedule_All_Reactive_Strategy (SVC_HANDLER * = 0);
  // Constructor

  // = Scheduling methods

  virtual int suspend (void);
  // Suspend hook.

  virtual int resume (void);
  // Resume hook.

  virtual void dump (void) const;
  // Dump the state of the object.

protected:

  ACE_Reactor *reactor_;
  // Thread Manager
};

template <class SVC_HANDLER>
class ACE_Schedule_All_Threaded_Strategy : public ACE_Scheduling_Strategy<SVC_HANDLER>
{
  // = TITLE
  //     Defines the interface for specifying how to suspend and
  //     resume a multithreaded service .
  //
  // = DESCRIPTION
  //     This class provides a strategy that suspends and resumes all
  //     the Event_Handlers controlled by a Thread_Manager in one fell swoop.
public:
  // = Initialization and termination methods.
  ACE_Schedule_All_Threaded_Strategy (SVC_HANDLER * = 0);
  // Constructor

  // = Scheduling methods

  virtual int suspend (void);
  // Suspend hook.

  virtual int resume (void);
  // Resume hook.

  virtual void dump (void) const;
  // Dump the state of the object.

protected:

  ACE_Thread_Manager *thr_mgr_;
  // Thread Manager
};

template <class SVC_HANDLER>
class ACE_NOOP_Creation_Strategy : public ACE_Creation_Strategy<SVC_HANDLER>
{
  // = TITLE
  //     Implements a no-op creation strategy in order to defer
  //     decisions regarding creation to some later point in time, such
  //     as in connect or accept strategy.
  //
  // = DESCRIPTION
  //     An example of the use of this is in the
  //     <ACE_Cached_Connect_Strategy>, which only returns a single
  //     connection for a given endpoint.
public:
  virtual int make_svc_handler (SVC_HANDLER *&);
  // This is a no-op.
};

template <class SVC_HANDLER>
class ACE_NOOP_Concurrency_Strategy : public ACE_Concurrency_Strategy<SVC_HANDLER>
{
  // = TITLE
  //     Implements a no-op activation strategy in order to avoid
  //     calling open on a svc_handler multiple times.
  //
  // = DESCRIPTION
  //     An example of the use of this is in the
  //     <ACE_Cached_Connect_Strategy>, which reuses svc_handlers.
  //     Therefore we don't want to call open on the recycled
  //     svc_handler more than once.
public:
  // = Factory method.
  virtual int activate_svc_handler (SVC_HANDLER *svc_handler,
                                    void *arg = 0);
  // This is a no-op.
};

template <class ADDR_T>
class ACE_Hash_Addr
{
  // = TITLE
  //     Internal class to compute hash values on addresses in
  //     <ACE_Cached_Connect_Strategy>.
  //
  // = DESCRIPTION
  //     Intended to be used as a key to an <ACE_Hash_Map_Manager>.
  //     <ADDR_T> parameter/subclass is typically <ACE_INET_Addr>.  It
  //     is expected to implement operator==().
public:
  // = Initialization methods.
  ACE_Hash_Addr (void);
  // Default constructor.

  ACE_Hash_Addr (const ADDR_T &a);
  // Pre-compute hash value.

  ~ACE_Hash_Addr (void);
  // Destructor.

  u_long hash (void) const;
  // Computes and returns hash value.  This "caches" the hash value to
  // improve performance.

  int operator== (const ACE_Hash_Addr<ADDR_T> &rhs) const;
  // Compares two hash values.

private:
  u_long hash_i (const ADDR_T &) const;
  // This is the method that actually performs the non-cached hash
  // computation.  It should typically be specialized.

  u_long hash_value_;
  // Pre-computed hash-value.

  ADDR_T addr_;
  // The underlying address.
};

template <class T>
class ACE_Refcounted_Hash_Recyclable :  public ACE_Refcountable,
                                        public ACE_Hashable,
                                        public ACE_Recyclable
{
public:
  ACE_Refcounted_Hash_Recyclable (void);
  // Default constructor.

  ACE_Refcounted_Hash_Recyclable (const T &t,
                                  int refcount = 0,
                                  ACE_Recyclable::State state = ACE_Recyclable::UNKNOWN);
  // Constructor.

  virtual ~ACE_Refcounted_Hash_Recyclable (void);
  // Destructor

  int operator== (const ACE_Refcounted_Hash_Recyclable<T> &rhs) const;
  // Compares two instances.

  int operator== (const T &rhs) const;
  // Compares two instances.

protected:
  u_long hash_i (void) const;
  // Computes and returns hash value.

  T t_;
};

template <class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class MUTEX>
class ACE_Cached_Connect_Strategy : public ACE_Connection_Recycling_Strategy, public ACE_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2>
{
  // = TITLE
  //     A connection strategy which caches connections to peers
  //     (represented by <SVC_HANDLER> instances), thereby allowing
  //     subsequent re-use of unused, but available, connections.
  //
  // = DESCRIPTION
  //     <ACE_Cached_Connect_Strategy> is intended to be used as a
  //     plug-in connection strategy for <ACE_Strategy_Connector>.
  //     It's added value is re-use of established connections.
  //
  // = USAGE
  //     In order to use this appropriately, the user must provide
  //     a template specialization for <ACE_Hash_Addr::compare_i()> and
  //     <ACE_Hash_Addr::hash_i()> based on the address type and the
  //     service handler type.  For example, a specialization using
  //     <ACE_INET_Addr> and <My_Service_Handler> might be:
  //     = BEGIN<NOFILL>
  //     = BEGIN<CODE>
  //     size_t
  //     ACE_Hash_Addr<ACE_INET_Addr, My_Service_Handler>::hash_i(const ACE_INET_Addr &a)
  //     {
  //       return ...;
  //     }
  //     = END<CODE>
  //     = END<NOFILL>
  //
  // = SEE ALSO
  //     <ACE_Hash_Addr>.
public:

  ACE_Cached_Connect_Strategy (ACE_Creation_Strategy<SVC_HANDLER> *cre_s = 0,
                               ACE_Concurrency_Strategy<SVC_HANDLER> *con_s = 0,
                               ACE_Recycling_Strategy<SVC_HANDLER> *rec_s = 0);
  // Constructor

  virtual ~ACE_Cached_Connect_Strategy (void);
  // Destructor

  virtual int open (ACE_Creation_Strategy<SVC_HANDLER> *cre_s,
                    ACE_Concurrency_Strategy<SVC_HANDLER> *con_s,
                    ACE_Recycling_Strategy<SVC_HANDLER> *rec_s);
  // This methods allow you to change the strategies used by the
  // cached connector.

  virtual int make_svc_handler (SVC_HANDLER *&sh);
  // Template method for making a new <svc_handler>

  virtual int activate_svc_handler (SVC_HANDLER *svc_handler);
  // Template method for activating a new <svc_handler>

  virtual int assign_recycler (SVC_HANDLER *svc_handler,
                               ACE_Connection_Recycling_Strategy *recycler,
                               const void *recycling_act);
  // Template method for setting the recycler information of the
  // svc_handler.

  virtual int prepare_for_recycling (SVC_HANDLER *svc_handler);
  // Template method for preparing the svc_handler for recycling.

  virtual int connect_svc_handler (SVC_HANDLER *&sh,
                                   const ACE_PEER_CONNECTOR_ADDR &remote_addr,
                                   ACE_Time_Value *timeout,
                                   const ACE_PEER_CONNECTOR_ADDR &local_addr,
                                   int reuse_addr,
                                   int flags,
                                   int perms);
  virtual int connect_svc_handler (SVC_HANDLER *&sh,
                                   SVC_HANDLER *&sh_copy,
                                   const ACE_PEER_CONNECTOR_ADDR &remote_addr,
                                   ACE_Time_Value *timeout,
                                   const ACE_PEER_CONNECTOR_ADDR &local_addr,
                                   int reuse_addr,
                                   int flags,
                                   int perms);
  // Checks to see if there is already a <SVC_HANDLER> in the cache
  // connected to the <remote_addr>.  If so, we return this pointer.
  // Otherwise we establish the connection, put it into the cache, and
  // return the <SVC_HANDLER> pointer.  <[NOTE]>: the <{reuse_addr}>
  // argument does NOT control re-use of addresses in the cache.
  // Rather, if the underlying protocol requires a "dead time" prior
  // to re-use of its addresses (TCP is a classic example of this),
  // <{and}> the protocol provides a means by which to defeat the dead
  // time, setting this argument to non-zero will defeat the dead-time
  // requirement.  <{Dev. Note: We might want to consider enhancing
  // the interface at some point so that this also controls re-use of
  // the cache.}>

  virtual int purge (const void *recycling_act);
  // Remove from cache.

  virtual int cache (const void *recycling_act);
  // Add to cache.

  virtual int mark_as_closed (const void *recycling_act);
  // Mark as closed.

  virtual int cleanup_hint (const void *recycling_act);
  // Cleanup hint.

  // = Define some useful typedefs.
  typedef ACE_Creation_Strategy<SVC_HANDLER>
          CREATION_STRATEGY;
  typedef ACE_Concurrency_Strategy<SVC_HANDLER>
          CONCURRENCY_STRATEGY;
  typedef ACE_Recycling_Strategy<SVC_HANDLER>
          RECYCLING_STRATEGY;

  // = Super class
  typedef ACE_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2>
          CONNECT_STRATEGY;

  // = Typedefs for managing the map
  typedef ACE_Refcounted_Hash_Recyclable<ACE_PEER_CONNECTOR_ADDR>
          REFCOUNTED_HASH_RECYCLABLE_ADDRESS;
  typedef ACE_Hash_Map_Manager <REFCOUNTED_HASH_RECYCLABLE_ADDRESS, SVC_HANDLER *, ACE_Null_Mutex>
          CONNECTION_MAP;
  typedef ACE_Hash_Map_Iterator <REFCOUNTED_HASH_RECYCLABLE_ADDRESS, SVC_HANDLER *, ACE_Null_Mutex>
          CONNECTION_MAP_ITERATOR;
  typedef ACE_Hash_Map_Entry<REFCOUNTED_HASH_RECYCLABLE_ADDRESS, SVC_HANDLER *>
          CONNECTION_MAP_ENTRY;

  // = Strategy accessors
  virtual ACE_Creation_Strategy<SVC_HANDLER> *creation_strategy (void) const;
  virtual ACE_Recycling_Strategy<SVC_HANDLER> *recycling_strategy (void) const;
  virtual ACE_Concurrency_Strategy<SVC_HANDLER> *concurrency_strategy (void) const;

protected:

  virtual int purge_i (const void *recycling_act);
  // Remove from cache (non-locking version).

  virtual int cache_i (const void *recycling_act);
  // Add to cache (non-locking version).

  virtual int mark_as_closed_i (const void *recycling_act);
  // Mark as closed (non-locking version).

  virtual int cleanup_hint_i (const void *recycling_act);
  // Cleanup hint.

  // = Helpers
  int check_hint_i (SVC_HANDLER *&sh,
                    const ACE_PEER_CONNECTOR_ADDR &remote_addr,
                    ACE_Time_Value *timeout,
                    const ACE_PEER_CONNECTOR_ADDR &local_addr,
                    int reuse_addr,
                    int flags,
                    int perms,
                    ACE_Hash_Map_Entry<ACE_Refcounted_Hash_Recyclable<ACE_PEER_CONNECTOR_ADDR>, SVC_HANDLER *> *&entry,
                    int &found);

  int connect_svc_handler_i (SVC_HANDLER *&sh,
                             const ACE_PEER_CONNECTOR_ADDR &remote_addr,
                             ACE_Time_Value *timeout,
                             const ACE_PEER_CONNECTOR_ADDR &local_addr,
                             int reuse_addr,
                             int flags,
                             int perms,
                             int &found);

  int find_or_create_svc_handler_i (SVC_HANDLER *&sh,
                                    const ACE_PEER_CONNECTOR_ADDR &remote_addr,
                                    ACE_Time_Value *timeout,
                                    const ACE_PEER_CONNECTOR_ADDR &local_addr,
                                    int reuse_addr,
                                    int flags,
                                    int perms,
                                    ACE_Hash_Map_Entry<ACE_Refcounted_Hash_Recyclable<ACE_PEER_CONNECTOR_ADDR>, SVC_HANDLER *> *&entry,
                                    int &found);

  CONNECTION_MAP connection_cache_;
  // Table that maintains the cache of connected <SVC_HANDLER>s.

  MUTEX lock_;
  // Mutual exclusion for this object.

  // = Strategy objects.

  CREATION_STRATEGY *creation_strategy_;
  // Creation strategy for an <Connector>.

  int delete_creation_strategy_;
  // 1 if <Connector> created the creation strategy and thus should
  // delete it, else 0.

  CONCURRENCY_STRATEGY *concurrency_strategy_;
  // Concurrency strategy for an <Connector>.

  int delete_concurrency_strategy_;
  // 1 if <Connector> created the concurrency strategy and thus should
  // delete it, else 0.

  RECYCLING_STRATEGY *recycling_strategy_;
  // Recycling strategy for an <Connector>.

  int delete_recycling_strategy_;
  // 1 if <Connector> created the recycling strategy and thus should
  // delete it, else 0.
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/Strategies_T.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Strategies_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Strategies_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_STRATEGIES_T_H */
