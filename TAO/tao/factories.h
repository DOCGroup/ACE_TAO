// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//     factories.h
//
// = AUTHOR
//     Chris Cleeland
//
// = VERSION
//     $Id$
// ============================================================================

#if !defined (TAO_FACTORIES_H)
#  define TAO_FACTORIES_H

#  include "ace/SOCK_Acceptor.h"
#  include "ace/SOCK_Connector.h"
#  include "ace/Strategies_T.h"
#  include "ace/Connector.h"
#  include "ace/Synch.h"

#  include "tao/params.h"
#  include "tao/connect.h"
#  include "tao/objtable.h"

class TAO_Client_Connection_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
  // = TITLE
  //      <Svc_Handler> used on the client side and returned
  //      by the <TAO_Client_Factory::CONNECTOR>.
{
public:
  TAO_Client_Connection_Handler (ACE_Thread_Manager* = 0);
  // Do-nothing constructor

  virtual int open (void*);
  // Initialization hook

  void in_use (CORBA_Boolean);
  // Set the in-use flag.

  CORBA_Boolean in_use (void);
  // Return state of the in-use flag.

private:
  CORBA_Boolean in_use_;
  // True value indicates that something is using this handler.
};

class TAO_Client_Factory
  // = TITLE
  //    Abstract factory used by the client to turn out various
  //    strategies used on the client side.
{
public:
  typedef ACE_Strategy_Connector<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR> 
          CONNECTOR;
  typedef ACE_NOOP_Creation_Strategy<TAO_Client_Connection_Handler> 
          NULL_CREATION_STRATEGY;
  typedef ACE_Cached_Connect_Strategy<TAO_Client_Connection_Handler, 
                                      ACE_SOCK_CONNECTOR,
				      ACE_SYNCH_RW_MUTEX>
          CACHED_CONNECT_STRATEGY;

#if defined (TAO_HAS_CLIENT_CONCURRENCY)
  CONCURRENCY_STRATEGY *concurrency_strategy (void);
#endif
  
  CONNECTOR *connector (void);
  // Return a pointer to a connector using appropriate strategies.

  TAO_Client_Factory (void);
  ~TAO_Client_Factory (void);
  
private:
#if defined (TAO_HAS_CLIENT_CONCURRENCY)
  CONCURRENCY_STRATEGY *concurrency_strategy_;
#endif
  CONNECTOR connector_;
  NULL_CREATION_STRATEGY null_creation_strategy_;
  CACHED_CONNECT_STRATEGY caching_connect_strategy_;
};

class TAO_Server_Factory
  // = TITLE
  //    Abstract factory used by the server side to turn out various
  //    strategies of special utility to it.
{
public:
  // = SERVER-SIDE
  typedef ACE_Creation_Strategy<TAO_OA_Connection_Handler> CREATION_STRATEGY;
  typedef ACE_Accept_Strategy<TAO_OA_Connection_Handler, ACE_SOCK_ACCEPTOR> ACCEPT_STRATEGY;
  typedef ACE_Concurrency_Strategy<TAO_OA_Connection_Handler> CONCURRENCY_STRATEGY;
  typedef ACE_Scheduling_Strategy<TAO_OA_Connection_Handler> SCHEDULING_STRATEGY;

  // @@ Please add comments for these methods.

  CREATION_STRATEGY *creation_strategy (void);
  ACCEPT_STRATEGY *accept_strategy (void);
  CONCURRENCY_STRATEGY *concurrency_strategy (void);
  SCHEDULING_STRATEGY *scheduling_strategy (void);
  TAO_Object_Table *object_lookup_strategy (void);
  void object_lookup_strategy (TAO_Object_Table *ot);

  TAO_Server_Factory (void);
  
private:
  // = COMMON
  ACE_Thread_Strategy<TAO_OA_Connection_Handler> threaded_strategy_;
  // The threaded strategy used for passively establishing connections.
  ACE_Reactive_Strategy<TAO_OA_Connection_Handler> reactive_strategy_;
  // A strategy for passively establishing connections which utilizes the Reactor.

  // = SERVER
  CONCURRENCY_STRATEGY *concurrency_strategy_;
  TAO_Object_Table *objtable_;
#if 0
  // Someday we'll need these!
  CREATION_STRATEGY *creation_strategy_;
  ACCEPT_STRATEGY *accept_strategy_;
  SCHEDULING_STRATEGY *scheduling_strategy_;
#endif
};

#  if defined (__ACE_INLINE__)
#    include "factories.i"
#  endif

#endif /* TAO_FACTORIES_H */
