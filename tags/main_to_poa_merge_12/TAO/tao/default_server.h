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

#if !defined (TAO_DEFAULT_SERVER_FACTORY_H)
#  define TAO_DEFAULT_SERVER_FACTORY_H

#include "tao/corba.h"

#include "tao/strategy_T.h"

class TAO_Default_Server_Strategy_Factory : public TAO_Server_Strategy_Factory
  // = TITLE
  //   This is the default strategy factory for CORBA servers.  It
  //   allows developers to choose strategies via argument flags.
  //   This design gives substantial freedom for experimentation.
{
public:
  // = Initialization and termination methods.
  TAO_Default_Server_Strategy_Factory (void);
  virtual ~TAO_Default_Server_Strategy_Factory (void);

  virtual int open (void);
  // Call <open> for our strategies.

  // = Server-side ORB Strategy Factory Methods.
  virtual CONCURRENCY_STRATEGY *concurrency_strategy (void);

  virtual TAO_Object_Table *create_object_table (void);
  // Factory method for object tables.

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
  //   where <{which}> is one of <dynamic>, <linear>, or <active>.
  
private:
  void tokenize (char *flag_string);

  u_long thread_flags_;
  // Default thread flags passed to thr_create().
  
  u_long object_table_size_;
  // Default size of object lookup table.
  
  TAO_Demux_Strategy object_lookup_strategy_;
  // The type of lookup/demultiplexing strategy being used

  // = Strategies Used.
  TAO_Reactive_Strategy<TAO_Server_Connection_Handler> reactive_strategy_;
  // A strategy for passively establishing connections which utilizes
  // the Reactor.

  ACE_Thread_Strategy<TAO_Server_Connection_Handler> threaded_strategy_;
  // The threaded strategy used for passively establishing
  // connections.
  
  CONCURRENCY_STRATEGY *concurrency_strategy_;
  // concrete concurrency strategy.

#if 0
  // Someday we'll need these!
  CREATION_STRATEGY *creation_strategy_;
  ACCEPT_STRATEGY *accept_strategy_;
  SCHEDULING_STRATEGY *scheduling_strategy_;
#endif /* 0 */
};

ACE_FACTORY_DECLARE (TAO, TAO_Default_Server_Strategy_Factory)

#endif /* TAO_DEFAULT_SERVER_FACTORY_H */
