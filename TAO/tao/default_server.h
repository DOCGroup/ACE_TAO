// This may look like C, but it's really -*- C++ -*-

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
// = VERSION
//     $Id$
// ============================================================================

#if !defined (TAO_DEFAULT_SERVER_FACTORY_H)
#  define TAO_DEFAULT_SERVER_FACTORY_H

#  include "ace/SOCK_Acceptor.h"
#  include "ace/Svc_Handler.h"
#  include "ace/Strategies_T.h"
#  include "ace/Connector.h"
#  include "ace/Synch.h"
#  include "ace/Service_Object.h"

#  include "tao/connect.h"
#  include "tao/objtable.h"

#  if !defined(SERVER_OBJECT_TABLE_SIZE)
#    define SERVER_OBJECT_TABLE_SIZE 64
#  endif

class TAO_Default_Server_Strategy_Factory : public TAO_Server_Factory
// = TITLE
//   This is the default strategy factory for CORBA servers.  It
//   basically allows one to choose strategies via argument flags and
//   so gives substantial freedom for experimentation at the expense
//   of memory.
{
public:
  // = Initialization and termination methods.
  TAO_Default_Server_Strategy_Factory (void);
  virtual ~TAO_Default_Server_Strategy_Factory (void);

  // = Server-side ORB Strategy Factory Methods.
  virtual CREATION_STRATEGY *creation_strategy (void);
  virtual ACCEPT_STRATEGY *accept_strategy (void);
  virtual CONCURRENCY_STRATEGY *concurrency_strategy (void);
  virtual SCHEDULING_STRATEGY *scheduling_strategy (void);
  virtual TAO_Object_Table *object_lookup_strategy( void);

  // = SERVICE CONFIGURATOR HOOKS
  virtual int init (int argc, char* argv[]);
  int parse_args (int argc, char* argv[]);
  
private:
  void tokenize (char* flag_string);

  u_long thread_flags_;
  // Default thread flags passed to thr_create().
  
  u_long object_table_size_;
  // Default size of object lookup table.
  
  // = Strategies Used.
  ACE_Reactive_Strategy<TAO_OA_Connection_Handler> reactive_strategy_;
  // A strategy for passively establishing connections which utilizes the Reactor.
  ACE_Thread_Strategy<TAO_OA_Connection_Handler> threaded_strategy_;
  // The threaded strategy used for passively establishing connections.
  
  CONCURRENCY_STRATEGY *concurrency_strategy_;
  // concrete concurrency strategy.

  TAO_Object_Table *objtable_;
  // instance of object table
#if 0
  // Someday we'll need these!
  CREATION_STRATEGY *creation_strategy_;
  ACCEPT_STRATEGY *accept_strategy_;
  SCHEDULING_STRATEGY *scheduling_strategy_;
#endif
};

ACE_SVC_FACTORY_DECLARE (TAO_Default_Server_Strategy_Factory)

#  if defined(__ACE_INLINE__)
#    include "default_server.i"
#  endif

#endif /* TAO_DEFAULT_SERVER_FACTORY_H */
