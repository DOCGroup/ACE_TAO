// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     default_server.h
//
// = AUTHOR
//     Chris Cleeland
//
// ============================================================================

#ifndef TAO_DEFAULT_SERVER_FACTORY_H
#  define TAO_DEFAULT_SERVER_FACTORY_H

#include "tao/corbafwd.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/params.h"
#include "tao/ORB_Strategies_T.h"

class TAO_Default_Server_Creation_Strategy : public TAO_Server_Strategy_Factory::CREATION_STRATEGY
{
public:
  TAO_Default_Server_Creation_Strategy (ACE_Thread_Manager * = 0);
  // ctor

  // = Factory method.
  virtual int make_svc_handler (TAO_Server_Connection_Handler *&sh);
};

class TAO_Default_Server_Strategy_Factory : public TAO_Server_Strategy_Factory
{
  // = TITLE
  //   This is the default strategy factory for CORBA servers.  It
  //   allows developers to choose strategies via argument flags.
  //   This design gives substantial freedom for experimentation.
public:
  // = Initialization and termination methods.
  TAO_Default_Server_Strategy_Factory (void);
  virtual ~TAO_Default_Server_Strategy_Factory (void);

  virtual int open (TAO_ORB_Core*);
  // Call <open> for our strategies.

  // = Server-side ORB Strategy Factory Methods.
  virtual CREATION_STRATEGY *creation_strategy (void);
  // Return concrete creation strategy.

  virtual CONCURRENCY_STRATEGY *concurrency_strategy (void);

  virtual int enable_poa_locking (void);
  // Enable POA locking?

  virtual ACE_Lock *create_servant_lock (void);
  // Creates and returns a lock for servants based on the setting of
  // POA, and concurrency strategy as follows:
  // 1. If concurrency policy is reactive and POA is TSS
  //    then return ACE_Null_Mutex via ACE_Lock_Adapter.
  //
  // 2. If concurrency policy is non-reactive then
  //    return ACE_Thread_Mutex ...
  //
  // 3. If the POA is global then, return
  //    ACE_Null_Mutex iff ORB_init count == 1,
  //    else if ORB_init count > 1 return
  //    ACE_Thread_Mutex.

  virtual ACE_Lock *create_event_loop_lock (void);
  // Creates and returns a lock for the event loop.
  // If the ORB is single threaded or some form of ORB-per-thread then
  // it is more efficient to use a Null_Mutex for the variables
  // controlling the event loop (termination). Otherwise a
  // Recursive_Thread_Mutex or Thread_Mutex may be required.

  virtual ACE_Lock *create_collocation_table_lock (void);
  // Creates and returns the lock for the global collocation table.

  virtual ACE_Lock *create_cached_connector_lock (void);
  // Creates and returns the lock for the global collocation table.

  // = Service Configurator hooks.
  virtual int init (int argc, char *argv[]);
  // Initialize the ORB when it's linked dynamically.

  int parse_args (int argc, char *argv[]);
  // Arguments are in the form of -ORBxxx.  Valid arguments are:
  // <-ORBconcurrency> <{which}>
  //   where <{which}> is one of <reactive> or <thread-per-connection>.
  // <-ORBtablesize> <{num}>
  //   to set the table size
  // <-ORBdemuxstrategy> <{which}>
  //   where <{which}> is one of <dynamic>, <linear>, or <active>
  // <-ORBpoalock> <{which}>
  //   where <{which}> is one of <thread> or <null> (default <thread>)
  // <-ORBpoamgrlock> <{which}>
  //   where <{which}> is one of <thread> or <null> (default <thread>)
  // <-ORBeventlock> <{which}>
  //   where <{which}> is one of <thread> or <null> (default <null>)

protected:
  void tokenize (char *flag_string);

  u_long thread_flags_;
  // Default thread flags passed to thr_create().

  enum Lock_Type
  {
    TAO_NULL_LOCK,
    TAO_THREAD_LOCK
  };

  Lock_Type poa_lock_type_;
  // The type of lock to be returned by <create_poa_lock()>.

  Lock_Type poa_mgr_lock_type_;
  // The type of lock to be returned by <create_poa_mgr_lock()>.

  Lock_Type event_loop_lock_type_;
  // The type of lock to be returned by <create_event_loop_lock()>.

  Lock_Type collocation_table_lock_type_;
  // Type of lock used by the collocation table.

  Lock_Type cached_connector_lock_type_;
  // Type of lock used by the cached connector.

  // = Strategies Used.
  TAO_Reactive_Strategy<TAO_Server_Connection_Handler> reactive_strategy_;
  // A strategy for passively establishing connections which utilizes
  // the Reactor.

  ACE_Thread_Strategy<TAO_Server_Connection_Handler> threaded_strategy_;
  // The threaded strategy used for passively establishing
  // connections.

  CREATION_STRATEGY *creation_strategy_;
  // concrete creation strategy.

  CONCURRENCY_STRATEGY *concurrency_strategy_;
  // concrete concurrency strategy.

  TAO_Default_Server_Creation_Strategy default_creation_strategy_;
  // The default creation strategy for the server side connection
  // handlers

#if 0
  // Someday we'll need these!
  ACCEPT_STRATEGY *accept_strategy_;
  SCHEDULING_STRATEGY *scheduling_strategy_;
#endif /* 0 */
};

#if defined (__ACE_INLINE__)
# include "tao/default_server.i"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE (TAO_Default_Server_Strategy_Factory)
ACE_FACTORY_DECLARE (TAO, TAO_Default_Server_Strategy_Factory)

#endif /* TAO_DEFAULT_SERVER_FACTORY_H */
