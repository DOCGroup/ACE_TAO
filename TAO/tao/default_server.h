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

class TAO_Default_Server_Strategy_Factory : public TAO_Server_Strategy_Factory
{
  // = TITLE
  //   This is the default strategy factory for CORBA servers.  It
  //   allows developers to choose strategies via argument flags.
  //   This design gives substantial freedom for experimentation.
  //
  // = DESCRIPTION


public:
  // = Initialization and termination methods.
  TAO_Default_Server_Strategy_Factory (void);
  virtual ~TAO_Default_Server_Strategy_Factory (void);

  // = Service Configurator hooks.
  virtual int init (int argc, char *argv[]);

  // = The TAO_Server_Strategy_Factory methods, please read the
  //   documentation in "tao/Server_Strategy_Factory.h"
  virtual int open (TAO_ORB_Core*);
  virtual int enable_poa_locking (void);
  virtual int activate_server_connections (void);
  virtual int server_connection_thread_flags (void);
  virtual int server_connection_thread_count (void);

  virtual ACE_Lock *create_servant_lock (void);
  // The servant lock is based on the setting of POA, and concurrency
  // strategy as follows:
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
  // If the ORB is single threaded or some form of ORB-per-thread then
  // it is more efficient to use a Null_Mutex for the variables
  // controlling the event loop (termination). Otherwise a
  // Recursive_Thread_Mutex or Thread_Mutex may be required.
  virtual ACE_Lock *create_cached_connector_lock (void);

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

protected:
  int activate_server_connections_;
  // Should the server connection handlers run in their own thread?

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

  Lock_Type cached_connector_lock_type_;
  // Type of lock used by the cached connector.
};

#if defined (__ACE_INLINE__)
# include "tao/default_server.i"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE (TAO_Default_Server_Strategy_Factory)
ACE_FACTORY_DECLARE (TAO, TAO_Default_Server_Strategy_Factory)

#endif /* TAO_DEFAULT_SERVER_FACTORY_H */
