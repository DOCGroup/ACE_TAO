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

#  include "params.h"

// Silence the compiler via a forward decl.

// @@ Please rename this to TAO_Svc_Handler to protect the namespace.

class Svc_Handler;

class TAO_Client_Factory
  // = TITLE
  //    Abstract factory used by the client to turn out various
  //    strategies used on the client side.
{
public:
  // = CLIENT-SIDE.
  typedef ACE_Strategy_Connector<Svc_Handler, ACE_SOCK_CONNECTOR> CONNECTOR;
  typedef ACE_NOOP_Creation_Strategy<Svc_Handler> NULL_CREATION_STRATEGY;
  typedef ACE_Cached_Connect_Strategy<Svc_Handler, ACE_SOCK_CONNECTOR, ACE_RW_Thread_Mutex> CACHED_CONNECT_STRATEGY;

#if 0
  CONCURRENCY_STRATEGY *concurrency_strategy (void);
#endif
  
  CONNECTOR *connector (void);

  TAO_Client_Factory (void);
  
private:
  // = CLIENT
#if 0
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
  typedef ACE_Creation_Strategy<ROA_Handler> CREATION_STRATEGY;
  typedef ACE_Accept_Strategy<ROA_Handler, ACE_SOCK_ACCEPTOR> ACCEPT_STRATEGY;
  typedef ACE_Concurrency_Strategy<ROA_Handler> CONCURRENCY_STRATEGY;
  typedef ACE_Scheduling_Strategy<ROA_Handler> SCHEDULING_STRATEGY;

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
  // @@ Please add comments.
  ACE_Thread_Strategy<ROA_Handler> threaded_strategy_;
  ACE_Reactive_Strategy<ROA_Handler> reactive_strategy_;

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

#  if defined(__ACE_INLINE__)
#    include "factories.i"
#  endif

#endif /* TAO_FACTORIES_H */
