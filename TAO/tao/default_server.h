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

  virtual int open (void);
  // Call <open> for our strategies.

  // = Server-side ORB Strategy Factory Methods.
  virtual CREATION_STRATEGY *creation_strategy (void);
  // Return concrete creation strategy.

  virtual CONCURRENCY_STRATEGY *concurrency_strategy (void);

  virtual TAO_Active_Object_Map_Impl *create_active_object_map (int user_id_policy);
  // Return a new id-->sevant table.  If <user_id_policy> is true, the
  // request is being made for a POA with USER_ID policy. Otherwise,
  // the SYSTEM_ID policy is being used.

  virtual TAO_Reverse_Active_Object_Map_Impl *create_reverse_active_object_map (int unique_id_policy);
  // Return a new servant-->id table.  If <unique_id_policy> is true,
  // the request is being made for a POA with UNIQUE_ID
  // policy. Otherwise, the MULTIPLE_ID policy is being used.

  virtual ACE_Lock *create_poa_lock (void);
  // Creates and returns a lock for POA based on the setting for
  // <-ORBpoalock>.  A setting of <thread> returns an
  // <ACE_Lock_Adapter\<ACE_Thread_Mutex\>>; a setting of <null>
  // returns an <ACE_Lock_Adapter\<ACE_NULL_Mutex\>>.

  virtual ACE_Lock *create_poa_mgr_lock (void);
  // Creates and returns a lock for a POA Manager based on the setting
  // for <-ORBpoamgrlock>.  A setting of <thread> returns an
  // <ACE_Lock_Adapter\<ACE_Thread_Mutex\>>; a setting of <null>
  // returns an <ACE_Lock_Adapter\<ACE_NULL_Mutex\>>.

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

  virtual u_long active_object_map_size (void) const;
  // Return the object table size

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

  // = Helpers for <create_active_object_map>
  TAO_Active_Object_Map_Impl *create_user_id_policy_active_object_map (void);
  TAO_Active_Object_Map_Impl *create_system_id_policy_active_object_map (void);
  TAO_Active_Object_Map_Impl *create_active_object_map_i (TAO_Demux_Strategy table_type,
                                                          int user_id_policy);

  u_long thread_flags_;
  // Default thread flags passed to thr_create().

  u_long active_object_map_size_;
  // Default size of object lookup table.

  TAO_Demux_Strategy object_lookup_strategy_for_user_id_policy_;
  // The type of lookup/demultiplexing strategy being used for user id
  // policy

  TAO_Demux_Strategy object_lookup_strategy_for_system_id_policy_;
  // The type of lookup/demultiplexing strategy being used for system
  // id policy

  TAO_Demux_Strategy reverse_object_lookup_strategy_for_unique_id_policy_;
  // The type of reverse lookup/demultiplexing strategy being used for
  // the UNIQUE_ID policy

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

#if defined (TAO_USES_STATIC_SERVICE) || defined (TAO_PLATFORM_SVC_CONF_FILE_NOTSUP)
ACE_STATIC_SVC_DECLARE (TAO_Default_Server_Strategy_Factory)
#endif /* TAO_USE_STATIC_SERVICE || TAO_PLATFORM_SVC_CONF_FILE_NOTSUP */

ACE_FACTORY_DECLARE (TAO, TAO_Default_Server_Strategy_Factory)

#endif /* TAO_DEFAULT_SERVER_FACTORY_H */
