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

#if 0
#  include "ace/SOCK_Acceptor.h"
#  include "ace/Svc_Handler.h"
#  include "ace/Strategies_T.h"
#  include "ace/Connector.h"
#  include "ace/Synch.h"
#  include "ace/Service_Object.h"

#  include "tao/connect.h"
#  include "tao/objtable.h"
#endif /* 0 */

#include "tao/corba.h"

#  if !defined(SERVER_OBJECT_TABLE_SIZE)
#    define SERVER_OBJECT_TABLE_SIZE 64
#  endif

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

  // = Server-side ORB Strategy Factory Methods.
  virtual CONCURRENCY_STRATEGY *concurrency_strategy (void);

  virtual TAO_Object_Table *create_object_table (void);
  // Factory method for object tables.

  // = Service Configurator hooks.
  virtual int init (int argc, char *argv[]);
  int parse_args (int argc, char *argv[]);
  
private:
  void tokenize (char *flag_string);

  u_long thread_flags_;
  // Default thread flags passed to thr_create().
  
  u_long object_table_size_;
  // Default size of object lookup table.
  
  TAO_Demux_Strategy object_lookup_strategy_;
  // The type of lookup/demultiplexing strategy being used

  // = Strategies Used.
  ACE_Reactive_Strategy<TAO_OA_Connection_Handler> reactive_strategy_;
  // A strategy for passively establishing connections which utilizes
  // the Reactor.

  ACE_Thread_Strategy<TAO_OA_Connection_Handler> threaded_strategy_;
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

ACE_SVC_FACTORY_DECLARE (TAO_Default_Server_Strategy_Factory)

#endif /* TAO_DEFAULT_SERVER_FACTORY_H */
