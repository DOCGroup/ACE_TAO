// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//     TAO
// 
// = FILENAME
//     server_factory.h
//
// = AUTHOR
//     Chris Cleeland
//
// = VERSION
//     $Id$
// ============================================================================

#if !defined (TAO_SERVER_FACTORY_H)
#  define TAO_SERVER_FACTORY_H

#  include "ace/SOCK_Acceptor.h"
#  include "ace/Svc_Handler.h"
#  include "ace/Strategies_T.h"
#  include "ace/Connector.h"
#  include "ace/Synch.h"
#  include "ace/Service_Object.h"

#  include "tao/connect.h"
#  include "tao/objtable.h"

class TAO_Server_Strategy_Factory : public ACE_Service_Object
  // = TITLE
  //    Base class for the server's abstract factory that manufactures
  //    various strategies of special utility to it.  This simply
  //    serves as an interface to a subclass that REALLY gets
  //    specified and loaded by the Service Configurator.
{
public:
  // = Initialization and termination methods.
  TAO_Server_Strategy_Factory (void);
  // Constructor.

  virtual ~TAO_Server_Strategy_Factory(void);
  // Destructor.

  // = Server-side factory types.
  typedef ACE_Creation_Strategy<TAO_OA_Connection_Handler>
          CREATION_STRATEGY;
  typedef ACE_Accept_Strategy<TAO_OA_Connection_Handler, ACE_SOCK_ACCEPTOR>
          ACCEPT_STRATEGY;
  typedef ACE_Concurrency_Strategy<TAO_OA_Connection_Handler>
          CONCURRENCY_STRATEGY;
  typedef ACE_Scheduling_Strategy<TAO_OA_Connection_Handler>
          SCHEDULING_STRATEGY;

  // = Factory methods for the server-side strategies.

  virtual CREATION_STRATEGY *creation_strategy (void);
  // Return concrete creation strategy.

  virtual ACCEPT_STRATEGY *accept_strategy (void);
  // Return concrete acceptor strategy.

  virtual CONCURRENCY_STRATEGY *concurrency_strategy (void);
  // Return the concurrency strategy used.

  virtual SCHEDULING_STRATEGY *scheduling_strategy (void);
  // Return the scheduling strategy used.

  virtual TAO_Object_Table *object_lookup_strategy (void);
  // Return the concrete object lookup strategy.
};


#  if defined(__ACE_INLINE__)
#    include "server_factory.i"
#  endif /* __ACE_INLINE__ */

#endif /* TAO_SERVER_FACTORY_H */
