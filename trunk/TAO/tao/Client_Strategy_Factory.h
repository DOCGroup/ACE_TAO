// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     Client_Strategy_Factory.h
//
// = AUTHOR
//     Chris Cleeland <cleeland@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_CLIENT_STRATEGY_FACTORY_H
#define TAO_CLIENT_STRATEGY_FACTORY_H
#include "ace/pre.h"

#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "TAO_Export.h"

class ACE_Lock;
class TAO_Transport_Mux_Strategy;
class TAO_Wait_Strategy;
class TAO_Transport;
class TAO_ORB_Core;

class TAO_Export TAO_Client_Strategy_Factory : public ACE_Service_Object
{
  // = TITLE
  //    Abstract factory used by the client to manufacture various
  //    strategies used on the client-side.
public:
  // = Intialization and termination methods.
  TAO_Client_Strategy_Factory (void);
  // Constructor.

  virtual ~TAO_Client_Strategy_Factory (void);
  // Destructor

  virtual ACE_Lock* create_profile_lock (void);
  // Create the lock for the forwarding Profile used by the
  // <TAO_GIOP_Invocation::location_forward> and the
  // <TAO_GIOP_Invocation::start>.

  virtual TAO_Transport_Mux_Strategy *create_transport_mux_strategy (TAO_Transport *transport);
  // Create the correct client request muxing strategy.

  virtual TAO_Wait_Strategy *create_wait_strategy (TAO_Transport *transport);
  // Create the correct client <wait_for_reply> strategy.

  virtual ACE_Lock *create_cached_connector_lock (void);
  // Create the lock to be used by the cached connector.

  virtual ACE_Lock *create_ft_service_retention_id_lock (void);
  // Create a lock to be used by the TAO_Fault_Tolerant_Service class
  // to generate unique retention ids
};

#include "ace/post.h"
#endif /* TAO_CLIENT_STRATEGY_FACTORY_H */
